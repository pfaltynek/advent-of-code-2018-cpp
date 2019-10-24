#include "main.hpp"

const std::regex point_regex("^(-?\\d+),(-?\\d+),(-?\\d+),(-?\\d+)$");

typedef struct POINT {
	int32_t x, y, z, q;

	int32_t get_distance() {
		return abs(x) + abs(y) + abs(z) + abs(q);
	}
	int32_t get_distance(POINT other) {
		return abs(x - other.x) + abs(y - other.y) + abs(z - other.z) + abs(q - other.q);
	}
} point_str;

class Adventure4D {
  public:
	bool init(const std::vector<std::string> input);
	bool init();
	int32_t find_constellation();

  private:
	std::vector<point_str> points_;
};

bool Adventure4D::init(const std::vector<std::string> input) {
	std::smatch sm;
	point_str pt;

	points_.clear();

	for (uint32_t i = 0; i < input.size(); i++) {
		pt = {};

		if (input[i].empty()) {
			continue;
		}

		if (std::regex_match(input[i], sm, point_regex)) {
			pt.x = stoi(sm.str(1));
			pt.y = stoi(sm.str(2));
			pt.z = stoi(sm.str(3));
			pt.q = stoi(sm.str(4));
		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		points_.push_back(pt);
	}

	return true;
}

bool Adventure4D::init() {
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

	return init(lines);
}

int main(void) {
	int32_t result1 = 0, result2 = 0;
	Adventure4D a4d;

#if TEST
	if (!a4d.init({"0,0,0,0", "3,0,0,0", "0,3,0,0", "0,0,3,0", "0,0,0,3", "0,0,0,6", "9,0,0,0", "12,0,0,0"})) {
		return -1;
	}

	result1 = 1;
	result2 = 2;
#endif

	if (!a4d.init()) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 25 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
