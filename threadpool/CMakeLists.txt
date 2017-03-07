cmake_minimum_required(VERSION 3.5)
project(testpool)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# set(CMAKE_CXX_COMPILER /usr/local/bin/clang-omp++)
# set(CMAKE_CXX_COMPILER /usr/local/bin/g++-6)

include_directories(/usr/local/include/libiomp)
link_directories(/usr/local/lib)

set(SOURCE_FILES
		utils/log.h
		utils/log.cpp
        utils/utils.h
        mutex.h
        mutex.cpp
        task.h
        task.cpp
        threadpool.h
        threadpool.cpp
        demo.cpp)

add_executable(testpool ${SOURCE_FILES})
target_link_libraries(testpool pthread iomp5)
