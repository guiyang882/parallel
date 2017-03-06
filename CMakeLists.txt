cmake_minimum_required(VERSION 3.5)
project(testsparse)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

find_package(Eigen 3 QUIET)
if(NOT Eigen_FOUND)
  include(/usr/local/Cellar/eigen/3.3.2/share/cmake/Modules/FindEigen3.cmake)
  set(Eigen_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
endif()
include_directories(${Eigen_INCLUDE_DIR})

set(SOURCE_FILES
        sparserepresentation.hpp
        test.cpp)

add_executable(testsparse ${SOURCE_FILES})
target_link_libraries(testsparse pthread)
