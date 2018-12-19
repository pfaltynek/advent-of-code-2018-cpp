#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

#define TEST1 0
#define TEST2 0

const std::regex init_state_template("^initial state: ([#.]+)$");
const std::regex rule_template("^([#.]{5}) => ([#.])$");

bool ParseInput(const std::vector<std::string> input, std::string &init_state, std::unordered_map<std::string, char> &rules) {
	std::smatch sm;
	bool init_ok = false, rule_ok = false;

	rules.clear();

	for (uint32_t i = 0; i < input.size(); ++i) {
		switch (i) {
			case 0:
				if (std::regex_match(input[i], sm, init_state_template)) {
					init_state = sm.str(1);
					init_ok = true;
				} else {
					std::cout << "invalid init state format" << std::endl;
					return false;
				}
				break;
			case 1:

				break;
			case 2:
			default:
				if (std::regex_match(input[i], sm, rule_template)) {
					rules[sm.str(1)] = sm.str(2)[0];
					rule_ok = true;
				} else {
					std::cout << "invalid rule format on line " << i + 1 << std::endl;
					return false;
				}
				break;
		}
	}

	return (init_ok && rule_ok);
}

void MakeRulesComlete(std::unordered_map<std::string, char> &rules) {
	uint32_t i[5], j;
	std::string rule;

	for (i[0] = 0; i[0] < 2; i[0]++) {
		for (i[1] = 0; i[1] < 2; i[1]++) {
			for (i[2] = 0; i[2] < 2; i[2]++) {
				for (i[3] = 0; i[3] < 2; i[3]++) {
					for (i[4] = 0; i[4] < 2; i[4]++) {
						rule = "";
						for (j = 0; j < 5; ++j) {
							rule += i[j] ? '.' : '#';
						}

						if (rules.find(rule) == rules.end()) {
							rules[rule] = '.';
						}
					}
				}
			}
		}
	}
}

int64_t GetPlantNumbersSum(const std::string init_state, const std::unordered_map<std::string, char> rules, const uint64_t generations) {
	uint64_t zero_pot_idx = 0;
	std::string pots = init_state, next;
	int64_t result = 0, prev_result = 0, diff = 0, prev_diff = 0, diff_cnt = 0, tip = 0;

	for (uint64_t i = 0; i < generations; ++i) {
		while (pots.substr(0, 5).compare(".....") != 0) {
			pots.insert(0, ".");
			zero_pot_idx++;
		}

		while (pots.substr(pots.size() - 5).compare(".....") != 0) {
			pots.append(".");
		}

		next = pots;

		for (uint64_t j = 2; j < (pots.size() - 2); ++j) {
			std::string part;
			char x;

			part = pots.substr(j - 2, 5);
			x = rules.at(part);
			next[j] = x;
		}
		pots = next;

		prev_result = result;
		result = 0;

		for (uint64_t i = 0; i < pots.size(); ++i) {
			if (pots[i] == '#') {
				result += i - zero_pot_idx;
			}
		}
		prev_diff = diff;
		diff = result - prev_result;

		if (diff == prev_diff) {
			diff_cnt++;
			if ((diff_cnt >= 5) && !tip) {
				tip = result + ((generations - i - 1) * diff);
				return tip;
			}
		} else {
			diff_cnt = 0;
		}

		//std::cout << i << ": Sum: " << result << " Prev. sum: " << prev_result << " Diff: " << diff << " Tip: " << tip << std::endl;
	}

	return result;
}

int main(void) {
	int cnt = 0;
	int64_t result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::string> puzzle_input;
	std::string init_state;
	std::unordered_map<std::string, char> rules;

	puzzle_input.clear();
	init_state.clear();
	rules.clear();

	std::cout << "=== Advent of Code 2018 - day 12 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	puzzle_input.push_back("initial state: #..#.#..##......###...###");
	puzzle_input.push_back("");
	puzzle_input.push_back("...## => #");
	puzzle_input.push_back("..#.. => #");
	puzzle_input.push_back(".#... => #");
	puzzle_input.push_back(".#.#. => #");
	puzzle_input.push_back(".#.## => #");
	puzzle_input.push_back(".##.. => #");
	puzzle_input.push_back(".#### => #");
	puzzle_input.push_back("#.#.# => #");
	puzzle_input.push_back("#.### => #");
	puzzle_input.push_back("##.#. => #");
	puzzle_input.push_back("##.## => #");
	puzzle_input.push_back("###.. => #");
	puzzle_input.push_back("###.# => #");
	puzzle_input.push_back("####. => #");

	if (ParseInput(puzzle_input, init_state, rules)) {
		MakeRulesComlete(rules);
	}

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
		puzzle_input.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}

	if (ParseInput(puzzle_input, init_state, rules)) {
		MakeRulesComlete(rules);
	}

#endif

	result1 = GetPlantNumbersSum(init_state, rules, 20);

	std::cout << "Result is " << result1 << std::endl;

	result2 = GetPlantNumbersSum(init_state, rules, 50000000000);

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
