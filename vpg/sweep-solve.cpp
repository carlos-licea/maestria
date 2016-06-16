#include "Graph.hpp"
#include "vpg.hpp"

#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <tuple>
#include <fstream>
#include <chrono>
#include <iostream>

struct lattice_options {
    int length;
    float q_min;
    float q_fluct;
    float capacity;
};

std::tuple<std::vector<bar>, std::vector<bar>> generate_lattice(const lattice_options& options) {
    std::vector<bar> fixedEdges;
    std::vector<bar> fluctEdges;

    auto evaluate_bar = [&](int fromNode, int toNode) {
        auto random_p = []() {
            return (float)rand() / (float)RAND_MAX;
        };

        const float p = random_p();
        if(p < options.q_min) {
            fixedEdges.push_back({fromNode, toNode, options.capacity});
        }
        else if(p < (options.q_min + options.q_fluct)) {
            fluctEdges.push_back({fromNode, toNode, options.capacity});
        }
    };

    const int l = options.length;
    for(int z = 1; z <= l; z++) {
        for(int x = 1; x <= l; x++) {
            for(int y = 1; y <= l; y++) {
                const int fromNode = ((x-1)*l) + (z-1)*(l*l) + y;
                int toNode;

                //hacia abajo
                if(y<l) {
                    toNode = ((x-1)*l) + (y+1) +(z-1)*(l*l);
                }
                else {
                    toNode = ((x-1)*l) + 1 +(z-1)*(l*l);
                }
                evaluate_bar(fromNode-1, toNode-1);

                //hacia la derecha  
                if(x<l) {
                    toNode = ((x-1)*l) + (z-1)*(l*l) + (y+l);
                }
                else {
                    toNode = ((x-1)*l) + (z-1)*(l*l) + (y-l*(l-1));
                } 
                evaluate_bar(fromNode-1, toNode-1);

                //hacia adentro
                if(z<l) {
                    toNode =((x-1)*l) + (z-1)*(l*l) + (y+l*l);
                }
                else {
                    toNode =((x-1)*l) + (z-1)*(l*l) + y-(l*l)*(l-1);
                }
                evaluate_bar(fromNode-1, toNode-1);
            } // y
        } // x
    } // z
    return std::make_tuple( fixedEdges, fluctEdges );
}

struct realization_options {
    float p;
    std::string type;
};

std::vector<bar> generate_realization(const realization_options& op, const std::vector<bar>& fixed, const std::vector<bar>& fluct) {
    std::vector<bar> present;
    present.insert(present.cend(), fixed.cbegin(), fixed.cend());

    auto evaluate_bar_pg = [&](bar e) {
        auto random_p = []() {
            return (float) rand() / (float) RAND_MAX;
        };

        const float p_rand = random_p();
        if(p_rand < op.p) {
            present.push_back(e);
        }
    };

    auto evaluate_bar_vpg = [&](bar e) {
        present.push_back({ e.firstVertex, e.secondVertex, op.p * e.capacity });
    };

    for(const auto& e : fluct) {
        if( op.type == "vpg" ) {
            evaluate_bar_vpg(e);
        }
        else {
            evaluate_bar_pg(e);
        }
    }
    return present;
}

int main() {
    int seed = std::time(nullptr);

    std::vector<int> lengths = {5, 10, 20/*, 50*/};

    const float q_min = 0.3;
    const float q_fluct = 0.3;
    const float cost = 5.0;

    std::ofstream metafile("meta", std::ofstream::out);
    metafile << q_min << " " << q_fluct << " " << seed << std::flush;

    const float p_inc = 0.01;
    std::ofstream outfile("result", std::ofstream::out);

    for( const auto& l : lengths ) {
        std::vector<bar> fixedEdges;
        std::vector<bar> fluctEdges;
        std::tie(fixedEdges, fluctEdges) = generate_lattice({l, q_min, q_fluct, cost});

        for(int i = 0; i <= 100; ++i) {
            float p = (float)i / 100.0;
            std::cout << "In " << l << ", " << p << "\n";

            typedef std::chrono::high_resolution_clock Time;
            typedef std::chrono::milliseconds ms;

            auto realization_pg = generate_realization({p, "pg"}, fixedEdges, fluctEdges);
//             for(const auto& e : realization_pg )
//                 std::cout << e.firstVertex << "," << e.secondVertex << ":" << e.capacity << ";";

            Graph g_pg(l*l*l);
            auto start = Time::now();

            virtual_pebble_game(realization_pg, g_pg);
            auto end = Time::now();
            auto duration = std::chrono::duration_cast<ms>(end - start).count();
            outfile << l << "," << p << ","<< g_pg.average_dof() << "," << duration << ",pg" << "\n" << std::flush;

            auto realization_vpg = generate_realization({p, "vpg"}, fixedEdges, fluctEdges);
//             std::cout << "\nvpg\n";
//             for(const auto& e : realization_vpg )
//                 std::cout << e.firstVertex << "," << e.secondVertex << ":" << e.capacity << ";";
            Graph g_vpg(l*l*l);
            start = Time::now();
            virtual_pebble_game(realization_vpg, g_vpg);
            end = Time::now();
            duration = std::chrono::duration_cast<ms>(end - start).count();
            outfile << l << "," << p << ","<< g_vpg.average_dof() << "," << duration << ",vpg" << "\n" << std::flush;
        }
    }
}