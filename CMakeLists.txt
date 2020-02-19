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

add_subdirectory(DataStructer)
add_subdirectory(Libevent)
add_subdirectory(DesignPattern)
add_subdirectory(Primercpp)
add_subdirectory(Redis)
add_subdirectory(Linux)