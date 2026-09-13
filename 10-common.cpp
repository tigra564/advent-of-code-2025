#include "10-common.hpp"

Machine::Machine(const std::string& record)
{
	// [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
	std::stringstream ss(record);
	std::string item;

	while (ss >> item) {
		switch (item[0]) {
		case '[':
			num_inds = item.size() - 2;
			for (std::size_t i = 0; i < num_inds; i++) {
				if (item[i+1] == '#')
					indicators |= 1U << i;
			}
			break;
		case '(': {
			unsigned int button = 0;
			// We rightfully assume that button spec consist of digits, not numbers.
			// And we don't care about ')' and whatnot, only digits.
			for (const auto& c : item) {
				if ('0' <= c && c <= '9')
					button |= 1U << (c - '0');
			}
			buttons.emplace_back(button);
			break;
			}
		case '{': {
			std::stringstream bs(item.substr(1, item.size()-1));
			std::string token;
			while (std::getline(bs, token, ','))
				joltages.emplace_back(std::stoul(token));
			break;
			}
		}
	}
}


std::ostream& operator<<(std::ostream& os, const Machine& machine)
{
	os << '[';
	for (auto i = 0U; i < machine.num_inds; i++)
		os << (machine.indicators & (1U << i) ? '#' : '.');
	os << ']';

	bool first;
	for (const auto& b: machine.buttons) {
		first = true;
		os << " (";
		for (auto i = 0U; i < machine.num_inds; i++) {
			if (! (b & (1U << i)) )
				continue;
			if (!first)
				os << ',';
			first = false;
			os << i;
		}
		os << ')';
	}

	os << " {";
	for (std::size_t j = 0; j < machine.joltages.size(); j++) {
		if (j != 0)
			os << ',';
		os << machine.joltages[j];
	}
	os << '}';

	return os;
}
