#include <iostream>
#include <boost/program_options.hpp>

#include "network_graph.hpp"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    /* Command line options handling */
    string app_name(argv[0]);
    string usage("Usage: " + app_name + " [options] <graph file>");
    string in_graph_file;
    po::options_description cmdline_opts;
    po::options_description normal_opts("Allowed options");
    po::options_description hidden_opts;
    po::positional_options_description pos_opts;
    po::variables_map var_map;

    try {
        normal_opts.add_options()
            ("help,h", "this help message")
        ;
        hidden_opts.add_options()
            ("input-file", po::value<string>()->default_value("graph.txt"),
             "graph file")
        ;
        pos_opts.add("input-file", -1);
        cmdline_opts.add(normal_opts).add(hidden_opts);
        store(po::command_line_parser(
                argc, argv).options(cmdline_opts).positional(pos_opts).run(),
              var_map);
        notify(var_map);

        if (var_map.count("help")) {
            cout << usage << endl;
            cout << normal_opts << endl;
            return 0;
        }

        if (var_map.count("input-file")) {
            in_graph_file = var_map["input-file"].as<string>();
        } else {
            cout << "error: input file missing" << endl;
            return 1;
        }
    } catch (exception& e) {
        cout << "error: " << e.what() << endl;
        return 1;
    }

    NetworkGraph ngraph;
    ngraph.add_routes_from_file(in_graph_file);

    return 0;
}
