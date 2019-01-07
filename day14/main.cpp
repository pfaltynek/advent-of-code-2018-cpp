#include <fstream>
#include <iostream>
#include <vector>

#define TEST1 0
#define TEST2 0

std::string SimulateRecipes(std::string rounds, uint64_t &result_rounds) {
	std::string recipes, recipes_score;
	uint64_t result = 0, elf1, elf2, new_val;
	uint64_t required_rounds;
	bool part1_done = false, part2_done = false;

	required_rounds = std::stol(rounds);
	recipes.reserve(required_rounds + 11);

	recipes.clear();
	recipes.append("37");
	elf1 = 0;
	elf2 = 1;

	while (!(part1_done && part2_done)) {
		new_val = recipes[elf1] - '0' + recipes[elf2] - '0';

		recipes.append(std::to_string(new_val));

		elf1 += 1 + recipes[elf1] - '0';
		elf1 = elf1 % recipes.size();
		elf2 += 1 + recipes[elf2] - '0';
		elf2 = elf2 % recipes.size();

		if (!part1_done) {
			if (recipes.size() >= (required_rounds + 10)) {
				recipes_score = recipes.substr(required_rounds, 10);
				part1_done = true;
			}
		}

		if (!part2_done) {
			if (recipes.size() >= (rounds.size() + 2)) {
				size_t pos = recipes.find(rounds, recipes.size() - rounds.size() - 2);
				if (pos != std::string::npos) {
					result_rounds = pos;
					part2_done = true;
				}
			}
		}
	}

	return recipes_score;
}

int main(void) {
	int cnt = 0;
	std::string result1;
	uint64_t result2 = 0;
	std::ifstream input;
	std::string line, input_number;

	std::cout << "=== Advent of Code 2018 - day 14 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	result1 = SimulateRecipes("9", "51589");	// 5158916779
	result1 = SimulateRecipes("5", "01245");	// 0124515891
	result1 = SimulateRecipes("18", "92510");   // 9251071085
	result1 = SimulateRecipes("2018", "59414"); // 5941429882

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

	if (input.is_open()) {
		input.close();
	}

	result1 = SimulateRecipes(input_number, result2);

#endif

	std::cout << "Result is " << result1 << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
