#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::string captcha;
	std::vector<int32_t> freq_diffs;
	std::unordered_set<int32_t> result_freq;
	bool result2_found = false;
	unsigned int i;

	std::cout << "=== Advent of Code 2018 - day 1 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	freq_diffs.clear();
	result_freq.clear();

	while (std::getline(input, line)) {
		freq_diffs.push_back(std::stol(line, nullptr));
	}

	if (input.is_open()) {
		input.close();
	}

	for (i = 0; i < freq_diffs.size(); ++i) {
		result1 += freq_diffs[i];
	}

	i = 0;
	while (!result2_found) {
		result2 += freq_diffs[i];
		if (result_freq.count(result2)) {
			result2_found = true;
			break;
		} else {
			result_freq.insert(result2);
		}
		i = (++i % freq_diffs.size());
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
