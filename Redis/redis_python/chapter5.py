import redis
import time
import logging
from datetime import datetime
import bisect
import json
import functools

SEVERITY = {
    logging.DEBUG : 'debug',
    logging.INFO : 'info',
    logging.WARNING : 'warning',
    logging.ERROR : 'error',
    logging.CRITICAL : 'critical'
}
SEVERITY.update((name, name) for name in SEVERITY.values())


config_connection = None

def log_recent(conn, name, message, severity=logging.INFO, pipe=None):
    severity = str(SEVERITY.get(severity, severity)).lower()
    destination = 'recent:%s:%s' % (name, severity)
    message = time.asctime() + ' ' + message
    pipe = pipe or conn.pipeline()
    pipe.lpsuh(destination, message)
    pipe.ltrim(destination, 0, 99)
    pipe.execute()


def log_common(conn, name, message, severity=logging.INFO, timeout=5):
    severity = str(SEVERITY.get(severity, severity)).lower()
    destination = 'common:%s:%s' % (name, severity)
    start_key = destination + ':start'
    pipe = conn.pipeline()
    end = time.time()+timeout
    while time.time() < end:
        try:
            pipe.watch(start_key)
            now = datetime.utcnow().timetuple()
            hourstart = datetime(*now[:4]).isoformat()
            existing = pipe.get(start_key)
            pipe.multi()
            if existing and existing < hourstart:
                pipe.rename(destination, destination + ':last')
                pipe.rename(start_key, destination + ':plast')
                pipe.set(start_key, hourstart)
            pipe.zincrby(destination, message)
            log_recent(pipe, name, message, severity, pipe)
            return
        except redis.exceptions.WatchError:
            continue


PRECISION = [1, 5, 60, 300, 3600, 18000, 86400]
def update_counter(conn, name, count=1, now=None):
    now = now or time.time()
    pipe = conn.pipeline()
    for prec in PRECISION:
        pnow = int(now/prec) * prec
        hash = '%s.%s' % (prec, name)
        pipe.zadd('known:', 0, hash)
        pipe.hincrby('count:' + hash, pnow, count)
    pipe.execute()


def get_counter(conn, name, precision):
    hash = '%s.%s' % (precision, name)
    data = conn.hgetall('count:' + hash)
    to_return = []
    for key, val in data.iteritems():
        to_return.append((int(key), int(val)))
    to_return.sort()
    return to_return


QUIT = False
SAMPLE_COUNT = 10
def clean_counters(conn):
    pipe = conn.pipeline()
    passes = 0
    while not QUIT:
        start = time.time()
        index = 0
        while index < conn.zcard('known'):
            hash = conn.zrange('known:', index, index)
            index += 1
            if not hash:
                break
            hash = hash[0]
            prec = int(hash.partition(':')[0])
            bprec = int(prec/60) or 1
            if passes % bprec:
                continue
            hkey = 'count:' + hash
            cutoff = time.time() - SAMPLE_COUNT * prec
            samples = map(int, conn.hkeys(hkey))
            samples.sort()
            remove = bisect.bisect_right(samples, cutoff)

            if remove:
                conn.hdel(hkey, *samples[:remove])
                if remove == len(samples):
                    try:
                        pipe.watch(hkey)
                        if not pipe.hlen(hkey):
                            pipe.multi()
                            pipe.zrem('known:', hash)
                            pipe.execute()
                            index -= 1
                        else:
                            pipe.unwatch()
                    except redis.exceptions.WatchError:
                        pass
        passes += 1
        duration = min(int(time.time() - start) + 1, 60)
        time.sleep(max(60 - duration, 1))


def set_config(conn, type, component, config):
    conn.set(
        'config:%s:%s' % (type, component),
        json.dumps(config)
    )


CONFIGS = {}
CHECKED = {}
def get_config(conn, type, component, wait=1):
    key = 'config:%s:%s' % (type, component)
    if CHECKED.get(key) < time.time() - wait:
        CHECKED[key] = time.time()
        config = json.loads(conn.get(key) or {})
        config = dict((str(k), config[k]) for k in config)
        old_config = CONFIGS.get(key)
        if config != old_config:
            CONFIGS[key] = config
    return CONFIGS.get(key)


REDIS_CONNECTIONS = {}
def redis_connection(component, wait=1):
    key = 'config:redis:%s' % component

    def wrapper(function):
        @functools.wraps(function)
        def call(*args, **kwargs):
            old_config = CONFIGS.get(key, object())
            new_config = get_config(config_connection, 'redis', wait)
            config = {}
            for k, v in new_config.items():
                config[k.encode('utf-8')] = v
            if config != old_config:
                REDIS_CONNECTIONS[key] = redis.Redis(**config)
            return function(REDIS_CONNECTIONS.get(key), *args, **kwargs)
        return call
    return wrapper


@redis_connection('logs')
def log_recent():
    pass



