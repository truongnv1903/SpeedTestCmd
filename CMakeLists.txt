cmake_minimum_required(VERSION 3.5)
project(SpeedTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")
set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/TCP)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp
		${PROJECT_SOURCE_DIR}/src/TCP/*.cpp)              

add_executable(${PROJECT_NAME} ${SOURCES})

