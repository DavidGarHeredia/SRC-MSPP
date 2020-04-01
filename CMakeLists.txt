cmake_minimum_required(VERSION 3.10)

# WHERE TO LOCATE THE OUTPUT (i.e.: executable file)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

# GUROBI VERSION
set(GUROBI "/opt/gurobi900/linux64") # [WARNING]: TO BE EDITED BY THE USER!!!
set(LGUROBI "gurobi90") # [WARNING]: TO BE EDITED BY THE USER!!!

# PROJECT NAME
project(SRC-MSPP)

# CFLAGS
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3  ${OpenMP_CXX_FLAGS}")

# FINDING OpenMP
find_package(OpenMP REQUIRED)

# LOCATE PERSONAL LIBS (AND HEADERS) TO LINK AFTERWARDS
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# SAME AS BEFORE, BUT FOR GUROBI
link_directories(${GUROBI}/lib)
include_directories(${GUROBI}/include)

# CREATING THE EXECUTABLE
add_executable(executable
  src/main.cpp
  src/HeuristicII.cpp
  src/MathSolver.cpp
  src/Reading.cpp
)

# LINKING LIBRARIES
target_link_libraries(executable OpenMP::OpenMP_CXX)
target_link_libraries(executable graph jarvis)
target_link_libraries(executable gurobi_g++5.2)
target_link_libraries(executable ${LGUROBI})
