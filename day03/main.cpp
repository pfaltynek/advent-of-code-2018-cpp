#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#define TEST1 1
#define TEST2 0


typedef struct claim {
	uint32_t id;
	uint32_t left;
	uint32_t top;
	uint32_t width;
	uint32_t height;
} claim_str;

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;

	std::cout << "=== Advent of Code 2018 - day 3 ====" << std::endl;
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
		//ids.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
