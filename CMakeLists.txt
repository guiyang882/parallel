cmake_minimum_required(VERSION 3.5)
project(testsparse)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fprofile-arcs -ftest-coverage -g -pg")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(CMAKE_CXX_COMPILER /usr/local/bin/clang-omp++)

find_package(Eigen 3 QUIET)
if(NOT Eigen_FOUND)
  include(/usr/local/Cellar/eigen/3.3.2/share/cmake/Modules/FindEigen3.cmake)
  set(Eigen_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
endif()
include_directories(${Eigen_INCLUDE_DIR})
include_directories(/usr/local/include/libiomp)
link_directories(/usr/local/lib)

ADD_DEFINITIONS(-D_OPENMP)

set(SOURCE_FILES
        sparserepresentation.hpp
        test.cpp)

add_executable(testsparse ${SOURCE_FILES})
target_link_libraries(testsparse pthread iomp5)
