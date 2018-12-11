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

const std::regex condition_template("^Step ([A-Z]) must be finished before step ([A-Z]) can begin.$");

bool DecodeInstruction(const std::string line, std::unordered_map<char, std::string> &rules, std::map<char, int32_t> &counts) {
	std::smatch sm;
	char x, y;

	if (std::regex_match(line, sm, condition_template)) {
		x = sm.str(1)[0];
		y = sm.str(2)[0];
		counts[x]++;
		counts[y]++;
		rules[x] += y;
		return true;
	}
	return false;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line;
	std::unordered_map<char, std::string> rules;
	std::map<char, int32_t> counts;

	std::cout << "=== Advent of Code 2018 - day 7 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	rules.clear();
	counts.clear();

#if TEST1
	DecodeInstruction("Step C must be finished before step A can begin.", rules, counts);
	DecodeInstruction("Step C must be finished before step F can begin.", rules, counts);
	DecodeInstruction("Step A must be finished before step B can begin.", rules, counts);
	DecodeInstruction("Step A must be finished before step D can begin.", rules, counts);
	DecodeInstruction("Step B must be finished before step E can begin.", rules, counts);
	DecodeInstruction("Step D must be finished before step E can begin.", rules, counts);
	DecodeInstruction("Step F must be finished before step E can begin.", rules, counts);

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
		if (!DecodeInstruction(line, rules, counts)) {
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
