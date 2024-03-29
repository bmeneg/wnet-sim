#ifndef CORE_HPP
#define CORE_HPP

#include <iostream>
#include "network_graph.hpp"

class Core
{
public:
	int init(int, char *[]);
	NetworkGraph * network_graph();
	int run_cli(void);

private:
	int _handle_po(int, char *[]);

	NetworkGraph _ngraph;
};


#endif // CORE_HPP
