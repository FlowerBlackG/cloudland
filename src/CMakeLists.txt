#[[
    cloudland building script
    created on 2024.7.25 at Haidian, Beijing
]]

cmake_minimum_required(VERSION 3.29.2)


#[[
    Cloud Land version
]]
set(CLOUDLAND_VERSION_MAJOR 1)
set(CLOUDLAND_VERSION_MINOR 0)
set(CLOUDLAND_VERSION_PATCH 0)
set(CLOUDLAND_VERSION_TAIL "") # example: "-dev"

set(CLOUDLAND_VERSION_CODE 1)

string(TIMESTAMP build_time_iso8601 UTC)  # example: 2024-04-19T13:39Z
string(TIMESTAMP build_time_human_readable "%B %d, %Y %H:%M:%S %Z")
set(CLOUDLAND_BUILD_TIME_ISO8601 ${build_time_iso8601})
set(CLOUDLAND_BUILD_TIME_HUMAN_READABLE ${build_time_human_readable})

set(
    CLOUDLAND_VERSION_NAME 
    "${CLOUDLAND_VERSION_MAJOR}.${CLOUDLAND_VERSION_MINOR}.${CLOUDLAND_VERSION_PATCH}${CLOUDLAND_VERSION_TAIL}"
)

#[[
    basic config
]]
project(cloudland)
configure_file(
    ${PROJECT_SOURCE_DIR}/config.cpp.in 
    ${PROJECT_SOURCE_DIR}/config.cpp
)

# use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


include_directories(${PROJECT_SOURCE_DIR})


#[[
    definitions
]]

add_compile_definitions(FUSE_USE_VERSION=316)


#[[ 
    GCC(g++) options
]]
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")


#[[ 
    targets
]]
file(GLOB_RECURSE CPP_SOURCE_FILES *.cpp)
file(GLOB_RECURSE C_SOURCE_FILES *.c)
add_executable(
    ${PROJECT_NAME} ${CPP_SOURCE_FILES} ${C_SOURCE_FILES}
)


#[[ 
    find libs
]]
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake-modules)
find_package(FUSE3 REQUIRED)
find_package(CURL REQUIRED)



#[[
    link libs
]]

include_directories(
    ${FUSE3_INCLUDE_DIR} 
    ${CURL_INCLUDE_DIR} 
)


target_link_libraries(
    ${PROJECT_NAME} 

    ${FUSE3_LIBRARIES}
    ${CURL_LIBRARIES}
)


