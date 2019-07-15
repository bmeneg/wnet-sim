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
	po::options_description cmdline_opts;
	po::options_description normal_opts("Allowed options");
	po::options_description hidden_opts;
	po::positional_options_description pos_opts;
	po::variables_map var_map;

	try {
		normal_opts.add_options()
			("help,h", "this help message")
			("no-gui", "disable gui")
			;
		hidden_opts.add_options()
			("input-file",
			 po::value<std::string>()->default_value("graph.txt"),
			 "graph file")
			;
		pos_opts.add("input-file", -1);
		cmdline_opts.add(normal_opts).add(hidden_opts);
		store(po::command_line_parser(
		      argc,
		      argv).options(cmdline_opts).positional(pos_opts).run(),
		      var_map);
		notify(var_map);

		if (var_map.count("help")) {
			std::cout << usage << std::endl;
			std::cout << normal_opts << std::endl;
			return 1;
		}
		if (var_map.count("no-gui"))
			enable_gui = false;
		if (var_map.count("input-file")) {
			in_graph_file = var_map["input-file"].as<std::string>();
		} else {
			std::cout << "error: input file missing" << std::endl;
			return -1;
		}
	} catch (std::exception& e) {
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}

	_ngraph.graph_file(in_graph_file);
	if (enable_gui)
		return 0;
	return 1;
}

NetworkGraph * Core::run_gui(std::string filename)
{
	_ngraph.graph_file(filename);
	_ngraph.add_routes_from_file(_ngraph.graph_file());
	return &_ngraph;
}

int Core::run_cli(void)
{
	unsigned long src, dest;
	route_t route;

	_ngraph.add_routes_from_file(_ngraph.graph_file());
	_ngraph.graph_edges();
	_ngraph.calc_routing_graph();

	std::cout << "Choose the source node: ";
	std::cin >> src;
	std::cout << "Choose the destination node: ";
	std::cin >> dest;

	route = _ngraph.find_shortest_path(src, dest);

	std::cout << "distance from " << src << " to " << dest << ": " <<
		     route.first << std::endl;
	std::cout << "route: ";
	for (unsigned long id : route.second)
		std::cout << id << " ";
	std::cout << std::endl;

	return 0;
}

int Core::init(int argc, char *argv[])
{
	return _handle_po(argc, argv);
}
