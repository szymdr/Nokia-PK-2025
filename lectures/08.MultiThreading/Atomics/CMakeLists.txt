#make file that will compile the code in this directory with pthread linked and C++20 support
cmake_minimum_required(VERSION 3.10)
project(Atomics)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

#add all cpp files in the directory to the project
file(GLOB_RECURSE SOURCES "*.cpp")
#add all header files in the directory to the project
file(GLOB_RECURSE HEADERS "*.hpp")
#add all source files to the project
add_executable(Atomics main.cpp ${HEADERS})
#link pthread library
target_link_libraries(Atomics pthread)


