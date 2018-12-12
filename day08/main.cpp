#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define TEST1 0
#define TEST2 0

void GetMetadataSum(std::stringstream &num, int32_t &result1, int32_t &result2) {
	int32_t children, metadata_size, metadata;
	int32_t value = 0, r = 0;

	num >> children;
	num >> metadata_size;

	if (children) {
		std::vector<int32_t> children_values(children);

		for (int32_t i = 0; i < children; ++i) {
			r = 0;
			GetMetadataSum(num, result1, r);
			children_values[i] = r;
		}
		for (int32_t i = 0; i < metadata_size; ++i) {
			num >> metadata;
			value += metadata;

			if (metadata && (metadata <= children)) {
				result2 += children_values[metadata - 1];
			}
		}
	} else {
		for (int32_t i = 0; i < metadata_size; ++i) {
			num >> metadata;
			value += metadata;
		}

		result2 = value;
	}

	result1 += value;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line, numbers;
	std::stringstream num;

	std::cout << "=== Advent of Code 2018 - day 8 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	numbers.clear();

#if TEST1
	numbers = "2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2";
#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
		numbers.append(line);
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	num.str(numbers);

	GetMetadataSum(num, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
