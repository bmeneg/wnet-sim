#ifndef CORE_HPP
#define CORE_HPP

#include <iostream>
#include "network_graph.hpp"

class Core
{
public:
	int init(int, char *[]);
	NetworkGraph * run_gui(std::string filename);
	int run_cli(void);

private:
	NetworkGraph ngraph;

	int handle_po(int, char *[]);
};


#endif // CORE_HPP
