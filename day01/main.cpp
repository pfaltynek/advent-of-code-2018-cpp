#include <fstream>
#include <iostream>
#include <vector>
//#include <string>

int main(void) {
	int result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::string captcha;
	std::vector<int32_t> freq_diffs;

	std::cout << "=== Advent of Code 2018 - day 1 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	freq_diffs.clear();
	while (std::getline(input, line)) {
		freq_diffs.push_back(std::stol(line, nullptr));
	}

	if (input.is_open()) {
		input.close();
	}

	for (unsigned int i = 0; i < freq_diffs.size(); ++i){
		result1 += freq_diffs[i];
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
