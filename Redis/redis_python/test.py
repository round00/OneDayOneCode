import redis

#start redis: src/redis-server --protected-mode no

conn = redis.Redis(host="121.36.51.156", port=8765, password="nibuzhidao1")
# conn.zadd('ip2cityid', {'216721_2': 271873233})
# print conn.zcard('ip2cityid')
print conn.info()['connected_clients']
print conn.ping()
# conn.set("foo1", "bar")
# print conn.get("foo1")

#

#strings command
# conn.substr()
# conn.getrange()
# conn.setrange()
# conn.setbit()
#
# #list
# conn.rpoplpush()
#
# #hash
#
# conn.hset()
# conn.hget()
# conn.hmset()
# conn.hmget()
# conn.hexists()
# conn.hkeys()
# conn.hvals()
#
# #zset
# conn.zadd()
# conn.zrem()
# conn.zcard()
# conn.zincrby()
# conn.zcount()
# conn.zrank()
# conn.zscore()
# conn.zrange()
# conn.zrevrank()
# conn.zrevrange()
# conn.zrangebyscore()
# conn.zrevrangebyscore()
# conn.zremrangebyrank()
# conn.zremrangebyscore()
# conn.zinterstore()
# conn.zunionstore()
#
# #pub/sub
# pb = conn.pubsub()
# pb.subscribe()
# pb.unsubscribe()
# pb.psubscribe()
# pb.punsubscribe()
# pb.listen()
#
# conn.publish()
#
# #others
# conn.sort()
# conn.info()
# conn.ping()
#
# #transcation
# conn.watch()
# conn.unwatch()
# transcation = conn.pipeline()
# transcation.multi()
# transcation.incr()
# transcation.zadd()
# transcation.execute()
#
# #ttl
# conn.persist()
# conn.ttl()
# conn.pttl()
# conn.expire()
# conn.expireat()
# conn.pexpire()
# conn.pexpireat()
#
# #save data to disk
# conn.bgsave()
# conn.save()
# conn.shutdown()
# conn.bgrewriteaof()
# conn.slaveof()


#redis-benchmark -c l -q
#