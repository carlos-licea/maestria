#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    int length;
    float q_min;
    float q_fluct;

    auto file_exists = [](const std::string & fileName) {
        std::ifstream file(fileName);
        return file.good();
    };

    const std::string description_file = "lattice-description";
    if(argc == 4) {
        length = atoi(argv[1]);
        q_min = atof(argv[2]);
        q_fluct = atof(argv[3]);

        std::cout << "Generating description";
        std::ofstream outfile(description_file);
        outfile << length << " " << q_min << " " << q_fluct;
    } else if( file_exists(description_file) ) {
        std::ifstream infile(description_file);
        infile >> length >> q_min >> q_fluct;
    } else {
        std::cout << "Not a description file present nor correct number of arguments given.";
        exit(1);
    }
// std::cout << length << " " << q_min << " " << q_fluct;
    if( length <= 0 ) {
        std::cout << "Length must be greater than 0";
        exit(1);
    }
    if(q_min < 0 || q_fluct < 0) {
        std::cout << "q_min and q_fluct must be 0 or greater";
        exit(1);
    }
    if((q_min + q_fluct) > 1.0) {
        std::cout << "q_min and q_fluct must sum at most 1.0";
        exit(1);
    }

    const std::string seed_file = "lattice-seed";
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
    };
    std::vector<edge> fixedEdges;
    std::vector<edge> fluctEdges;

    auto evaluate_edge = [&](int fromNode, int toNode) {
        auto random_p = []() {
            return (float)rand() / (float)RAND_MAX;
        };

        const float p = random_p();
        if(p < q_min) {
            fixedEdges.push_back({fromNode, toNode});
        }
        else if(p < (q_min + q_fluct)) {
            fluctEdges.push_back({fromNode, toNode});
        }
    };

    const int l = length;
    for(int z = 1; z <= length; z++) {
        for(int x = 1; x <= length; x++) {
            for(int y = 1; y <= length; y++) {
                const int fromNode = ((x-1)*l) + (z-1)*(l*l) + y;
                int toNode;

                //hacia abajo
                if(y<l) {
                    toNode = ((x-1)*l) + (y+1) +(z-1)*(l*l);
                }
                else {
                    toNode = ((x-1)*l) + 1 +(z-1)*(l*l);
                }
                evaluate_edge(fromNode-1, toNode-1);

                //hacia la derecha  
                if(x<l) {
                    toNode = ((x-1)*l) + (z-1)*(l*l) + (y+l);
                }
                else {
                    toNode = ((x-1)*l) + (z-1)*(l*l) + (y-l*(l-1));
                } 
                evaluate_edge(fromNode-1, toNode-1);

                //hacia adentro
                if(z<l) {
                    toNode =((x-1)*l) + (z-1)*(l*l) + (y+l*l);
                }
                else {
                    toNode =((x-1)*l) + (z-1)*(l*l) + y-(l*l)*(l-1);
                }
                evaluate_edge(fromNode-1, toNode-1);
            } // y
        } // x
    } // z

    std::ofstream outfile("lattice", std::ofstream::out);
    auto write_edges = [&](const std::vector<edge>& edges){
        outfile << edges.size() << std::endl;
        for(auto e : edges) {
            outfile << e.firstVertex << " " << e.secondVertex << std::endl;
        }
    };
    write_edges(fixedEdges);
    write_edges(fluctEdges);
}
