#include <iostream>
#include <fstream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include "network_graph.hpp"

using namespace std;
using namespace boost;

void NetworkGraph::add_routes_from_file(string filename)
{
	ifstream routes_file(filename);
	string line;
	vector<string> tokens;
	vertex_desc_t vertex_1, vertex_2;
	bool vertex_1_found = false;
	bool vertex_2_found = false;

	while (getline(routes_file, line)) {
		if (line == "") {
			continue;
		}

		split(tokens, line, is_any_of(" "));
		auto vpair = vertices(graph);
		for (auto it = vpair.first; it != vpair.second; it++) {
			if (graph[*it].id == stoi(tokens.at(0))) {
				vertex_1 = *it;
				vertex_1_found = true;
			}
			else if (graph[*it].id == stoi(tokens.at(1))) {
				vertex_2 = *it;
				vertex_2_found = true;
			}
		}

		if (!vertex_1_found)
			vertex_1 = add_vertex({stoi(tokens.at(0))}, graph);
		if (!vertex_2_found)
			vertex_2 = add_vertex({stoi(tokens.at(1))}, graph);

		add_edge(vertex_1, vertex_2, {stoul(tokens.at(2))}, graph);
	}

#ifdef DEBUG
	cout << "vertices found: " << endl;
	print_graph(graph, get(&Vertex::id, graph));
#endif
}
