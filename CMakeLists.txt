cmake_minimum_required(VERSION 2.8.4)
project(mpi)
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpic++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Process.cpp Process.h Mpi.cpp Mpi.h Gravedigger.cpp Gravedigger.h Management.cpp Management.h)
add_executable(mpi ${SOURCE_FILES})