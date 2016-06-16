#include "vpg.hpp"
#include "bars.hpp"

#include <iostream>
#include <string>

int main() {
    auto bars = read_bars("realization");
    const int vertex_count = read_vertex_count("../lattice-description");

//     std::vector<bar> bars = {
//         { 1, 0, 2.5 },
//         { 1, 2, 5.0 },
//         { 0, 2, 2.0 },
//         { 1, 2, 1.5 }
//     };

    Graph g(vertex_count);

    virtual_pebble_game(bars, g);

    std::cout << g.average_dof() << std::endl;
//     std::cout << g;

    return 0;
}
