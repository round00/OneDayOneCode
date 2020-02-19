import redis
import time
import csv
import json


HOST = '121.36.51.156'
PORT = 6379


def ip_to_score(ip):
    score = 0
    if '/' in ip:
        ip = ip.split('/')[0]
    for v in ip.split('.'):
        score = score*256 + int(v, 10)
    return score


def import_ips_to_redis(conn, filename):
    fcsv = csv.reader(open(filename, 'rb'))
    for count, row in enumerate(fcsv):
        start_ip = row[0] if row else ' '
        if 'i' in start_ip.lower():
            continue
        if '.' in start_ip:
            start_ip = ip_to_score(start_ip)
        elif start_ip.isdigit():
            start_ip = int(start_ip, 10)
        else:
            continue
        cityid = row[2] + '_' + str(count)
        print cityid, start_ip
        res = conn.zadd('ip2cityid:', {cityid: start_ip})
        if count % 1000 == 0:
            print 'count=' + str(count)


def import_cities_to_redis(conn, filename):
    fcsv = csv.reader(open(filename, 'rb'))
    for cnt, row in enumerate(fcsv):
        if len(row)<4 or not row[0].isdigit():
            continue
        row = [i.decode('utf-8') for i in row]
        city_id = row[0]
        country = row[1]
        region = row[2]
        city = row[3]
        conn.hset('cityid2city:', city_id, json.dumps([city, region, country]))
        if cnt%1000==0:
            print cnt


def find_city_by_ip(conn, ip):
    if isinstance(ip, str):
        ip = ip_to_score(ip)
    city_id = conn.zrevrangebyscore('ip2cityid:', ip, start=0, num=1)
    if not city_id:
        return None
    city_id = city_id.partition('_')[0]
    city_info = conn.hget('cityid2city:', city_id)
    return json.loads(city_info)


if __name__ == '__main__':
    cli = redis.Redis(host=HOST, port=PORT, db=15)
    # import_ips_to_redis(cli, 'D:\\downloads\\GeoLite2-City-CSV_20200114\\GeoLite2-City-Blocks-IPv4.csv')
    import_cities_to_redis(cli, 'D:\\downloads\\GeoLite2-City-CSV_20200114\\GeoLite2-City-Locations-zh-CN.csv')
    while True:
        print "Input ip: ",
        ip = input()
        addr = find_city_by_ip(cli, ip)
        print addr





