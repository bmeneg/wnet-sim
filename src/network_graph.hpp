#include <iostream>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

class NetworkGraph
{
private:
    struct Route {
        int src_id;
        int dst_id;
        unsigned int cost;

        friend std::istream& operator>>(std::istream& is, Route& route)
        {
            std::vector<std::string> tokens;
            std::string line;
            std::getline(is, line);
            if (line == "") {
                return is;
            }
            boost::split(tokens, line, boost::is_any_of(" "));
            route.src_id = std::stoi(tokens[0]);
            route.dst_id = std::stoi(tokens[1]);
            route.cost = std::stoul(tokens[2]);
            return is;
        }
    };

    void populate_graph(std::vector<struct Route>);

public:
    void add_routes_from_file(std::string);
};
