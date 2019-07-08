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
	this->graph_filename = filename;
}

std::string NetworkGraph::graph_file(void)
{
	return this->graph_filename;
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

		auto vpair = vertices(this->graph);
		for (auto it = vpair.first; it != vpair.second; it++) {
			if (!vertex_0_found && this->graph[*it].id == ve_attr.at(0)) {
				vertex_0 = *it;
				vertex_0_found = true;
			} else if (!vertex_1_found && this->graph[*it].id == ve_attr.at(1)) {
				vertex_1 = *it;
				vertex_1_found = true;
			}
		}

		// in case vertex doesn't already exists, create new one and add
		// it to LUT
		if (!vertex_0_found) {
			vertex_0 = add_vertex({ve_attr.at(0)}, this->graph);
			this->vertex_lut.push_back(std::make_pair(ve_attr.at(0), vertex_0));
		}
		if (!vertex_1_found) {
			vertex_1 = add_vertex({ve_attr.at(1)}, this->graph);
			this->vertex_lut.push_back(std::make_pair(ve_attr.at(1), vertex_1));
		}

		add_edge(vertex_0, vertex_1, {ve_attr.at(2)}, this->graph);
	}

	// sort vertex LUT to allow binary search
	std::sort(vertex_lut.begin(), vertex_lut.end());

#ifdef DEBUG
	std::cout << "vertices found: ";
	print_vertices(this->graph, get(&Vertex::id, this->graph));
	auto vpair = vertices(this->graph);
	for (auto v_it = vpair.first; v_it != vpair.second; v_it++) {
		std::cout << "edges incident to vertex " << this->graph[*v_it].id <<
			" are: " << std::endl;
		std::pair<edge_it_t, edge_it_t> epair = out_edges(*v_it, this->graph);
		for (auto e_it = epair.first; e_it != epair.second; e_it++) {
			std::cout << "(" << this->graph[source(*e_it, this->graph)].id <<
				     "," << this->graph[target(*e_it, this->graph)].id <<
				     ")" << " = " << this->graph[*e_it].cost <<
				     std::endl;
		}
	}

	std::cout << std::endl;
	print_graph(this->graph, get(&Vertex::id, this->graph));
	std::cout << std::endl;
#endif
}

void NetworkGraph::calc_routing_graph()
{
	std::vector<int> distance(num_vertices(this->graph));
	vertex_desc_t curr_vertex_desc = 0;

	// run the whole graph collecting each distance for all vertex
	for (auto src_vpair : this->vertex_lut) {
		dijkstra_shortest_paths(this->graph, src_vpair.second,
					predecessor_map(get(&Vertex::prev, this->graph))
					.distance_map(make_iterator_property_map(distance.begin(),
					       get(vertex_index, this->graph)))
					.weight_map(get(&Edge::cost, this->graph)));

		for (auto dest_vpair : this->vertex_lut) {
			route_t route;
			route.first = distance[dest_vpair.second];
			curr_vertex_desc = dest_vpair.second;
			while (curr_vertex_desc != src_vpair.second) {
				route.second.push_back(this->graph[curr_vertex_desc].id);
				curr_vertex_desc = graph[curr_vertex_desc].prev;
			}
			this->graph[src_vpair.second].routing_table.push_back(route);
		}
	}
}

route_t NetworkGraph::find_shortest_path(unsigned long src, unsigned long dest)
{
	return this->graph[this->vertex_lut[src].second].routing_table[dest];
}
