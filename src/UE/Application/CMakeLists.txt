project(UeApplication)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Ports SRC_LIST)
aux_source_directory(States SRC_LIST)

include_directories(Ports States)

add_library(${PROJECT_NAME} ${SRC_LIST}
        States/ConstructSmsState.cpp
        States/ConstructSmsState.hpp)
target_link_libraries(${PROJECT_NAME} Common)
