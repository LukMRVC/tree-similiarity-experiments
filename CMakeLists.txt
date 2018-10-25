cmake_minimum_required(VERSION 2.8)

# Create the project.
project(ted-experiments)

# Compiler flags.
# MUST be declared after project().
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")

# Verify which timing implementation to use.
# NOTE: Comes from common-code.
include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(clock_gettime "time.h" HAVE_CLOCK_GETTIME)
if(NOT HAVE_CLOCK_GETTIME)
  add_definitions(-DGETRUSAGE)
endif()

# Build executable with the experiments.
add_executable(
  ted-join-experiments                   # EXECUTABLE NAME
  src/join_algs/join_algs_experiments.cc # EXECUTABLE SOURCE
)
target_link_libraries(
  ted-join-experiments # TARGET EXECUTABLE NAME
  TimingLibrary        # LIBRARY NAME
)
target_include_directories(
  ted-join-experiments                # TARGET EXECUTABLE NAME
  PUBLIC external/common-code/timings # HEADER FILE
)

# Build executable with the experiments.
add_executable(
  ted-algs-experiments                   # EXECUTABLE NAME
  src/ted_algs/ted_algs_experiments.cc   # EXECUTABLE SOURCE
)
target_link_libraries(
  ted-algs-experiments # TARGET EXECUTABLE NAME
  TimingLibrary        # LIBRARY NAME
)
target_include_directories(
  ted-algs-experiments                # TARGET EXECUTABLE NAME
  PUBLIC external/common-code/timings # HEADER FILE
)

# Create the timing library.
add_library(
  TimingLibrary                              # LIBRARY NAME
  external/common-code/timings/timing.cxx    # LIBRARY SOURCE
)

# Create header-only library 'TreeSimilarity' with our algorithms.
add_library(
  TreeSimilarity INTERFACE
)
target_include_directories(
  TreeSimilarity INTERFACE
  external/tree-similarity/src/cost_model
  external/tree-similarity/src/data_structures
  external/tree-similarity/src/join
  external/tree-similarity/src/join/tjoin
  external/tree-similarity/src/join/tang
  external/tree-similarity/src/join/naive
  external/tree-similarity/src/label
  external/tree-similarity/src/node
  external/tree-similarity/src/parser
  external/tree-similarity/src/ted
  external/tree-similarity/src/ted_ub
)

# Let the compiler know to find the header files in TreeSimilarity library.
target_link_libraries(
  ted-join-experiments # EXECUTABLE NAME
  TreeSimilarity       # LIBRARY NAME
)

# Let the compiler know to find the header files in TreeSimilarity library.
target_link_libraries(
  ted-algs-experiments # EXECUTABLE NAME
  TreeSimilarity       # LIBRARY NAME
)
