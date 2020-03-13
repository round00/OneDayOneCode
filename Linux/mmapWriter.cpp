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
    struct User user = {17, 2};
    //没有文件要创建，如果没有文件是无法mmap的。如果不加0644权限是无法访问这个文件的
    int fd  =::open("/tmp/mmaptestfile", O_RDWR|O_CREAT|O_TRUNC, 0644);
    if(fd<0){
        perror("::open failed\n");
        return 0;
    }
    //文件大小为0的话，mmap出来的空间也是0， 所以先扩大空间
    ftruncate(fd, sizeof(struct User));
    //以写权限打开，文件映射是将磁盘的文件映射到虚拟地址空间中
    struct User* p = (struct User*)::mmap(NULL, sizeof(struct User), PROT_WRITE, MAP_SHARED, fd, 0);
    //这个判断不能省略
    if(p == MAP_FAILED){
        perror("::mmap failed\n");
        return 0;
    }
    printf("mmap success\n");
    //内存映射好了之后，fd就可以关闭了
    ::close(fd);

    while(user.age < 100){
        memcpy(p, &user, sizeof(struct User));
        user.age++;
        ::sleep(1);
    }

    if(::munmap(p, sizeof(struct User)) < 0){
        perror("::munmap failed\n");
    }
    return 0;
}
