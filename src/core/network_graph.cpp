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
#include "protocol.hpp"

using namespace boost;

const unsigned int MAX_NUM_VERTICES=6;

NetworkGraph::NetworkGraph()
{
	_grid.resize(_grid_dim * _grid_dim);
}

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

unsigned int NetworkGraph::grid_dim(void) const
{
	return _grid_dim;
}

void NetworkGraph::add_routes_from_file()
{
	if (_graph_filename.empty()) {
		std::cerr << "_graph_filename is empty!" << std::endl;
		exit(1);
	}

	std::ifstream routes_file(_graph_filename);
	std::string line;
	std::vector<std::string> tokens;
	std::vector<unsigned int> ve_attr;
	Vertex *vertex_0 = nullptr, *vertex_1 = nullptr;
	bool vertex_0_found, vertex_1_found;

	// Protocol message
	ProtoMsg src_proto_msg, dst_proto_msg;

	while (std::getline(routes_file, line)) {
		if (line == "")
			continue;

		// zerofy control structs
		vertex_0_found = vertex_1_found = false;
		ve_attr.clear();

		// retrieve data from graph file, which follows per line:
		// <v0> <v1> <cost> <v0_x> <v0_y> <v1_x> <v0_y>
		// and covert to unsigned int
		split(tokens, line, is_any_of(" "));
		for (auto it = tokens.begin(); it != tokens.end(); it++)
			ve_attr.push_back(static_cast<unsigned int>(std::stoul(*it)));

		for (auto vertex_lut : _vertex_lut) {
			Vertex *vertex = vertex_lut.second;
			if (!vertex_0_found && vertex->id == ve_attr.at(0)) {
				vertex_0_found = true;
				vertex_0 = vertex;
			} else if (!vertex_1_found && vertex->id == ve_attr.at(1)) {
				vertex_1_found = true;
				vertex_1 = vertex;
			}
		}

		src_proto_msg = {};
		dst_proto_msg = {};

		// in case vertex doesn't already exists, create new one and add
		// it to LUT
		if (!vertex_0_found) {
			vertex_0 = new Vertex({ve_attr.at(0),
								   {ve_attr.at(3), ve_attr.at(4)}});
			vertex_0->self = add_vertex(*vertex_0, _graph);
			src_proto_msg.dst_id = BROADCAST_ID;
			src_proto_msg.payload.msg = MSG_HELLO;
			src_proto_msg.src_id = vertex_0->id;
			vertex_0->msg_table.push_back(src_proto_msg);

			for (auto adj_item : _adj_list) {
				if (adj_item.second.first == vertex_0->id ||
					adj_item.second.second == vertex_0->id) {
					for (auto lut_item : _vertex_lut) {
						if (lut_item.first == adj_item.second.second ||
							lut_item.first == adj_item.second.first) {
							lut_item.second->msg_table.push_back(src_proto_msg);

							dst_proto_msg.src_id = lut_item.second->id;
							dst_proto_msg.dst_id = vertex_0->id;
							dst_proto_msg.payload.msg = MSG_UPDATE;
//							for (auto route : vertex_1->routing_table) {
//								unsigned int route_cost = route.first.cost;
//								std::vector<unsigned int> vertices_id;
//								for (auto vertex : route.second) {
//									vertices_id.push_back(vertex->id);
//								}
//								dst_proto_msg.payload.routing_table.push_back(
//											std::make_pair(route_cost, vertices_id));
//							}
							vertex_0->msg_table.push_back(dst_proto_msg);
							lut_item.second->msg_table.push_back(dst_proto_msg);
						}
					}
				}
			}
			_vertex_lut.push_back(std::make_pair(vertex_0->id, vertex_0));
		}

		src_proto_msg = {};
		dst_proto_msg = {};

		if (!vertex_1_found) {
			vertex_1 = new Vertex({ve_attr.at(1),
								   {ve_attr.at(5), ve_attr.at(6)}});
			vertex_1->self = add_vertex(*vertex_1, _graph);
			src_proto_msg.dst_id = BROADCAST_ID;
			src_proto_msg.payload.msg = MSG_HELLO;
			src_proto_msg.src_id = vertex_1->id;
			vertex_1->msg_table.push_back(src_proto_msg);
			std::cout << "vertex_1 = " << vertex_1->id << std::endl;

			for (auto adj_item : _adj_list) {
				if (adj_item.second.first == vertex_1->id ||
					adj_item.second.second == vertex_1->id) {
					for (auto lut_item : _vertex_lut) {
						if (lut_item.first == adj_item.second.second ||
							lut_item.first == adj_item.second.first) {
							lut_item.second->msg_table.push_back(src_proto_msg);

							dst_proto_msg.src_id = lut_item.second->id;
							dst_proto_msg.dst_id = vertex_1->id;
							dst_proto_msg.payload.msg = MSG_UPDATE;
//							for (auto route : vertex_1->routing_table) {
//								unsigned int route_cost = route.first.cost;
//								std::vector<unsigned int> vertices_id;
//								for (auto vertex : route.second) {
//									vertices_id.push_back(vertex->id);
//								}
//								dst_proto_msg.payload.routing_table.push_back(
//											std::make_pair(route_cost, vertices_id));
//							}
							lut_item.second->msg_table.push_back(dst_proto_msg);
							vertex_1->msg_table.push_back(dst_proto_msg);
						}
					}
				}
			}
			_vertex_lut.push_back(std::make_pair(vertex_1->id, vertex_1));
		}

		if (vertex_0 == nullptr || vertex_1 == nullptr) {
			std::cerr << "ERROR: vertices are null" << std::endl;
			exit(1);
		}

		add_edge(vertex_0->self, vertex_1->self, {ve_attr.at(2)}, _graph);
		// sort vertex LUT to allow binary search
		std::sort(_vertex_lut.begin(), _vertex_lut.end());
	}


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
	std::vector<unsigned int> busy_pixel;
	unsigned int id_count = 0;
	bool maxed_id = false;

	// place the resulting vector to a temp file to be processed afterwards
	std::string tmp_filename(".wn_tmp");
	std::ofstream out_file(tmp_filename);

	// populate the grid with a certain propability
	// TODO: drop hardcoded probability and allow user selection
	for (unsigned int y = 0; y < _grid_dim; y++) {
		for (unsigned int x = 0; x < _grid_dim; x++) {
			if ((rand() % 8) < 1) {
				unsigned int pixel = x + y * _grid_dim;
				Vertex *vertex = new Vertex({id_count++, {x, y}});
				_vertex_lut.push_back(std::make_pair(vertex->id, vertex));
				_grid.at(pixel) = vertex;
				busy_pixel.push_back(pixel);
			}
			if (id_count > MAX_NUM_VERTICES) {
				maxed_id = true;
				break;
			}
		}
		if (maxed_id)
			break;
	}

#ifdef DEBUG
	std::cout << "random> grid created with " << busy_pixel.size() <<
				 " vertices" << std::endl;
#endif
	_calc_adjacent_list(busy_pixel);

	for (auto item : _adj_list) {
		out_file << item.second.first << ' ' << item.second.second << ' ' <<
					item.first << ' ' <<
					_vertex_lut[item.second.first].second->coord.x << ' ' <<
					_vertex_lut[item.second.first].second->coord.y << ' ' <<
					_vertex_lut[item.second.second].second->coord.x << ' ' <<
					_vertex_lut[item.second.second].second->coord.y << std::endl;
//		for (auto msg : _vertex_lut[item.second.second].second->msg_table) {
//			std::cout << msg.payload.msg << std::endl;
//		}
	}

	_vertex_lut.clear();
	out_file.close();
	_graph_filename = tmp_filename;

#ifdef DEBUG
	std::cout << "filename: " << tmp_filename << std::endl;
#endif
}

