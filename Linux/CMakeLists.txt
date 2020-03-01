

add_executable(epoll_reactor epoll_reactor.cpp)
add_executable(accept_herd accept_herd.cpp)
add_executable(epoll_herd epoll_herd.cpp)

add_executable(aio_glibc aio_glibc.cpp)
target_link_libraries(aio_glibc rt)

add_executable(signal  signal.cpp)
add_executable(echoserver_plain echoserver_plain.cpp util.cpp)
add_executable(echoserver_fork echoserver_fork.cpp util.cpp)
add_executable(echoserver_select echoserver_select.cpp util.cpp)
add_executable(echoserver_poll echoserver_poll.cpp util.cpp)
add_executable(echoserver_epoll echoserver_epoll.cpp util.cpp)
add_executable(echoserver_thread echoserver_thread.cpp util.cpp)
target_link_libraries(echoserver_thread pthread)

add_executable(thread_sync_error thread_sync_error.cpp)
target_link_libraries(thread_sync_error pthread)

add_executable(tiny_httpserver tiny_httpserver.cpp util.cpp)