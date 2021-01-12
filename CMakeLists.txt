project(oi_svr)

cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(. src_files)

add_executable(${PROJECT_NAME} ${src_files})
