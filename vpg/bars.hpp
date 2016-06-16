#ifndef BARS_H
#define BARS_H

#include <string>
#include <vector>
#include <fstream>

struct bar {
    int firstVertex;
    int secondVertex;
    float capacity;
};

std::vector<bar> read_bars(const std::string& filename);

int read_vertex_count(const std::string& filename);

#endif //BARS_H