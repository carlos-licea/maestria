#include "Graph.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

using std::tie;
using std::min;
using std::tuple;
using std::vector;


Graph::Graph(std::size_t size)
    : m_size(size)
    , m_matrix(m_size, m_size)
{
    for(int i = 0; i < m_size; ++i) {
        vertexRef(i) = 6.0f;
    }
}

void Graph::set_bar(const int from, const int to, const float pebbles) {
//     assert(from >= 0 && from < m_size && to >= 0 && to < m_size);
//     assert(pebbles <= vertex(from));

    edgeRef(from, to) += pebbles;
    vertexRef(from) -= pebbles;
}

tuple< vector< int >, float > Graph::try_to_find_pebbles(int targetEdge, float required_pebbles, const vector< int >& blocked_vertices) {
    float total_pebbles_found = 0;
    vector<int> visited_vertices;
    vector<int> old_visited_vertices;

    // We need to run it several times because we might have visited vertices that
    // have already covered the whole cost of the edge and hence no longer part of
    // the rigid section but if we collapse them we would be considering them as
    // part of it so we run through the section over and over until we are sure it no longer covers
    // flexible vertices. The last run is actually all rigid vertices.
    do {
        old_visited_vertices = visited_vertices;
        std::tie(visited_vertices, total_pebbles_found) = collect_pebbles(targetEdge, required_pebbles, blocked_vertices);
    } while(total_pebbles_found < required_pebbles && visited_vertices != old_visited_vertices);

    return std::make_tuple(visited_vertices, total_pebbles_found);
}

tuple< vector< int >, float > Graph::collect_pebbles(int targetEdge, float required_pebbles, const vector< int >& blocked_vertices) {
    vector<int> visited_vertices;
    visited_vertices.insert(visited_vertices.end(), blocked_vertices.begin(), blocked_vertices.end() );

    vector<int> vertex_queue;
    vertex_queue.push_back(targetEdge);

    Route route { vertex_queue.front() };

    bool full = false;
    float total_pebbles_found = 0.0;

    while(!vertex_queue.empty() && !full) {
        int w = vertex_queue.back();
        vertex_queue.pop_back();

        visited_vertices.push_back(w);

        const float missing_pebbles = required_pebbles - total_pebbles_found;
        const float pebbles_to_bakctrack = min(missing_pebbles, vertex(w));
        float pebbles_found = vertex(w);
        bool limiting_edge_found = false;

        if( pebbles_found > 0 ) {
            const float pebbles_back_tracked = back_track(pebbles_to_bakctrack, w, route);
            if(pebbles_back_tracked < pebbles_to_bakctrack) {
                // We can only backtrack as many pebbles as the bound requires
                // Therefore some pebbles might just be unaccesible and if that's
                // the case it makes no sense to keep searching on further regions
                // of the network as there's no way to bring more pebbles back
                limiting_edge_found = true;
            }
            pebbles_found = pebbles_back_tracked;
        }
        total_pebbles_found += pebbles_found;
        if(total_pebbles_found >= required_pebbles) {
            full = true;
        }
        else {
            if(!limiting_edge_found) {
                // Add other candidates
                for(int i = 0; i < m_size; ++i) {
                    auto not_in_visited = [&]() { return  std::find(visited_vertices.cbegin(), visited_vertices.cend(), i) == visited_vertices.cend(); };
                    if( edge(w,i) > 0 && not_in_visited() ) {
                        vertex_queue.push_back(i);
                        route.addStep(i, w);
                    }
                }
            }
        }
    }

    return std::make_tuple(visited_vertices, total_pebbles_found);
}

