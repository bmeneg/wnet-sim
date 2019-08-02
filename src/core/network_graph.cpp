#include <iostream>
#include <fstream>
#include <algorithm>

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

std::string NetworkGraph::graph_file(void)
{
	return _graph_filename;
}

void NetworkGraph::add_routes_from_file(std::string filename)
{
	std::ifstream routes_file(filename);
	std::string line;
	std::vector<std::string> tokens;
	std::vector<unsigned long> ve_attr;
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
		// and covert to unsigned long
		split(tokens, line, is_any_of(" "));
		for (auto it = tokens.begin(); it != tokens.end(); it++)
			ve_attr.push_back(std::stoul(*it));

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

void NetworkGraph::add_routes_random()
{
	// do nothing
}

void NetworkGraph::calc_routing_graph()
{
	std::vector<int> distance(num_vertices(_graph));
	vertex_desc_t curr_vertex_desc = 0;

	// run the whole graph collecting each distance for all vertex
	for (auto src_vpair : _vertex_lut) {
		dijkstra_shortest_paths(_graph, src_vpair.second,
			predecessor_map(get(&Vertex::prev, _graph))
			.distance_map(make_iterator_property_map(distance.begin(),
				get(vertex_index, _graph)))
			.weight_map(get(&Edge::cost, _graph)));

		for (auto dest_vpair : _vertex_lut) {
			route_t route;
			route.first = distance[dest_vpair.second];
			curr_vertex_desc = dest_vpair.second;
			while (curr_vertex_desc != src_vpair.second) {
				route.second.push_back(_graph[curr_vertex_desc].id);
				curr_vertex_desc = _graph[curr_vertex_desc].prev;
			}
			_graph[src_vpair.second].routing_table.push_back(route);
		}
	}
}

route_t NetworkGraph::find_shortest_path(unsigned long src, unsigned long dest)
{
	return _graph[_vertex_lut[src].second].routing_table[dest];
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
		std::cout << edge.second.first << "," << edge.second.second << " = " <<
			edge.first << std::endl;
#endif

	return edges_list;
}
