#include "11-common.hpp"

Connections parse_connections(std::ifstream& file)
{
	Connections connections;
	std::string record;

	while (std::getline(file, record)) {
		auto delim_pos = record.find(':');
		std::string from = record.substr(0, delim_pos);

		std::set<std::string> to;
		std::stringstream ss(record.substr(delim_pos+1));
		std::string node;
		while (ss >> node)
			to.emplace(node);

		connections.emplace(from, to);
	}

	//std::cout << connections << std::endl;

	return connections;
}