void NetworkGraph::_calc_adjacent_list(std::vector<unsigned int> busy_pixel)
{
	// the distance between pixel will be considered 1du (distance units)
	const unsigned int dist_pixel = 1;

	// maximum field of view (range) for which we consider when a vertex can
	// actually "see" other vertex.
	const unsigned int max_fov = 4 * dist_pixel;

	// traverse the vector of pixels calculating the euclidean distance
	// between every vertex to determine what which can actually see
	for (unsigned int i = 0; i < busy_pixel.size(); i++) {
		unsigned int src_pixel = busy_pixel.at(i);
		Vertex *src_vertex = _grid.at(src_pixel);

		for (unsigned int j = i + 1; j < busy_pixel.size(); j++) {
//		for (unsigned int j = 0; j < busy_pixel.size(); j++) {
			unsigned int dst_pixel = busy_pixel.at(j);
			Vertex *dst_vertex = _grid.at(dst_pixel);

			unsigned int dx = dist_pixel * (src_vertex->coord.x -
							dst_vertex->coord.x);
			unsigned int dy = dist_pixel * (src_vertex->coord.y -
							dst_vertex->coord.y);
			unsigned int euc_dist = static_cast<unsigned int>(
										std::sqrt(dx * dx + dy * dy));
			if (euc_dist < max_fov) {
				_adj_list.push_back(std::make_pair(euc_dist,
												   std::make_pair(src_vertex->id,
																  dst_vertex->id)));
			}
		}
	}

#ifdef DEBUG
	std::cout << "random> distance vector defined" << std::endl;
	for (auto item : _adj_list) {
		std::cout << item.second.first << " " << item.second.second << " "
			<< item. first << std::endl;
	}
#endif
}

