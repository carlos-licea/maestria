#include "Route.hpp"

Route::Route(int startingVertex)
{
    m_map.insert( {-1, startingVertex} );
}

Route::Route(std::initializer_list< int > simpleRoute)
{
    int previousVertex = -1;
    for( const auto& v : simpleRoute ) {
        addStep( v, previousVertex );
        previousVertex = v;
    }
}

Route::~Route()
{
}

void Route::addStep(int from, int to)
{
    m_map.insert( {from, to} );
}

int Route::arrivedFrom(int vertex) const
{
    return m_map.at(vertex);
}

bool Route::isEmpty() const
{
    return m_map.empty();
}

int Route::firstVertex() const
{
    return m_map.at(-1);
}

int Route::size() const
{
    return m_map.size();
}

std::ostream& operator<< (std::ostream& o, const Route& r) {
    for(auto& kv : r.m_map) {
        o << kv.first << " " << kv.second << std::endl;
    }
}
