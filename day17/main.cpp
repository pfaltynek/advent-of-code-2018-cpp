#include "main.hpp"

const std::regex regex_bef("^Before: \\[(\\d), (\\d), (\\d), (\\d)\\]$");
const std::regex regex_ins("^(\\d+) (\\d) (\\d) (\\d)$");
const std::regex regex_aft("^After:  \\[(\\d), (\\d), (\\d), (\\d)\\]$");
/*
bool init(const std::vector<std::string> input, std::vector<snippet_str>& debug_lines, std::vector<instr_str>& instructions) {
	int cnt = 0;
	std::vector<std::string> snippets, insts;
	bool inst_part = false;

	if (!input.size()) {
		std::cout << "Empty input." << std::endl;
	}

	for (uint32_t i = 0; i < input.size(); ++i) {
		if (input[i].empty()) {
			cnt++;
			if (cnt > 1) {
				inst_part = true;
			}
		} else {
			cnt = 0;
			if (inst_part) {
				insts.push_back(input[i]);
			} else {
				snippets.push_back(input[i]);
			}
		}
	}

	return parse_instructions(insts, instructions) && parse_snippets(snippets, debug_lines);
}

bool init(std::vector<snippet_str>& debug_lines, std::vector<instr_str>& instructions) {
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

	return init(lines, debug_lines, instructions);
}
*/

int main(void) {
	uint64_t result1 = 0, result2 = 0;

#if TEST

#endif
/*
	if (!init(debug_lines, instructions)) {
		return -1;
	}
*/
	std::cout << "=== Advent of Code 2018 - day 17 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
