#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    float p;
    float cost;
    std::string type;
    int length;

    auto file_exists = [](const std::string & fileName) {
        std::ifstream file(fileName);
        return file.good();
    };

    const std::string realization_description_file = "realization-description";
    if( file_exists(realization_description_file) ) {
        std::ifstream infile(realization_description_file);
        infile >> p >> cost >> type;
    } else if(argc == 4) {
        p = atof(argv[1]);
        cost = atof(argv[2]);
        type = argv[3];

        std::cout << "Generating realization description";
        std::ofstream outfile(realization_description_file);
        outfile << p << " " << cost << " " << type;
    }
    else {
        std::cout << "Not a lattice description file present.";
        exit(1);
    }

    if(p < 0 || p > 1) {
        std::cout << "P has to be between 0 and 1";
        exit(1);
    }

    if( cost < 0 ) {
        std::cout << "Cost has to be greater than 0";
        exit(1);
    }

    if(type != "pg" && type != "vpg") {
        std::cout << "Type has to be either pg or vpg";
        exit(1);
    }

    const std::string seed_file = "realization-seed";
    int seed;
    if(file_exists(seed_file)) {
        std::ifstream infile(seed_file);
        infile >> seed;
    }
    else {
        std::cout << "No seed found, generating one.";
        seed = std::time(nullptr);
        std::ofstream outfile(seed_file);
        outfile << seed;
    }

    // Sanitized inputs, now lets generate the bonds
    struct edge {
        int firstVertex;
        int secondVertex;
        float cost;
    };
    std::vector<edge> presentEdges;

    std::vector<edge> fluctEdges;

    std::ifstream infile("lattice", std::ofstream::in);
    auto read_edges = [&](){
        int fixedEdgesCount;
        infile >> fixedEdgesCount;
        for(int i = 0; i < fixedEdgesCount; ++i) {
            int firstVertex;
            int secondVertex;
            infile >> firstVertex >> secondVertex;
            presentEdges.push_back({firstVertex, secondVertex, cost});
        }

        float edge_cost = cost;
        if(type == "vpg") {
            edge_cost = cost * p;
        }

        int fluctEdgesCount;
        infile >> fluctEdgesCount;
        for(int i = 0; i < fluctEdgesCount; ++i) {
            int firstVertex;
            int secondVertex;
            infile >> firstVertex >> secondVertex;
            fluctEdges.push_back({firstVertex, secondVertex, edge_cost});
        }
    };
    read_edges();

    auto evaluate_edge = [&](edge e) {
        auto random_p = []() {
            return (float) rand() / (float) RAND_MAX;
        };

        const float p_rand = random_p();
        if(p_rand < p) {
            presentEdges.push_back(e);
        }
    };

    if(type == "pg") {
        for(const auto& e : fluctEdges) {
            evaluate_edge(e);
        }
    }
    else if(type == "vpg") {
        presentEdges.insert(presentEdges.end(), fluctEdges.begin(), fluctEdges.end());
    }

    std::ofstream outfile("realization", std::ofstream::out);
    auto write_edges = [&](const std::vector<edge>& edges){
        outfile << edges.size() << std::endl;
        for(auto e : edges) {
            outfile << e.firstVertex << " " << e.secondVertex << " " << e.cost << std::endl;
        }
    };
    write_edges(presentEdges);
}

struct edge {
    int firstVertex;
    int secondVertex;
    float cost;
};

struct options {
    float p;
    std::string type;
};
