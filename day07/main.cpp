#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#define TEST1 0
#define TEST2 0

const std::regex condition_template("^Step ([A-Z]) must be finished before step ([A-Z]) can begin.$");

std::string GetInstructionsOrder(const std::map<char, int32_t> counts, std::unordered_map<char, std::string> rules) {
	std::string result = "", ready = "", done = "", tmp;
	std::unordered_map<char, std::string>::iterator itr;
	char item;

	for (auto it = counts.begin(); it != counts.end(); ++it) {
		itr = rules.find(it->first);
		if (itr == rules.end()) {
			ready += it->first;
		}
	}

	while (rules.size()) {
		char next;

		if (!ready.size()) {
			break;
		}

		std::sort(ready.begin(), ready.end());
		next = ready[0];

		for (auto it = rules.begin(); it != rules.end(); ++it) {
			std::size_t pos;

			item = it->first;
			tmp = it->second;
			pos = it->second.find(next);

			if (pos != std::string::npos) {
				if (it->second.size() == 1) {
					ready += it->first;
					it->second.clear();
					done += it->first;
				} else {
					it->second = it->second.substr(0, pos) + it->second.substr(pos + 1);
				}
			}
		}
		result += next;
		ready = ready.substr(1);

		for (uint32_t i = 0; i < done.size(); ++i) {
			rules.erase(done[i]);
		}

		done.clear();

		std::sort(ready.begin(), ready.end());
	}

	if (ready.size()) {
		std::sort(ready.begin(), ready.end());
		result.append(ready);
	}

	return result;
}

bool DecodeInstruction(const std::string line, std::unordered_map<char, std::string> &rules, std::map<char, int32_t> &counts) {
	std::smatch sm;
	char x, y;

	if (std::regex_match(line, sm, condition_template)) {
		x = sm.str(1)[0];
		y = sm.str(2)[0];
		counts[x]++;
		counts[y]++;
		rules[y] += x;
		return true;
	}
	return false;
}

int main(void) {
	int result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line, result1;
	std::unordered_map<char, std::string> rules;
	std::map<char, int32_t> counts;

	std::cout << "=== Advent of Code 2018 - day 7 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1.clear();
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

	result1 = GetInstructionsOrder(counts, rules);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
