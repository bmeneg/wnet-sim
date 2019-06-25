#pragma once

#include <iostream>
#include <boost/graph/adjacency_list.hpp>

struct Vertex {
    int id;
    bool selected = false;
};

struct Edge {
    unsigned long cost;
    bool selected = false;
};

/* There is no need to use boost::listS since we're not inserting nor removing
 * nodes, thus the performance gain is useless. Because of that we've choosen
 * boost::vectS which provides better space optimization. */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
        Vertex, Edge> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_desc_t;

class NetworkGraph
{
private:
    graph_t graph;

public:
    void add_routes_from_file(std::string);
};
