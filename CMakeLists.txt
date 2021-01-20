cmake_minimum_required(VERSION 3.4.1)
project(oi_svr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lprotobuf -pthread")
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

# aux_source_directory(. src_files)
file(GLOB_RECURSE src_files "*.cc")

message(src_files: ${src_files})

include_directories(/usr/local/include)

link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} ${src_files})
