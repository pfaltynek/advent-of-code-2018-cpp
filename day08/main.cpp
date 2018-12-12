#include <fstream>
#include <iostream>
#include <sstream>

#define TEST1 0
#define TEST2 0

void GetMetadataSum(std::stringstream& num, int32_t &result) {
	int32_t children, metadata_size, metadata;

	num >> children;
	num >> metadata_size;

	for (int32_t i = 0; i < children;++i){
		GetMetadataSum(num, result);
	}

	for (int32_t i = 0; i < metadata_size ;++i) {
		num >> metadata;
		result += metadata;
	}
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
		// if (!DecodeInstruction(line, rules, counts)) {
		//	std::cout << "Invalid instruction at line " << cnt << std::endl;
		//}
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	num.str(numbers);

	GetMetadataSum(num, result1);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
