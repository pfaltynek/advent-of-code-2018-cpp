#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#define TEST1 0
#define TEST2 0

typedef struct limits {
	uint32_t x_min;
	uint32_t y_min;
	uint32_t x_max;
	uint32_t y_max;
} limits_t;

const std::regex condition_template("^Step ([A-Z]) must be finished before step ([A-Z]) can begin.$");

bool DecodeInstruction(const std::string line, std::vector<std::pair<char, char>> &conditions) {
	std::smatch sm;
	char x, y;

	if (std::regex_match(line, sm, condition_template)) {
		x = sm.str(1)[0];
		y = sm.str(2)[0];
		conditions.push_back(std::pair<char, char>(x, y));
		return true;
	}
	return false;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::pair<char, char>> conditions;

	std::cout << "=== Advent of Code 2018 - day 7 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	conditions.clear();

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
		if (!DecodeInstruction(line, conditions)) {
			std::cout << "Invalid instruction at line " << cnt << std::endl;
		}
	}

	if (input.is_open()) {
		input.close();
	}
#endif


	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
