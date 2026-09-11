#include <cstring>
#include <cerrno>

#include "common.hpp"

std::ifstream aoc::open_data(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <file-name>" << std::endl;
		std::exit(1);
	}

	std::string fname = argv[1];
	std::ifstream file(fname);
	if (!file.is_open()) {
		std::cerr << "Cannot open '" << argv[1] << "': " <<
			strerror(errno) << std::endl;
		std::exit(1);
	}

	return file;
}
