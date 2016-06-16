cmake_minimum_required(VERSION 2.8.4)
project(vpg)

include_directories(tests/catch/include/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES bars.cpp
                Route.cpp
                Graph.cpp
                GraphTester.cpp
                vpg.cpp )

set( HEADERS bars.cpp
             Route.hpp
             Graph.hpp
             GraphTester.hpp
             vpg.hpp )

add_executable(vpg ${SOURCE_FILES} main.cpp )

add_executable(generate_lattice generate_lattice.cpp)
add_executable(generate_realization generate_realization.cpp)

add_executable(test_side_by_side tests/test_collapse_side_by_side.cpp Graph.cpp bars.cpp Route.cpp)
add_executable(sweep sweep-solve.cpp Graph.cpp bars.cpp Route.cpp vpg.cpp)

# set( TESTS   tests/test_backtrack.cpp
#              tests/test_graph.cpp
#              tests/test_collect_pebbles.cpp)

# add_executable(test ${TESTS} ${SOURCE_FILES} )