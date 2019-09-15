#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>

#include "network_graph.hpp"

using namespace boost;

void NetworkGraph::graph_file(std::string filename)
{
	_graph_filename = filename;
}

std::string NetworkGraph::graph_file(void) const
{
	return _graph_filename;
}

void NetworkGraph::sqr_weight(unsigned int weight)
{
	_sqr_weight = weight;
}

unsigned int NetworkGraph::sqr_weight(void) const
{
	return _sqr_weight;
}

void NetworkGraph::add_routes_from_file()
{
	if (_graph_filename.empty()) {
		std::cout << "_graph_filename is empty!" << std::endl;
		exit(1);
	}

	std::ifstream routes_file(_graph_filename);
	std::string line;
	std::vector<std::string> tokens;
	std::vector<unsigned int> ve_attr;
	vertex_desc_t vertex_0 = 0, vertex_1 = 0;
	bool vertex_0_found, vertex_1_found;

	while (std::getline(routes_file, line)) {
		if (line == "")
			continue;

		// zerofy control structs
		vertex_0_found = vertex_1_found = false;
		ve_attr.clear();

		// retrieve data from graph file, which follows per line:
		// <vertex_0> <vertex_1> <cost>
		// and covert to unsigned int
		split(tokens, line, is_any_of(" "));
		for (auto it = tokens.begin(); it != tokens.end(); it++)
			ve_attr.push_back(static_cast<unsigned int>(std::stoul(*it)));

		auto vpair = vertices(_graph);
		for (auto it = vpair.first; it != vpair.second; it++) {
			if (!vertex_0_found && _graph[*it].id == ve_attr.at(0)) {
				vertex_0 = *it;
				vertex_0_found = true;
			} else if (!vertex_1_found && _graph[*it].id == ve_attr.at(1)) {
				vertex_1 = *it;
				vertex_1_found = true;
			}
		}

		// in case vertex doesn't already exists, create new one and add
		// it to LUT
		if (!vertex_0_found) {
			vertex_0 = add_vertex({ve_attr.at(0)}, _graph);
			_vertex_lut.push_back(std::make_pair(ve_attr.at(0), vertex_0));
		}
		if (!vertex_1_found) {
			vertex_1 = add_vertex({ve_attr.at(1)}, _graph);
			_vertex_lut.push_back(std::make_pair(ve_attr.at(1), vertex_1));
		}

		add_edge(vertex_0, vertex_1, {ve_attr.at(2)}, _graph);
	}

	// sort vertex LUT to allow binary search
	std::sort(_vertex_lut.begin(), _vertex_lut.end());

#ifdef DEBUG
	std::cout << "vertices found: ";
	print_vertices(_graph, get(&Vertex::id, _graph));
	auto vpair = vertices(_graph);
	for (auto v_it = vpair.first; v_it != vpair.second; v_it++) {
		std::cout << "edges incident to vertex " << _graph[*v_it].id <<
			" are: " << std::endl;
		auto epair = out_edges(*v_it, _graph);
		for (auto e_it = epair.first; e_it != epair.second; e_it++) {
			std::cout << "(" << _graph[source(*e_it, _graph)].id <<
				"," << _graph[target(*e_it, _graph)].id <<
				")" << " = " << _graph[*e_it].cost << std::endl;
		}
	}

	std::cout << std::endl;
	print_graph(_graph, get(&Vertex::id, _graph));
	std::cout << std::endl;
#endif
}

struct vertex_coord {
	unsigned int id;
	unsigned int x;
	unsigned int y;
};

