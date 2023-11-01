cmake_minimum_required(VERSION 3.7)

# Create the project.
project(ted-experiments)

# Compiler flags.
# MUST be declared after project().
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O0 -g")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O3")
endif ()
# Verify which timing implementation to use.
# NOTE: Comes from common-code.
include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(clock_gettime "time.h" HAVE_CLOCK_GETTIME)
if(NOT HAVE_CLOCK_GETTIME)
  add_definitions(-DGETRUSAGE)
endif()

#set(Boost_NO_WARN_NEW_VERSIONS 1)
#SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/lukas/school/boost_1_83_0")
#SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/lukas/school/boost_1_83_0/stage/lib")
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.83.0 REQUIRED COMPONENTS system iostreams)
#if (Boost_FOUND)
#  include_directories( ${Boost_INCLUDE_DIRS} )
#endif ()


# Moravec APTED distance dataset calculator
add_executable(ted-ds-dist
        src/lb/ted_ds_dist.cpp
        src/lb/ted_ds_dist.h
)

# Moravec LB experiments
add_executable(ted-lb-experiments
        src/lb/ted_lb_experiments.cpp
        src/lb/ted_lb_experiments.h
)

target_link_libraries(
    ted-ds-dist
    TimingLibrary
#    Boost::iostreams
)

target_link_libraries(
        ted-lb-experiments
        TimingLibrary
)

target_include_directories(
      ted-ds-dist
      PUBLIC external/common-code/timings
)
#
target_include_directories(
        ted-lb-experiments
        PUBLIC external/common-code/timings
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
  external/tree-similarity/src/join/binary_branches
  external/tree-similarity/src/join/label_histogram
  external/tree-similarity/src/join/degree_histogram
  external/tree-similarity/src/join/leaf_dist_histogram
  external/tree-similarity/src/join/histogram
  external/tree-similarity/src/label
  external/tree-similarity/src/node
  external/tree-similarity/src/parser
  external/tree-similarity/src/ted
  external/tree-similarity/src/ted_ub
  external/tree-similarity/src/ted_lb
  external/tree-similarity/src/join/guha
)




find_package(Threads REQUIRED)
# Let the compiler know to find the header files in TreeSimilarity library.

# Link LB experiments to TreeSim library
target_link_libraries(
        ted-ds-dist
        TreeSimilarity
        Threads::Threads
)
# Let the compiler know to find the header files in TreeSimilarity library.
target_link_libraries(
        ted-lb-experiments
        TreeSimilarity
        Threads::Threads
)