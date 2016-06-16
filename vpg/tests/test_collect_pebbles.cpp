#include "catch.hpp"

#include <vector>

#include "../GraphTester.hpp"
#include "../Graph.hpp"


TEST_CASE("collect_pebbles") {
    {
        Graph grid { 3 };
        GraphTester tester { grid };
        std::vector<int> visited_vertices;
        float collected_pebbles;

        collected_pebbles = grid.collect_pebbles( {0}, visited_vertices, 6.0f);
        REQUIRE( visited_vertices == std::vector<int>{0} );
        REQUIRE( collected_pebbles == 6.0f );

        std::vector<float> expectedVertices = {6.0f, 6.0f, 6.0f};
        REQUIRE(tester.m_vertices == expectedVertices );

        std::vector<float> expectedEdges = { 0, 0, 0,
                                             0, 0, 0,
                                             0, 0, 0 };
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
    }
    {
        Graph grid { 3 };
        grid.set_bar(0, 1, 2.5f);
        GraphTester tester { grid };
        std::vector<int> visited_vertices;
        float collected_pebbles;

        collected_pebbles = grid.collect_pebbles( {0}, visited_vertices, 6.0f);
        REQUIRE( visited_vertices == std::vector<int>( {0, 1} ) );
        REQUIRE( collected_pebbles == 6.0f );

        std::vector<float> expectedVertices = {6.0f, 3.5f, 6.0f};
        REQUIRE(tester.m_vertices == expectedVertices );

        std::vector<float> expectedEdges = { 0, 0, 0,
                                             2.5f, 0, 0,
                                             0, 0, 0 };
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
    }
    {
        Graph grid { 3 };
        grid.set_bar(0, 1, 2.5f);
        GraphTester tester { grid };
        std::vector<int> visited_vertices;
        float collected_pebbles;

        collected_pebbles = grid.collect_pebbles( {0}, visited_vertices, 5.0f);
        REQUIRE( visited_vertices == std::vector<int>( {0, 1} ) );
        REQUIRE( collected_pebbles == 5.0f );

        std::vector<float> expectedVertices = {5.0f, 4.5f, 6.0f};
        REQUIRE(tester.m_vertices == expectedVertices );

        std::vector<float> expectedEdges = { 0, 1.0f, 0,
                                             1.5f, 0, 0,
                                             0, 0, 0 };
        REQUIRE(tester.m_edgeCapacity == expectedEdges );
    }
//     {
//         Graph grid { 3 };
//         grid.set_bar(0, 1, 2.5f);
//         GraphTester tester { grid };
//         std::vector<int> visited_vertices;
//         float collected_pebbles;
// 
//         collected_pebbles = grid.collect_pebbles( {0}, visited_vertices, 12.0f);
// //         REQUIRE( visited_vertices == std::vector<int>( {0, 1} ) );
//         REQUIRE( collected_pebbles == 12.0f );
// 
//         std::vector<float> expectedVertices = {9.5f, 0.0f, 6.0f};
//         REQUIRE(tester.m_vertices == expectedVertices );
// 
//         std::vector<float> expectedEdges = { 0, 1.0f, 0,
//                                              1.5f, 0, 0,
//                                              0, 0, 0 };
//         REQUIRE(tester.m_edgeCapacity == expectedEdges );
//     }

}