int NetworkGraph::calc_routing_graph()
{
#ifdef PRE_PROCESSING
	std::vector<unsigned int> distance(num_vertices(_graph));
	vertex_desc_t curr_vertex_desc = 0;

	// run the whole graph collecting each distance for all vertex
	for (auto src_vpair : _vertex_lut) {
		dijkstra_shortest_paths(_graph, src_vpair.second->self,
			predecessor_map(get(&Vertex::prev, _graph))
			.distance_map(make_iterator_property_map(distance.begin(),
				get(vertex_index, _graph)))
			.weight_map(get(&Edge::cost, _graph)));

		for (auto dst_vpair : _vertex_lut) {
			route_t route;
			route.first.cost = distance[dst_vpair.second->self];
			curr_vertex_desc = dst_vpair.second->self;
			while (curr_vertex_desc != src_vpair.second->self) {
				// if the following condition is true and the while didn't
				// finish, it means the vertices are not in the same graph
				if (curr_vertex_desc == _graph[curr_vertex_desc].prev)
					return -1;

				route.second.push_back(_vertex_lut[curr_vertex_desc].second);
				_vertex_lut[curr_vertex_desc].second->prev = _graph[curr_vertex_desc].prev;
				curr_vertex_desc = _vertex_lut[curr_vertex_desc].second->prev;
			}
			_vertex_lut[src_vpair.second->self].second->routing_table.push_back(route);
		}
	}
#endif // PRE_PROCESSING
	return 0;
}

route_t NetworkGraph::find_shortest_path(unsigned int src, unsigned int dst)
{
#ifdef PRE_PROCESSING
	return _vertex_lut[src].second->routing_table[dst];
#else
	std::vector<unsigned int> distance(num_vertices(_graph));
	vertex_desc_t curr_vertex_desc = 0;
	route_t route;

	// run the whole graph collecting each distance for all vertex
	dijkstra_shortest_paths(_graph, _vertex_lut[src].second->self,
		predecessor_map(get(&Vertex::prev, _graph))
		.distance_map(make_iterator_property_map(distance.begin(),
			get(vertex_index, _graph)))
		.weight_map(get(&Edge::cost, _graph)));

	route.first = distance[_vertex_lut[dst].second->self];
	curr_vertex_desc = _vertex_lut[dst].second->self;
	while (curr_vertex_desc != _vertex_lut[src].second->self) {
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
		Vertex *src_v = _vertex_lut[_graph[source(*it, _graph)].id].second;
		Vertex *dst_v = _vertex_lut[_graph[target(*it, _graph)].id].second;
		Edge edge = _graph[*it];

		auto vpair = std::make_pair(src_v, dst_v);
		edges_list.push_back(std::make_pair(edge, vpair));
	}

#ifdef DEBUG
	std::cout << "Uniq edges:" << std::endl;
	for (auto edge : edges_list)
		std::cout << edge.second.first->id << "," << edge.second.second->id <<
					 " = " << edge.first.cost << std::endl;
#endif

	return edges_list;
}

// clear everything
void NetworkGraph::clear_graph()
{
	_vertex_lut.clear();
	_grid.clear();
	_grid.resize(_grid_dim * _grid_dim);
	_adj_list.clear();
	_graph.clear();
	_graph_filename.clear();
}

Vertex * NetworkGraph::vertex(unsigned int id)
{
	return _vertex_lut[id].second;
}
