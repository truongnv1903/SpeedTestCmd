cmake_minimum_required(VERSION 3.5)
project(SpeedTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)              

add_executable(${PROJECT_NAME} ${SOURCES})

