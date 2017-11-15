cmake_minimum_required(VERSION 2.8)

# Create the project.
project(ted-join-experiments)

# Compiler flags.
# MUST be declared after project().
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Verify which timing implementation to use.
# NOTE: Comes from common-code.
include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(clock_gettime "time.h" HAVE_CLOCK_GETTIME)
if(NOT HAVE_CLOCK_GETTIME)
  add_definitions(-DGETRUSAGE)
endif()

# Build executable with the experiments.
add_executable(
  ted-join-experiments # EXECUTABLE NAME
  src/experiments.cc   # EXECUTABLE SOURCE
)

# Link the timing library and header file.
add_library(
  TimingLibrary                              # LIBRARY NAME
  external/common-code/timings/timing.cxx    # LIBRARY SOURCE
)
target_link_libraries(
  ted-join-experiments # TARGET EXECUTABLE NAME
  TimingLibrary        # LIBRARY NAME
)
target_include_directories(
  ted-join-experiments                # TARGET EXECUTABLE NAME
  PUBLIC external/common-code/timings # HEADER FILE
)

# Create header-only library 'TreeSimilarity' with our algorithms.
add_library(
  TreeSimilarity INTERFACE
)
target_include_directories(
  TreeSimilarity INTERFACE
  external/tree-similarity-private/src/node
  external/tree-similarity-private/src/label
  external/tree-similarity-private/src/parser
)

# Let the compiler know to find the header files in TreeSimilarity library.
target_link_libraries(
  ted-join-experiments # EXECUTABLE NAME
  TreeSimilarity       # LIBRARY NAME
)
