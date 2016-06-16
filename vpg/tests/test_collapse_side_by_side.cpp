#include <iostream>
#include <assert.h>
#include <vector>
#include <tuple>

#include "../bars.hpp"
#include "../Graph.hpp"

using std::vector;

int main() {
    auto bars = read_bars("realization");
    const int vertex_count = read_vertex_count("../lattice-description");

    Graph with_collapse(vertex_count);
    Graph without_collapse(vertex_count);

    int i = 0;
    for(const auto & bar : bars) {
        float old_dof_collapse = with_collapse.all_dof();
        float old_dof = without_collapse.all_dof();
//         std::cout << "All dofs: " << graph.all_dof() << std::endl;
        std::cout << "Bar " << ++i <<" of " << bars.size() << std::endl;

        int u = bar.firstVertex;
        int v = bar.secondVertex;
        float c = bar.capacity;

        vector<int> blocked_vertices;
        with_collapse.try_to_find_pebbles(v, 6.0f, blocked_vertices);
        without_collapse.try_to_find_pebbles(v, 6.0f, blocked_vertices);

        blocked_vertices.push_back(v);
        vector<int> visited_vertices_collapse;
        float collected_pebbles_collapse;
        tie(visited_vertices_collapse, collected_pebbles_collapse) = with_collapse.try_to_find_pebbles(u, c, blocked_vertices);

        vector<int> visited_vertices;
        float collected_pebbles;
        tie(visited_vertices, collected_pebbles) = without_collapse.try_to_find_pebbles(u, c, blocked_vertices);

        with_collapse.set_bar(u, v, collected_pebbles_collapse);
        without_collapse.set_bar(u, v, collected_pebbles);

        if(collected_pebbles_collapse < c) {
            with_collapse.collapse_vertices(visited_vertices_collapse);
        }
        assert(old_dof_collapse == with_collapse.all_dof());
        assert(old_dof == without_collapse.all_dof());
        assert(old_dof == old_dof_collapse);
        std::cout << with_collapse.average_dof() << " " << without_collapse.average_dof() << std::endl;
        assert(with_collapse.average_dof() == without_collapse.average_dof());
    }
}
