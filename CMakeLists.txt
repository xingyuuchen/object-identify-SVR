cmake_minimum_required(VERSION 3.4)
project(oi-svr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(framework)

include_directories(autogen)
include_directories(/usr/local/include)
include_directories(framework)
include_directories(framework/autogen)
include_directories(framework/dao)
include_directories(framework/netscene)
include_directories(framework/utils)

file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} *.cc)
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})

file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} autogen/*.cc)
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})

file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} businesslayer/*.cc)
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})

file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} businesslayer/db/*.cc)
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})

file(GLOB SELF_TEMP_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} businesslayer/netscene_queryimg/*.cc)
list(APPEND SELF_SRC_FILES ${SELF_TEMP_SRC_FILES})

link_directories(/usr/local/lib)

if (${DAEMON})
    add_definitions("-DDAEMON")
endif()

add_executable(${PROJECT_NAME} ${SELF_SRC_FILES})

add_dependencies(${PROJECT_NAME} framework)

target_link_libraries(${PROJECT_NAME} framework protobuf)
