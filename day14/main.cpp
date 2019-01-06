#include <fstream>
#include <iostream>
#include <vector>

#define TEST1 0
#define TEST2 0

uint64_t SimulateRecipes(uint64_t rounds) {
	std::vector<uint8_t> recipes;
	uint64_t result = 0, elf1, elf2, new_val;

	recipes.clear();
	recipes.push_back(3);
	recipes.push_back(7);
	elf1 = 0;
	elf2 = 1;

	while (recipes.size() < (rounds + 10)) {
		new_val = recipes[elf1] + recipes[elf2];

		if (new_val >= 10) {
			recipes.push_back(new_val / 10);
			recipes.push_back(new_val % 10);
		} else {
			recipes.push_back(new_val);
		}

		elf1 += 1 + recipes[elf1];
		elf1 = elf1 % recipes.size();
		elf2 += 1 + recipes[elf2];
		elf2 = elf2 % recipes.size();
	}

	for (uint32_t i = 0; i < 10; ++i){
		result *= 10;
		result += recipes[rounds + i];
	}

	return result;
}

int main(void) {
	int cnt = 0;
	uint64_t result1 = 0, result2 = 0, rounds;
	std::ifstream input;
	std::string line, input_number;

	std::cout << "=== Advent of Code 2018 - day 14 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	result1 = SimulateRecipes(9); // 5158916779
	result1 = SimulateRecipes(5); // 0124515891
	result1 = SimulateRecipes(18); // 9251071085
	result1 = SimulateRecipes(2018); // 5941429882

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	input_number.clear();

	while (std::getline(input, line)) {
		input_number.append(line);
		cnt++;
	}

	rounds = std::stol(input_number);

	if (input.is_open()) {
		input.close();
	}

	result1 = SimulateRecipes(rounds);

#endif

	std::cout << "Result is " << result1 << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
