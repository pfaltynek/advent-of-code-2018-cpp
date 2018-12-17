#include <fstream>
#include <iostream>
#include <unordered_map>

#define TEST1 0
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

void GetLargestTotalPower(const int32_t grid_sn, int32_t grid_x, int32_t grid_y, int32_t area_x, int32_t area_y, uint32_t &result_x, uint32_t &result_y) {
	int32_t grid[grid_x][grid_y];
	int32_t max = INT32_MIN, sum;
	uint32_t i, j, i1, j1;

	result_x = 0;
	result_y = 0;

	for (i = 0; i < grid_x; ++i) {
		for (j = 0; j < grid_y; ++j) {
			grid[i][j] = GetPowerLevel(i + 1, j + 1, grid_sn);
		}
	}

	for (i = 0; i < (grid_x - 3); ++i) {
		for (j = 0; j < (grid_y - 3); ++j) {
			sum = 0;
			for (i1 = i; i1 < (i + 3); ++i1) {
				for (j1 = j; j1 < (j + 3); ++j1) {
					sum += grid[i1][j1];
				}
			}

			if (sum > max) {
				max = sum;
				result_x = i + 1;
				result_y = j + 1;
			}
		}
	}
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

	test = GetPowerLevel(3, 5, 8);		// 4
	test = GetPowerLevel(122, 79, 57);  // -5
	test = GetPowerLevel(217, 196, 39); // 0
	test = GetPowerLevel(101, 153, 71); // 4

	GetLargestTotalPower(18, 300, 300, 3, 3, result1x, result1y);
	GetLargestTotalPower(42, 300, 300, 3, 3, result1x, result1y);

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
		grid_sn = stoi(line);
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	GetLargestTotalPower(grid_sn, 300, 300, 3, 3, result1x, result1y);
	std::cout << "Result is " << result1x << "," << result1y << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
