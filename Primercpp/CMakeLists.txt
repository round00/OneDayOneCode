set(primer_src
        Chapter15.cpp
        Quote.cpp
        Query.cpp
        TextQuery.cpp
        Chapter18.cpp Chapter16.cpp Blob.h)

add_executable(primercpp_chapter15 ${primer_src})
add_executable(primercpp_chapter18 Chapter18.cpp)
add_executable(primercpp_chapter16 Chapter16.cpp)
add_executable(primercpp_chapter17 Chapter17.cpp)
add_executable(tmprogramming template_metaprogramming.cpp)

add_executable(mysharedptr mysharedptr.cpp)
add_executable(OperatorOverLoad OperatorOverLoad.cpp)
add_executable(friend friend.cpp)