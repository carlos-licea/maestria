#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Route.hpp"

#include <vector>
#include <tuple>
#include <ostream>

#include "Eigen/SparseCore"

class GraphTester;

class Graph {
    friend std::ostream& operator<<(std::ostream& o, Graph& g);
    friend GraphTester;

public:
    Graph(std::size_t size);

    void set_bar(const int from, const int to, const float pebbles);

    //rigid,vertices, total_pebbles_found
    std::tuple<std::vector<int>, float> try_to_find_pebbles(int startingEdge, float required_pebbles, const std::vector<int>& blocked_vertices);
    std::tuple< std::vector< int >, float > collect_pebbles(int targetEdge, float required_pebbles, const std::vector<int>& blocked_vertices);


    void collapse_vertices(const std::vector<int>& blocked_vertices);

    float average_dof();
    float all_dof();
    float available_dof();

private:
    float back_track(float pebbles, int startingVertex, const Route& route);

    inline float edge(int from, int to);
    inline float vertex(int vertex);
    inline float& edgeRef(int from, int to);
    inline float& vertexRef(int vertex);

    const std::size_t m_size;
//     std::vector<float> m_matrix;
    Eigen::SparseMatrix<float> m_matrix;
};

std::ostream& operator<< (std::ostream& o, Graph& g);

#endif //GRAPH_HPP
