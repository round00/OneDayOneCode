//
// Created by gjk on 2020/1/29.
//
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <stdio.h>
#include <event.h>

const char* IP = "localhost";
const int PORT = 6379;

int testSyncApi(){
    printf("==============Sync==============\n");
    redisContext* rc = redisConnect(IP, PORT);
    redisReply* reply;
    if(rc->err){
        redisFree(rc);
        printf("redis connect failed, err=%s\n", rc->errstr);
        return 0;
    }

    printf("redis connect success!\n");
    reply = (redisReply*)redisCommand(rc, "set foo bar");
    printf("set ok, redis reply=%s\n", reply->str);
    reply = (redisReply*)redisCommand(rc, "get foo");
    printf("get ok, redis reply=%s\n", reply->str);
    printf("==============Over==============\n");
    return 0;
}

int main()
{
    testSyncApi();
    return 0;
}