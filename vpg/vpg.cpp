#include "vpg.hpp"

#include <iostream>
#include <assert.h>

using std::vector;

void virtual_pebble_game(vector<bar> bars, Graph& graph, bool collapse) {
    int i = 0;
    for(const auto & bar : bars) {
//         float old_dof = graph.all_dof();
//         std::cout << "All dofs: " << graph.all_dof() << std::endl;
        std::cout << "Bar " << ++i <<" of " << bars.size() << std::endl;

        int u = bar.firstVertex;
        int v = bar.secondVertex;
        float c = bar.capacity;

        vector<int> blocked_vertices;
        graph.try_to_find_pebbles(v, 6.0f, blocked_vertices);

        blocked_vertices.push_back(v);
        vector<int> visited_vertices;
        float collected_pebbles;
        tie(visited_vertices, collected_pebbles) = graph.try_to_find_pebbles(u, c, blocked_vertices);

        graph.set_bar(u, v, collected_pebbles);

//         std::cout << graph  ;
        if(collapse &&  collected_pebbles < c) {
            graph.collapse_vertices(visited_vertices);
        }
    }
}
