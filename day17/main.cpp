#include "main.hpp"

const std::regex regex_x("^x=(\\d+), y=(\\d+)..(\\d+)$");
const std::regex regex_y("^y=(\\d+), x=(\\d+)..(\\d+)$");

bool parse_line(const std::string line, std::map<coord_str, char>& scan) {
	std::smatch sm;
	uint32_t x1, x2, y1, y2;

	if (std::regex_match(line, sm, regex_x)) {
		x1 = stoi(sm.str(1));
		y1 = stoi(sm.str(2));
		y2 = stoi(sm.str(3));

		for (uint32_t i = y1; i <= y2; ++i) {
			scan[coord_str(x1, i)] = '#';
		}

	} else if (std::regex_match(line, sm, regex_y)) {
		y1 = stoi(sm.str(1));
		x1 = stoi(sm.str(2));
		x2 = stoi(sm.str(3));

		for (uint32_t i = x1; i <= x2; ++i) {
			scan[coord_str(i, y1)] = '#';
		}

	} else {
		std::cout << "Invalid scan line '" << line << "'." << std::endl;
		return false;
	}

	return true;
}

bool init(const std::vector<std::string> input, std::map<coord_str, char>& scan) {
	int cnt = 0;
	std::vector<std::string> snippets, insts;
	bool inst_part = false;

	if (!input.size()) {
		std::cout << "Empty input." << std::endl;
		return false;
	}

	for (uint32_t i = 0; i < input.size(); ++i) {
		if (!parse_line(input[i], scan)) {
			return false;
		}
	}

	return true;
}

bool init(std::map<coord_str, char>& scan) {
	std::ifstream input;
	std::string line;
	std::vector<std::string> lines;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return false;
	}

	lines.clear();

	while (std::getline(input, line)) {
		lines.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}

	return init(lines, scan);
}

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	std::map<coord_str, char> main_scan;

#if TEST
	if (!init({"x=495, y=2..7", "y=7, x=495..501", "x=501, y=3..7", "x=498, y=2..4", "x=506, y=1..2", "x=498, y=10..13", "x=504, y=10..13", "y=13, x=498..504"},
			  main_scan)) {
		return -1;
	}

#endif

	if (!init(main_scan)) {
		return -1;
	}
	
	std::cout << "=== Advent of Code 2018 - day 17 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
