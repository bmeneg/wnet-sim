#pragma once

#include <iostream>

#include <boost/graph/adjacency_list.hpp>

#include "protocol.hpp"

struct ProtoMsg;
struct Vertex;
struct Edge;

/* There is no need to use boost::listS since we're not inserting nor removing
 * nodes, thus the performance gain is useless. Because of that we've choosen
 * boost::vectS which provides better space optimization. */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
	Vertex, Edge> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_desc_t;

/* Route vector contains [<total cost>, <vector of vertices>] */
typedef std::pair<Edge, std::vector<Vertex *>> route_t;
typedef std::pair<Edge, std::pair<Vertex *, Vertex *>> edge_t;

struct Vertex {
	unsigned int id;
	struct coord {
		unsigned int x;
		unsigned int y;
	} coord = {};

	vertex_desc_t self = 0;
	vertex_desc_t prev = 0;
	std::vector<route_t> routing_table{};
	std::vector<ProtoMsg> msg_table{};
};

struct Edge {
	unsigned int cost;
};

class NetworkGraph
{
public:
	NetworkGraph(void);
	void graph_file(std::string);
	std::string graph_file(void) const;
	void sqr_weight(unsigned int);
	unsigned int grid_dim(void) const;
	unsigned int sqr_weight(void) const;

	void add_routes_from_file(void);
	void add_routes_random(void);
	int calc_routing_graph(void);
	route_t find_shortest_path(unsigned int, unsigned int);
	std::vector<edge_t> graph_edges(void) const;
	void clear_graph(void);
	Vertex * vertex(unsigned int);

private:
	void _calc_adjacent_list(std::vector<unsigned int>);

	graph_t _graph;
	// input filename for a graph already generated
	std::string _graph_filename;

	// how much a sqr represents as graph weight
	unsigned int _sqr_weight = 10;
	unsigned int _grid_dim = 5;
	// adjacent list to handle random nodes generation
	std::vector<std::pair<unsigned int,
			std::pair<unsigned int, unsigned int>>> _adj_list;
	// grid is a collection of "pixels" which might store a vertex or not:
	// in case a vertex is there present, it'll be added to busy_pixel vector,
	// easing the traverse when calculating the euclidean distance.
	std::vector<Vertex *> _grid;

	// vertex id <-> graph vertex ref lookup table
	std::vector<std::pair<unsigned int, Vertex *>> _vertex_lut;
	// node (vertex) id <-> node ref lookup table
};
