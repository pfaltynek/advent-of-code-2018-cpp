#include <fstream>
#include <iostream>
#include <vector>

#define TEST1 0
#define TEST2 0

std::string SimulateRecipes(uint64_t rounds) {
	std::string recipes;
	uint64_t result = 0, elf1, elf2, new_val;

	recipes.reserve(rounds + 11);

	recipes.clear();
	recipes.append("37");
	elf1 = 0;
	elf2 = 1;

	while (recipes.size() < (rounds + 10)) {
		new_val = recipes[elf1] - '0' + recipes[elf2] - '0';

		recipes.append(std::to_string(new_val));

		elf1 += 1 + recipes[elf1] - '0';
		elf1 = elf1 % recipes.size();
		elf2 += 1 + recipes[elf2] - '0';
		elf2 = elf2 % recipes.size();
	}

	return recipes.substr(rounds, 10);
}

int main(void) {
	int cnt = 0;
	std::string result1;
	uint64_t result2 = 0, rounds;
	std::ifstream input;
	std::string line, input_number;

	std::cout << "=== Advent of Code 2018 - day 14 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	result1 = SimulateRecipes(9);	// 5158916779
	result1 = SimulateRecipes(5);	// 0124515891
	result1 = SimulateRecipes(18);   // 9251071085
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
