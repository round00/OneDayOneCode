add_executable(libevent1 libevent1.cpp)
target_link_libraries(libevent1 event)  #链接libevent的库

add_executable(libevent2 libevent2.cpp)
target_link_libraries(libevent2 event)  #链接libevent的库

add_executable(HelloServer HelloServer.cpp)
target_link_libraries(HelloServer event)  #链接libevent的库

add_executable(EchoServer EchoServer.cpp)
target_link_libraries(EchoServer event)

add_executable(HttpServer HttpServer.cpp)
target_link_libraries(HttpServer event)
