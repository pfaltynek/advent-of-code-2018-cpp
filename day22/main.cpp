#include "main.hpp"

const std::regex depth_regex("^depth: (\\d+)$");
const std::regex target_regex("^target: (\\d+),(\\d+)$");

typedef enum AREA_TYPE { rocky = 0, wet = 1, narrow = 2 } area_type_t;

class Cave {
  public:
	bool init(const std::vector<std::string> input);
	bool init();

  private:
	int32_t depth_;
	coord_str target_;

	int32_t get_geologic_index(int32_t x, int32_t y);
	int32_t get_erosion_level(int32_t geo_idx);
	area_type_t get_area_type(int32_t erosion_lvl);
};

bool Cave::init(const std::vector<std::string> input) {
	std::smatch sm;

	if (!input.size() < 2) {
		std::cout << "Incomplete input data" << std::endl;
		return false;
	}

	if (std::regex_match(input[0], sm, depth_regex)) {
		depth_ = stoi(sm.str(1));
	} else {
		std::cout << "Invalid input - missing depth data" << std::endl;
		return false;
	}

	if (std::regex_match(input[1], sm, target_regex)) {
		target_.x = stoi(sm.str(1));
		target_.y = stoi(sm.str(2));
	} else {
		std::cout << "Invalid input - missing target data" << std::endl;
		return false;
	}

	return true;
}

bool Cave::init() {
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

int32_t Cave::get_geologic_index(int32_t x, int32_t y) {
	if ((x == target_.x) && (y == target_.y)) {
		return 0;
	}

	if (x == 0) {
		return y * 48721;
	}

	if (y == 0) {
		return x * 16807;
	}

	// TODO: implement cascade!

	return 0;
}

int32_t Cave::get_erosion_level(int32_t geo_idx){
	return (geo_idx + depth_) % 20183;
}

area_type_t Cave::get_area_type(int32_t erosion_lvl){
	return static_cast<area_type_t>(erosion_lvl % 3);
}

int main(void) {
	int32_t result1 = 0, result2 = 0;
	Cave cave;

#if TEST

#endif

	if (!cave.init()) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 22 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
