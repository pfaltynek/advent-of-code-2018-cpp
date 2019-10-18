#include "main.hpp"

const std::regex nanobot_regex("^pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(\\d+)$");

typedef enum AREA_TYPE { rocky = 0, wet = 1, narrow = 2 } area_type_t;
typedef enum TOOL_TYPE { torch = 1, climbing_gear = 2, neither = 0 } tool_type_t;

typedef struct NANOBOT_INFO {
	int32_t x, y, z, r;
} nanobot_info_str;

class ExperimentalEmergencyTransportation {
  public:
	bool init(const std::vector<std::string> input);
	bool init();

  private:
	std::vector<nanobot_info_str> data_;
};


bool ExperimentalEmergencyTransportation::init(const std::vector<std::string> input) {
	std::smatch sm;
	nanobot_info_str nanobot;
	data_.clear();

	for (uint32_t i = 0; i < input.size(); i++) {
		if (std::regex_match(input[i], sm, nanobot_regex)) {
			nanobot.x = stoi(sm.str(1));
			nanobot.y = stoi(sm.str(2));
			nanobot.z = stoi(sm.str(3));
			nanobot.r = stoi(sm.str(4));
		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		data_.push_back(nanobot);
	}

	return true;
}

bool ExperimentalEmergencyTransportation::init() {
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
	ExperimentalEmergencyTransportation eet;

#if TEST
	if (!eet.init({"pos=<0,0,0>, r=4", "pos=<1,0,0>, r=1", "pos=<4,0,0>, r=3", "pos=<0,2,0>, r=1", "pos=<0,5,0>, r=3", "pos=<0,0,3>, r=1", "pos=<1,1,1>, r=1",
					"pos=<1,1,2>, r=1", "pos=<1,3,1>, r=1"})) {
		return -1;
	}

	result1 = 1;

#endif

	if (!eet.init()) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 23 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
