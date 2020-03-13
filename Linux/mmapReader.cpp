//
// Created by gjk on 2020/3/13.
//

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

struct User{
    int id;
    int age;
};

int main(){

    int fd  =::open("/tmp/mmaptestfile", O_RDONLY);
    if(fd<0){
        perror("::open failed\n");
        return 0;
    }
    //以读权限打开
    struct User* p = (struct User*)::mmap(NULL, sizeof(struct User), PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        perror("::mmap failed\n");
        return 0;
    }
    printf("mmap success\n");
    //内存映射好了之后，fd就可以关闭了
    ::close(fd);

    while(1){
        printf("User id=%d, age=%d\n", p->id, p->age);
        ::sleep(2);
        if(p->age>99)break;
    }

    if(::munmap(p, sizeof(struct User)) < 0){
        perror("::munmap failed\n");
    }
    return 0;
}
