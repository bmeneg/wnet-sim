#include <iostream>
#include <fstream>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
using namespace boost;

#include "network_graph.hpp"

struct Vertex {
    int id;
    bool selected;
};

struct Edge {
    unsigned int cost;
    bool selected;
};

/* There is no need to use boost::listS since we're not inserting nor removing
 * nodes, thus the performance gain is useless. Because of that we've choosen
 * boost::vectS which provides better space optimization. */
typedef adjacency_list<vecS, vecS, directedS, Vertex, Edge> graph_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_desc_t;
typedef pair<Edge, Edge> edge_t;

void NetworkGraph::populate_graph(vector<struct Route>)
{

}

void NetworkGraph::add_routes_from_file(string filename)
{
    ifstream routes_file(filename);
    string line;
    vector<struct Route> graph_routes;
    struct Route route;

    while (routes_file >> route) {
        graph_routes.push_back(route);
    }

    for_each(graph_routes.begin(), graph_routes.end(),
             [](struct Route route) {
                 cout << route.src_id << " " << route.dst_id << " " <<
                     route.cost << endl;
             });
}
