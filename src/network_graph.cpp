#include <iostream>
#include <fstream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>

#include "network_graph.hpp"

using namespace boost;

void NetworkGraph::add_routes_from_file(std::string filename)
{
	std::ifstream routes_file(filename);
	std::string line;
	std::vector<std::string> tokens;
	vertex_desc_t vertex_1, vertex_2;
	bool vertex_1_found = false;
	bool vertex_2_found = false;

	while (std::getline(routes_file, line)) {
		if (line == "")
			continue;

		split(tokens, line, is_any_of(" "));
		auto vpair = vertices(graph);
		for (auto it = vpair.first; it != vpair.second; it++) {
			if (graph[*it].id == std::stoi(tokens.at(0))) {
				vertex_1 = *it;
				vertex_1_found = true;
			} else if (graph[*it].id == std::stoi(tokens.at(1))) {
				vertex_2 = *it;
				vertex_2_found = true;
			}
		}

		if (!vertex_1_found)
			vertex_1 = add_vertex({std::stoi(tokens.at(0))}, graph);
		if (!vertex_2_found)
			vertex_2 = add_vertex({std::stoi(tokens.at(1))}, graph);

		add_edge(vertex_1, vertex_2, {std::stoul(tokens.at(2))}, graph);
	}

#ifdef DEBUG
	std::cout << "vertices found: ";
	print_vertices(graph, get(&Vertex::id, graph));
	std::cout << "edges found: ";
	print_edges(graph, get(&Vertex::id, graph));
	std::cout << "resulting graph:" << std::endl;
	print_graph(graph, get(&Vertex::id, graph));
	std::cout << std::endl;
#endif
}

void NetworkGraph::keep_running()
{
	vertex_desc_t vertex;
	//std::vector<int> distance(num_vertices(graph));

	do {
		std::cout << "choose one of the vertices found: ";
		print_vertices(graph, get(&Vertex::id, graph));
		std::cout << std::endl;
		std::cin >> vertex;
		dijkstra_shortest_paths(graph, vertex,
					predecessor_map(get(&Vertex::prev,
							    graph))
					.distance_map(get(&Vertex::prev,
							  graph))
					.weight_map(get(&Edge::cost, graph)));
		auto vpair = vertices(graph);
		for (auto it = vpair.first; it != vpair.second; it++) {
			//std::cout << graph[*it].id << " " << distance[*it] <<
			//	std::endl;
			std::cout << graph[*it].id << " " <<
				graph[graph[*it].prev].id << std::endl;
		}
		//while (graph[vertex].prev != vertex)
		//	cout << graph[vertex].prev;
	} while (true);
}
