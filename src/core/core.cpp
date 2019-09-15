#include <iostream>
#include <boost/program_options.hpp>

#include "core.hpp"
#include "network_graph.hpp"

namespace po = boost::program_options;

int Core::_handle_po(int argc, char *argv[])
{
	/* Command line options handling */
	std::string app_name(argv[0]);
	std::string usage("Usage: " + app_name + " [options] <graph file>");
	std::string in_graph_file;
	bool enable_gui = true;
	unsigned int sqr_weight = 10;
	po::options_description cmdline_opts;
	po::options_description normal_opts("Allowed options");
	po::options_description hidden_opts;
	po::positional_options_description pos_opts;
	po::variables_map var_map;

	try {
		normal_opts.add_options()
			("help,h", "this help message")
			("no-gui", "disable gui")
			("sqr-weight,s", po::value<unsigned int>()->default_value(10),
			 "number of squares of vertices")
			;
		hidden_opts.add_options()
			("input-file", "graph file")
			;
		pos_opts.add("input-file", -1);
		cmdline_opts.add(normal_opts).add(hidden_opts);
		store(po::command_line_parser(argc, argv).options(cmdline_opts).positional(pos_opts).run(),
			var_map);
		notify(var_map);

		if (var_map.count("help")) {
			std::cout << usage << std::endl;
			std::cout << normal_opts << std::endl;
			return 1;
		}
		if (var_map.count("no-gui"))
			enable_gui = false;
		if (var_map.count("input-file"))
			in_graph_file = var_map["input-file"].as<std::string>();
		if (var_map.count("sqr-weight"))
			sqr_weight = var_map["sqr-weight"].as<unsigned int>();
	} catch (std::exception& e) {
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}

	if (enable_gui)
		return 0;

	if (in_graph_file.empty())
		_ngraph.sqr_weight(sqr_weight);
	else
		_ngraph.graph_file(in_graph_file);

	return 1;
}

NetworkGraph * Core::network_graph()
{
	return &_ngraph;
}

int Core::run_cli()
{
	unsigned int src, dst;
	route_t route;

	if (_ngraph.graph_file().empty())
		_ngraph.add_routes_random();

	_ngraph.add_routes_from_file();
	_ngraph.graph_edges();
	_ngraph.calc_routing_graph();

	std::cout << "Choose the source node: ";
	std::cin >> src;
	std::cout << "Choose the destination node: ";
	std::cin >> dst;

	route = _ngraph.find_shortest_path(src, dst);

	std::cout << "distance from " << src << " to " << dst << ": " <<
		route.first << std::endl;
	std::cout << "route: ";
	for (unsigned int id : route.second)
		std::cout << id << " ";
	std::cout << std::endl;

	return 0;
}

int Core::init(int argc, char *argv[])
{
	return _handle_po(argc, argv);
}