float Graph::back_track(float pebbles, int startingVertex, const Route& route) {
    if(route.isEmpty()) {
        return 0;
    }

    int currentVertex = startingVertex;
    int previousVertex = route.arrivedFrom(currentVertex);

    while(previousVertex != -1) {
        // If it wants more pebbles than the bond costs, we can only cover the
        // cost of the edge and nothing more
        pebbles = min(pebbles, edge(previousVertex, currentVertex) );

        edgeRef(previousVertex, currentVertex) -= pebbles;
        vertexRef(previousVertex) += pebbles;

        vertexRef(currentVertex) -= pebbles;
        edgeRef(currentVertex, previousVertex) += pebbles;

        currentVertex = previousVertex;
        previousVertex = route.arrivedFrom(currentVertex);
    }

    return pebbles;
}

void print_vertices(const vector<int>& vertices) {
    for( const auto& i : vertices ) {
        std::cout << i << ",";
    }
}

void Graph::collapse_vertices(const vector<int>& vertices) {

    //const float old_average_dof = average_dof();

    if(vertices.size() == 1) {
        return;
    }

    const int root = *std::min_element(std::begin(vertices), std::end(vertices));

    // take care of the root first
    vertexRef(root) = 6.0f;

    auto in_visited = [&](const int i) { return (std::find(vertices.cbegin(), vertices.cend(), i) != vertices.cend()); };
    // incomming edges. We only care about incoming.
    for(int i = 0; i < m_size; ++i) {
        if( i == root )
            continue;

        if( edge(i, root) > 0.0f ) { //there's an edge
            if( in_visited(i) ) {
                //and it's comming from inside the rigid cluster
                edgeRef(i, root) = 0.0f;
            }
        }
    }

    // take care of the rest
    for(const int& currentVertex : vertices ) {
       if(currentVertex == root) {
            continue;
        }

        vertexRef(currentVertex) = 0.0f;

        // Incoming edges. We only need to take care of incoming as the vertices
        // all point to each other only, i.e., there's no outgoing vertices that
        // will not be visited
        for(int i = 0; i < m_size; ++i) {
            if( i == currentVertex )
                continue;

            if( edge(i, currentVertex) > 0.0f ) { //there's an edge
                if( !in_visited(i) ) {
                    //and it's comming from outside the rigid cluster
                    //forward it to the root
                    const float edgeCost = edge(i, currentVertex);
                    edgeRef(i, currentVertex) -= edgeCost;
                    edgeRef(i, root) += edgeCost;
                }
                else {
                        edgeRef(i, currentVertex) = 0.0f;
                }
            }
        }

        edgeRef(currentVertex, root) = 6.0f;
    }

    //assert(old_average_dof == average_dof());
}

float Graph::edge(int from, int to)
{
    return m_matrix.coeff(from, to);
}

float Graph::vertex(int vertex)
{
    return edge(vertex, vertex);
}

float& Graph::edgeRef(int from, int to)
{
    return m_matrix.coeffRef(from, to);
}

float& Graph::vertexRef(int vertex)
{
    return edgeRef(vertex, vertex);
}

float Graph::average_dof()
{
    return available_dof() / (float) m_size;
}

float Graph::all_dof()
{
    float sum = 0.0f;
    for(int i = 0; i < m_size; ++i) {
        for(int j = 0; j < m_size; ++j) {
            sum += edge(i,j); // yes we also include available dof, ALL dofs
        }
    }
    return sum;
}

float Graph::available_dof()
{
    float sum = 0.0f;
    for(int i = 0; i < m_size; ++i) {
        sum += vertex(i);
    }

    return sum;
}

std::ostream& operator<<(std::ostream& o, Graph& g) {
    o << "Size: " << g.m_size << std::endl;

    o << "Vertex: [";
    for(int i = 0; i < g.m_size; ++i) {
        o << g.vertex(i) << ",";
    }
    o << "]" << std::endl;

    o << "Edges: [" << std::endl;
    for(int i = 0; i < g.m_size; ++i) {
        for(int j = 0; j < g.m_size; ++j) {
            o << g.edge(i, j) << ",";
        }
        o << std::endl;
    }
    o << "]" << std::endl;
}
