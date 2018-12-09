#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#define TEST1 1
#define TEST2 0

typedef struct limits {
	uint32_t x_min;
	uint32_t y_min;
	uint32_t x_max;
	uint32_t y_max;
} limits_t;

const std::regex coord_template("(\\d+), (\\d+)");

limits_t GetCoordLimits(const std::vector<std::pair<int32_t, int32_t>> &coords) {
	limits_t result{};

	for (uint32_t i = 0; i < coords.size(); ++i) {
		if (!i) {
			result.x_max = result.x_min = coords[i].first;
			result.y_max = result.y_min = coords[i].second;
		} else {
			if (result.x_min > coords[i].first) {
				result.x_min = coords[i].first;
			}
			if (result.x_max < coords[i].first) {
				result.x_max = coords[i].first;
			}
			if (result.y_min > coords[i].second) {
				result.y_min = coords[i].second;
			}
			if (result.y_max < coords[i].second) {
				result.y_max = coords[i].second;
			}
		}
	}

	result.x_min--;
	result.x_max++;
	result.y_min--;
	result.y_max++;

	return result;
}

void FillClosestCoordinateMap(const std::vector<std::pair<int32_t, int32_t>> coords, const limits_t limits,
							  std::unordered_map<std::string, std::pair<int32_t, int32_t>> &map) {
	
}

bool DecodeCoord(const std::string line, std::vector<std::pair<int32_t, int32_t>> &coords) {
	std::smatch sm;
	int32_t x, y;

	if (std::regex_match(line, sm, coord_template)) {
		x = stoi(sm.str(1));
		y = stoi(sm.str(2));
		coords.push_back(std::pair<int32_t, int32_t>(x, y));
		return true;
	}
	return false;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line, polymer;
	std::vector<std::pair<int32_t, int32_t>> coords;
	std::unordered_map<std::string, std::pair<int32_t, int32_t>> map;
	limits_t limits;

	std::cout << "=== Advent of Code 2018 - day 6 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	coords.clear();
	map.clear();

#if TEST1
	coords.push_back(std::pair<int32_t, int32_t>(1, 1));
	coords.push_back(std::pair<int32_t, int32_t>(1, 6));
	coords.push_back(std::pair<int32_t, int32_t>(8, 3));
	coords.push_back(std::pair<int32_t, int32_t>(3, 4));
	coords.push_back(std::pair<int32_t, int32_t>(5, 5));
	coords.push_back(std::pair<int32_t, int32_t>(8, 9));
#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
		if (!DecodeCoord(line, coords)) {
			std::cout << "Invalid coord at line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	limits = GetCoordLimits(coords);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
