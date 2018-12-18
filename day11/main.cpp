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

void GetLargestTotalPower(const int32_t grid_sn, int32_t grid_size, int32_t area_size, uint32_t &result_x1, uint32_t &result_y1, uint32_t &result_x2,
						  uint32_t &result_y2, uint32_t &result_z2) {
	int32_t grid[grid_size][grid_size];
	int32_t max1 = INT32_MIN, max2 = INT32_MIN, sum;
	uint32_t i, j, i1, j1, s;

	result_x1 = 0;
	result_y1 = 0;
	result_x2 = 0;
	result_y2 = 0;
	result_z2 = 0;

	for (i = 0; i < grid_size; ++i) {
		for (j = 0; j < grid_size; ++j) {
			grid[i][j] = GetPowerLevel(i + 1, j + 1, grid_sn);
		}
	}

	for (s = 1; s <= grid_size; ++s) {
		for (i = 0; i < (grid_size - s + 1); ++i) {
			for (j = 0; j < (grid_size - s + 1); ++j) {
				sum = 0;
				for (i1 = 0; i1 < s; ++i1) {
					for (j1 = 0; j1 < s; ++j1) {
						sum += grid[i + i1][j + j1];
					}
				}

				if ((s == area_size) && (sum > max1)) {
					max1 = sum;
					result_x1 = i + 1;
					result_y1 = j + 1;
				}

				if (sum > max2) {
					max2 = sum;
					result_x2 = i + 1;
					result_y2 = j + 1;
					result_z2 = s;
				}
			}
		}
	}
}

int main(void) {
	int cnt = 0;
	uint32_t result1x = 0, result1y = 0, result2x = 0, result2y = 0, result2z = 0, grid_sn;
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

	GetLargestTotalPower(18, 300, 3, result1x, result1y, result2x, result2y, result2z); // 33,45,90,269,16
	GetLargestTotalPower(42, 300, 3, result1x, result1y, result2x, result2y, result2z); // 21,61,232,251,12

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

	GetLargestTotalPower(grid_sn, 300, 3, result1x, result1y, result2x, result2y, result2z);
#endif

	std::cout << "Result is " << result1x << "," << result1y << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2x << "," << result2y << "," << result2z << std::endl;
}
