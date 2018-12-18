#include <fstream>
#include <iostream>

#define TEST1 0
#define TEST2 0

int main(void) {
	int cnt = 0;
	uint32_t result1x = 0, result1y = 0, result2x = 0, result2y = 0, result2z = 0, grid_sn;
	std::ifstream input;
	std::string line;

	std::cout << "=== Advent of Code 2018 - day 12 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
	}

	if (input.is_open()) {
		input.close();
	}

#endif

	std::cout << "Result is " << result1x << "," << result1y << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2x << "," << result2y << "," << result2z << std::endl;
}
