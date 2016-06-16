#include "catch.hpp"

#include "../GraphTester.hpp"
#include "../Graph.hpp"

#include <iostream>

TEST_CASE("graph") {
    Graph grid { 3 };
    GraphTester tester { grid };

    grid.set_bar(1, 0, 2.5f);
    std::vector<float> expectedVertices = {6, 3.5, 6};
    REQUIRE(tester.m_vertices == expectedVertices );

    std::vector<float> expectedEdges = {   0, 0, 0,
                                        2.5, 0, 0,
                                           0, 0, 0 };
    REQUIRE(tester.m_edgeCapacity == expectedEdges );

    //REQUIRE_THROW( grid.set_bar(-1, -1, 1) );
//     REQUIRE_THROWS( grid.set_bar(7, 7, 1) );

}