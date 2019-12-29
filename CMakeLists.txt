cmake_minimum_required(VERSION 3.14)
project(OneDayOneCode)

set(CMAKE_CXX_STANDARD 11)

#centos7 上很多新装的东西都放到了/usr/local/下，而不是原来的/usr/
include_directories(
        /usr/local/include/
)
link_directories(
        /usr/local/lib/
)
add_executable(main main.cpp)
add_executable(Bitmap Bitmap.cpp)
add_executable(QuickSort QuickSort.cpp)
add_executable(QuickSort_list QuickSort_list.cpp)
add_executable(InsertSort InsertSort.cpp)
add_executable(HeapSort HeapSort.cpp)
add_executable(BinarySearchTree BinarySearchTree.cpp)
add_executable(OperatorOverLoad OperatorOverLoad.cpp)
add_executable(epoll_reactor epoll_reactor.cpp)

add_executable(libevent1 libevent/libevent1.cpp)
target_link_libraries(libevent1 event)  #链接libevent的库

add_executable(libevent2 libevent/libevent2.cpp)
target_link_libraries(libevent2 event)  #链接libevent的库

add_executable(mysharedptr mysharedptr.cpp)

add_executable(HelloServer libevent/HelloServer.cpp)
target_link_libraries(HelloServer event)  #链接libevent的库

add_executable(EchoServer libevent/EchoServer.cpp)
target_link_libraries(EchoServer event)

add_executable(HttpServer libevent/HttpServer.cpp)
target_link_libraries(HttpServer event)