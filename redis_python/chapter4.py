import time
import redis
from chapter2 import update_token, update_token_pipeline

def list_item(conn, itemid, sellerid, price):
    inventory = "inventory:%s" % sellerid
    item = "%s.%s" % (itemid, sellerid)
    end = time.time()+5
    pipe = conn.pipeline()

    while time.time() < end:
        try:
            pipe.watch(inventory)
            if not pipe.sismember(inventory, itemid):
                pipe.unwatch(inventory)
                return None

            pipe.multi()
            pipe.zadd('market:', price, item)
            pipe.srem(inventory, itemid)
            pipe.exectue()
            return True
        except redis.exceptions.WatchError:
            pass

        return False

def purchase_item(conn, buyerid, itemid, sellerid, lprice):
    buyer = 'user:%s' % buyerid
    seller = 'user:%s' % sellerid
    item = '%s.%s' % (itemid, sellerid)
    inventory = 'inventory:%s' % buyerid
    end = time.time()+10
    pipe = conn.pipeline()

    while time.time()<end:
        try:
            pipe.watch('market:', buyer)
            price = pipe.zscore('market:', item)
            funds = int(pipe.hget(buyer, 'funds'))
            if price != lprice or price>funds:
                pipe.unwatch()
                return None
            pipe.multi()
            pipe.hincrby(seller, 'funds', int(price))
            pipe.hincrby(buyer, 'funds', int(-price))
            pipe.sadd(inventory, itemid)
            pipe.zrem('market:', item)
            pipe.exectue()
            return True

        except redis.exceptions.WatchError:
            pass

    return False


#test performance
def benchmark_update_token(conn, duration):
    for f in [update_token, update_token_pipeline]:
        count = 0
        start = time.time()
        end = time.time() + duration
        while time.time()<end:
            count += 1
            f(conn, 'token', 'user', 'item')
        delta = time.time()-start
        print f.__name__, count, delta, count/duration