void NetworkGraph::add_routes_random()
{
	const unsigned int sqr = _sqr_weight;

	// grid is a collection of "pixels" which might store a vertex or not:
	// in case a vertex is there present, it'll be added to busy_pixel vector,
	// easing the traverse when calculating the euclidean distance.
	std::vector<struct vertex_coord *> grid(sqr * sqr);
	std::vector<unsigned int> busy_pixel;

	std::vector<std::pair<unsigned int,
			std::pair<unsigned int, unsigned int>>> adj_list;
	unsigned int id_count = 0;

	// the distance between pixel will be considered 1du (distance units)
	const unsigned int dist_pixel = 1;

	// maximum field of view (range) for which we consider when a vertex can
	// actually "see" other vertex.
	const unsigned int max_fov = 4 * dist_pixel;

	// place the resulting vector to a temp file to be processed afterwards
	std::string tmp_filename(".wn_tmp");
	std::ofstream out_file(tmp_filename);

	// make random seems a true rng
	srand(static_cast<unsigned int>(time(nullptr)));

	// populate the grid with a certain propability
	// TODO: drop hardcoded probability and allow user selection
	for (unsigned int y = 0; y < sqr; y++) {
		for (unsigned int x = 0; x < sqr; x++) {
			unsigned int pixel = x + y * sqr;
			if ((rand() % 12) < 1) {
				auto *vertex = new struct
					vertex_coord({id_count++, x , y});
				grid.at(pixel) = vertex;
				busy_pixel.push_back(pixel);
			}
		}
	}

#ifdef DEBUG
	std::cout << "random> grid created with " << busy_pixel.size() <<
				 " vertices" << std::endl;
#endif

	// traverse the vector of pixels calculating the euclidean distance
	// between every vertex to determine what which can actually see
	for (unsigned int i = 0; i < busy_pixel.size(); i++) {
		unsigned int src_pixel = busy_pixel.at(i);
		struct vertex_coord src_vertex = *grid.at(src_pixel);
		for (unsigned int j = i + 1; j < busy_pixel.size(); j++) {
			unsigned int dst_pixel = busy_pixel.at(j);
			struct vertex_coord dst_vertex = *grid.at(dst_pixel);

			unsigned int dx = dist_pixel * (src_vertex.x -
							dst_vertex.x);
			unsigned int dy = dist_pixel * (src_vertex.y -
							dst_vertex.y);
			unsigned int euc_dist = static_cast<unsigned int>(
										std::sqrt(dx * dx + dy * dy));
			if (euc_dist < max_fov)
				adj_list.push_back(std::make_pair(euc_dist,
								  std::make_pair(src_vertex.id,
										 dst_vertex.id)));
		}
	}

#ifdef DEBUG
	std::cout << "random> distance vector defined" << std::endl;
#endif

	for (auto item : adj_list) {
		out_file << item.second.first << " " << item.second.second << " "
			<< item. first << std::endl;
	}
	out_file.close();
	_graph_filename = tmp_filename;

#ifdef DEBUG
	for (auto item : adj_list) {
		std::cout << item.second.first << " " << item.second.second << " "
			<< item. first << std::endl;
	}
	std::cout << "filename: " << tmp_filename << std::endl;
#endif
}

int NetworkGraph::calc_routing_graph()
{
#ifdef PRE_PROCESSING
	std::vector<unsigned int> distance(num_vertices(_graph));
	vertex_desc_t curr_vertex_desc = 0;

	// run the whole graph collecting each distance for all vertex
	for (auto src_vpair : _vertex_lut) {
		dijkstra_shortest_paths(_graph, src_vpair.second,
			predecessor_map(get(&Vertex::prev, _graph))
			.distance_map(make_iterator_property_map(distance.begin(),
				get(vertex_index, _graph)))
			.weight_map(get(&Edge::cost, _graph)));

		for (auto dst_vpair : _vertex_lut) {
			route_t route;
			route.first = distance[dst_vpair.second];
			curr_vertex_desc = dst_vpair.second;
			while (curr_vertex_desc != src_vpair.second) {
				// if the following condition is true and the while didn't
				// finish, it means the vertices are not in the same graph
				if (curr_vertex_desc == _graph[curr_vertex_desc].prev)
					return -1;

				route.second.push_back(_graph[curr_vertex_desc].id);
				curr_vertex_desc = _graph[curr_vertex_desc].prev;
			}
			_graph[src_vpair.second].routing_table.push_back(route);
		}
	}
#endif // PRE_PROCESSING
	return 0;
}

route_t NetworkGraph::find_shortest_path(unsigned int src, unsigned int dst)
{
#ifdef PRE_PROCESSING
	return _graph[_vertex_lut[src].second].routing_table[dst];
#else
	std::vector<unsigned int> distance(num_vertices(_graph));
	vertex_desc_t curr_vertex_desc = 0;
	route_t route;

	// run the whole graph collecting each distance for all vertex
	dijkstra_shortest_paths(_graph, _vertex_lut[src].second,
		predecessor_map(get(&Vertex::prev, _graph))
		.distance_map(make_iterator_property_map(distance.begin(),
			get(vertex_index, _graph)))
		.weight_map(get(&Edge::cost, _graph)));

	route.first = distance[_vertex_lut[dst].second];
	curr_vertex_desc = _vertex_lut[dst].second;
	while (curr_vertex_desc != _vertex_lut[src].second) {
		// if the following condition is true and the while didn't finish,
		// it means the vertices are not in the same graph
		if (curr_vertex_desc == _graph[curr_vertex_desc].prev)
			return std::make_pair(0, std::vector<unsigned int>{});

		route.second.push_back(_graph[curr_vertex_desc].id);
		curr_vertex_desc = _graph[curr_vertex_desc].prev;
	}
	return route;
#endif // PRE_PROCESSING
}

std::vector<edge_t> NetworkGraph::graph_edges() const
{
	std::vector<edge_t> edges_list;

	auto epair = edges(_graph);
	for (auto it = epair.first; it != epair.second; it++) {
		auto vpair = std::make_pair(_graph[source(*it, _graph)].id,
			_graph[target(*it, _graph)].id);
		edges_list.push_back(std::make_pair(_graph[*it].cost, vpair));
	}

#ifdef DEBUG
	std::cout << "Uniq edges:" << std::endl;
	for (auto edge : edges_list)
		std::cout << edge.second.first << "," << edge.second.second << " = "
			<< edge.first << std::endl;
#endif

	return edges_list;
}

// clear everything
void NetworkGraph::clear_graph()
{
	_vertex_lut.clear();
	_graph.clear();
	_graph_filename.clear();
}
