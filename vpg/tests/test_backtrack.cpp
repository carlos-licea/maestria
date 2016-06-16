#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../GraphTester.hpp"
#include "../Graph.hpp"

#include <iostream>

TEST_CASE("backtrack") {

    {
        //default tests
        Graph grid { 3 };
        GraphTester tester { grid };
        grid.set_bar(1, 0, 2.5f);
        std::vector<float> expectedVertices = {6, 3.5f, 6};

        std::vector<float> expectedEdges = {   0, 0, 0,
                                               2.5f, 0, 0,
                                               0, 0, 0 };

        float backtrackedPebbles;
        backtrackedPebbles = grid.back_track(0, 0, {});
        REQUIRE(tester.m_vertices == expectedVertices );
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
        REQUIRE(backtrackedPebbles == 0 );


        backtrackedPebbles = grid.back_track(2.5f, 0, {});
        REQUIRE(tester.m_vertices == expectedVertices );
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
        REQUIRE(backtrackedPebbles == 0 );


        backtrackedPebbles = grid.back_track(0, 0, {0});
        REQUIRE(tester.m_vertices == expectedVertices );
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
        REQUIRE(backtrackedPebbles == 0 );


        backtrackedPebbles = grid.back_track(3.5f, 0, {0});
        REQUIRE(tester.m_vertices == expectedVertices );
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
        REQUIRE(backtrackedPebbles == 3.5f );
    }

    {
        //backtrack through 1 bar
        Graph grid { 3 };
        GraphTester tester { grid };

        grid.set_bar(1, 0, 2.5f);
        grid.back_track(2.5f, 0, {1, 0});

        std::vector<float> expectedVertices = {3.5f, 6.0f, 6.0f};
        REQUIRE(tester.m_vertices == expectedVertices );

        std::vector<float> expectedEdges = { 0, 2.5, 0,
                                             0, 0, 0,
                                             0, 0, 0 };
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
    }

    {
        //backtrack through 2 bars
        Graph grid { 3 };
        GraphTester tester { grid };

        grid.set_bar(0, 1, 2.5f);
        grid.set_bar(1, 2, 2.5f);
        grid.back_track(2.5f, 2, {0, 1, 2});

        std::vector<float> expectedVertices = {6.0f, 3.5f, 3.5f};
        REQUIRE(tester.m_vertices == expectedVertices );

        std::vector<float> expectedEdges = { 0, 0, 0,
                                             2.5f, 0, 0,
                                             0, 2.5f, 0 };
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
    }

}
