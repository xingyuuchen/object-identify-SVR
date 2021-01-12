cmake_minimum_required(VERSION 3.4.1)
project(oi_svr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# or set(CMAKE_CXX_STANDARD 11)

# aux_source_directory(. src_files)
file(GLOB_RECURSE src_files "*.cc")

message(src_files: ${src_files})

add_executable(${PROJECT_NAME} ${src_files})
