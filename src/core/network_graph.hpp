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
typedef std::pair<unsigned int, std::vector<unsigned int>> route_t;
typedef std::pair<unsigned int, std::pair<unsigned int, unsigned int>> edge_t;

struct Vertex {
	unsigned int id;
	vertex_desc_t prev = 0;
	std::vector<route_t> routing_table{};
};

struct Edge {
	unsigned int cost;
};

class NetworkGraph
{
public:
	void graph_file(std::string);
	std::string graph_file(void) const;
	void sqr_weight(unsigned int);
	unsigned int sqr_weight(void) const;

	void add_routes_from_file(void);
	void add_routes_random(void);
	int calc_routing_graph(void);
	route_t find_shortest_path(unsigned int, unsigned int);
	std::vector<edge_t> graph_edges(void) const;
	void clear_graph(void);

private:
	graph_t _graph;
	// input filename for a graph already generated
	std::string _graph_filename;
	// how much a sqr represents as graph weight
	unsigned int _sqr_weight = 10;

	// node id <-> graph vertex ref look up table
	std::vector<std::pair<unsigned int, vertex_desc_t>> _vertex_lut;
};
