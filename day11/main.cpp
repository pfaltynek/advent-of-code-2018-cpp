#include <fstream>
#include <iostream>
#include <unordered_map>

#define TEST1 1
#define TEST2 0

int32_t GetPowerLevel(uint32_t x, uint32_t y, uint32_t grid_sn) {
	int32_t rack_id, result;

	rack_id = x + 10;
	result = rack_id * y;
	result += grid_sn;
	result *= rack_id;
	result = ((result / 100) % 10) - 5;

	return result;
}

int main(void) {
	int cnt = 0;
	uint32_t result1x = 0, result1y = 0, result2 = 0, grid_sn;
	std::ifstream input;
	std::string line;

	std::cout << "=== Advent of Code 2018 - day 11 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	int test;

	test = GetPowerLevel(3, 5, 8); // 4
	test = GetPowerLevel(122, 79, 57); // -5
	test = GetPowerLevel(217, 196, 39); // 0
	test = GetPowerLevel(101, 153, 71); // 4
#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
		if (!DecodeElvPoint(line, points)) {
			std::cout << "Invalid point data at line " << cnt << std::endl;
			return -1;
		}
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	std::cout << "Result is " << result1x << "," << result1y << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
