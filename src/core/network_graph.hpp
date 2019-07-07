#pragma once

#include <iostream>
#include <boost/graph/adjacency_list.hpp>

struct Vertex;
struct Edge;

/* There is no need to use boost::listS since we're not inserting nor removing
 * nodes, thus the performance gain is useless. Because of that we've choosen
 * boost::vectS which provides better space optimization. */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
	Vertex, Edge> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_desc_t;
typedef boost::graph_traits<graph_t>::out_edge_iterator edge_it_t;

struct Vertex {
	int id;
	vertex_desc_t prev;
	bool selected = false;
};

struct Edge {
	unsigned long cost;
	bool selected = false;
};

class NetworkGraph
{
private:
	graph_t graph;

public:
	void add_routes_from_file(std::string);
	void find_shortest_path();
	void keep_running();
};
