cmake_minimum_required(VERSION 3.10) 

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../..)
project(graph)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O0 -c")

# output library
add_library(graph STATIC 
  f1ExportData.cpp
  f2AfterReading.cpp
)

# target_include_directories(jarvis${PROJECT_VERSION} PUBLIC "${CMAKE_BINARY_DIR}/..")






