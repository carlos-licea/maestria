#include "bars.hpp"
#include <iostream>

std::vector<bar> read_bars(const std::string& filename) {
    std::vector<bar> bars;

    std::ifstream infile(filename, std::ofstream::in);

    int barsCount;
    infile >> barsCount;
    for(int i = 0; i < barsCount; ++i) {
        int firstVertex;
        int secondVertex;
        float cost;
        infile >> firstVertex >> secondVertex >> cost;
        bars.push_back({firstVertex, secondVertex, cost});
    }

    return bars;
}

int read_vertex_count(const std::string& filename) {
    int length = 0;

    auto file_exists = [](const std::string & fileName) {
        std::ifstream file(fileName);
        return file.good();
    };

    if(!file_exists(filename)) {
        std::cout << "No file present";
    }

    std::ifstream infile(filename);
    infile >> length;

    return length * length * length;
}