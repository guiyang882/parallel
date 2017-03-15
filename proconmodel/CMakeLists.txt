cmake_minimum_required(VERSION 3.5)
project(proconmodel)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# set(CMAKE_CXX_COMPILER /usr/local/bin/clang-omp++)
# set(CMAKE_CXX_COMPILER /usr/local/bin/g++-6)

include_directories(/usr/local/include/libiomp)
include_directories(/Users/liuguiyang/Documents/CodeProj/ConsoleProj/parallel/thirdpart)
link_directories(/usr/local/lib)

set(thirdpath /Users/liuguiyang/Documents/CodeProj/ConsoleProj/parallel/thirdpart)

set(SOURCE_FILES
        ${thirdpath}/log/log.h
        ${thirdpath}/log/log.cpp
        demo.cpp)

add_executable(proconmodel ${SOURCE_FILES})
target_link_libraries(proconmodel pthread iomp5)
