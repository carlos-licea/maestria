#ifndef VPG_H
#define VPG_H

#include <vector>

#include "bars.hpp"
#include "Graph.hpp"

void virtual_pebble_game(const std::vector<bar>& bars, Graph& graph, bool collapse = true);

#endif //VPG_H
