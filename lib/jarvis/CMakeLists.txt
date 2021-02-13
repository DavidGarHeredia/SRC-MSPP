cmake_minimum_required(VERSION 3.10) 

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../..)
project(jarvis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O0 -c")

# output library
add_library(jarvis STATIC 
  Log.cpp
  Random.cpp
  TableCreator.cpp
  Timer.cpp
)

# target_include_directories(jarvis${PROJECT_VERSION} PUBLIC "${CMAKE_BINARY_DIR}/..")






