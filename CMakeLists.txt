cmake_minimum_required(VERSION 3.14)
project(OneDayOneCode)

set(CMAKE_CXX_STANDARD 11)

#add_executable(OneDayOneCode main.cpp Bitmap.cpp QuickSort.cpp InsertSort.cpp)

add_executable(Bitmap Bitmap.cpp)
add_executable(QuickSort QuickSort.cpp)
add_executable(QuickSort_list QuickSort_list.cpp)
add_executable(InsertSort InsertSort.cpp)