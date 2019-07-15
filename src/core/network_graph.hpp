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

/* Route vector contains [<total cost>, <vector of node ids>] */
typedef std::pair<int, std::vector<unsigned long>> route_t;
typedef std::pair<unsigned long, std::pair<unsigned long, unsigned long>> edge_t;

struct Vertex {
	unsigned long id;
	vertex_desc_t prev = 0;
	std::vector<route_t> routing_table{};
};

struct Edge {
	unsigned long cost;
};

class NetworkGraph
{
public:
	void graph_file(std::string);
	std::string graph_file(void);

	void add_routes_from_file(std::string);
	void calc_routing_graph(void);
	route_t find_shortest_path(unsigned long src, unsigned long dest);
	std::vector<edge_t> graph_edges(void) const;

private:
	graph_t _graph;
	std::string _graph_filename;

	/* node id <-> graph vertex ref look up table */
	std::vector<std::pair<unsigned long, vertex_desc_t>> _vertex_lut;
};
