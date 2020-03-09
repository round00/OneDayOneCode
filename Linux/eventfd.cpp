//
// Created by gjk on 2020/3/7.
//
#include <sys/eventfd.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
using namespace std;
int main()
{
    int fd[2];
    ::pipe(fd);
    int flags = ::fcntl(fd[0],F_GETFL,0);
    flags |= O_NONBLOCK;
//    ::fcntl(fd[0],F_SETFL,flags);

    int64_t x = 23;
    ::write(fd[1], &x, sizeof(x));
    x+=3;
    ::write(fd[1], &x, sizeof(x));

    int n = ::read(fd[0], &x, sizeof(x));
    cout<<n<<" "<<x<<endl;
    n = ::read(fd[0], &x, sizeof(x));
    cout<<n<<" "<<x<<endl;
    n = ::read(fd[0], &x, sizeof(x));
    cout<<n<<" "<<x<<endl;
    return 0;
}