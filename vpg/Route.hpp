#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <map>
#include <ostream>

class Route {
    friend std::ostream& operator<<(std::ostream& o, const Route& r);

public:
    Route(int startingVertex);
    Route( std::initializer_list<int> simpleRoute );
    ~Route();

    void addStep(int from, int to);

    int arrivedFrom(int vertex) const;

    bool isEmpty() const;
    int firstVertex() const;
    int size() const;

private:
    //vertex, arrivedFrom
    std::map<int, int> m_map;
};

class RouteIteration {
    
};

class RouteIterator {
    
};

std::ostream& operator<< (std::ostream& o, const Route& r);

#endif