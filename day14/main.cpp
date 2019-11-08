#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"

#define TEST 1

class AoC2018_day14 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::string simulate_recipes(uint64_t &result_rounds);

	std::string rounds_;
};

bool AoC2018_day14::init(std::vector<std::string> lines) {

	rounds_.clear();

	for (uint32_t i = 0; i < lines.size(); ++i) {
		rounds_.append(lines[i]);
	}

	return true;
}

std::string AoC2018_day14::simulate_recipes(uint64_t &result_rounds) {
	std::string recipes, recipes_score;
	uint64_t elf1, elf2, new_val;
	uint64_t required_rounds;
	bool part1_done = false, part2_done = false;

	required_rounds = std::stol(rounds_);
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
			if (recipes.size() >= (rounds_.size() + 2)) {
				size_t pos = recipes.find(rounds_, recipes.size() - rounds_.size() - 2);
				if (pos != std::string::npos) {
					result_rounds = pos;
					part2_done = true;
				}
			}
		}
	}

	return recipes_score;
}

int32_t AoC2018_day14::get_aoc_day() {
	return 14;
}

int32_t AoC2018_day14::get_aoc_year() {
	return 2018;
}

void AoC2018_day14::tests() {
#if TEST
	init({"9"});
	part1(); //5158916779

	init({"5"});
	part1(); //0124515891

	init({"18"});
	part1(); //9251071085

	init({"2018"});
	part1(); //5941429882



	init({"51589"});
	part1(); //9

	init({"01245"});
	part1(); //5

	init({"92510"});
	part1(); //18

	init({"59414"});
	part1(); //2018
#endif
}

bool AoC2018_day14::part1() {
	uint64_t result2;

	result1_ = simulate_recipes(result2);
	result2_ = std::to_string(result2);

	return true;
}

bool AoC2018_day14::part2() {
	return true;
}

int main(void) {
	AoC2018_day14 day14;

	return day14.main_execution();
}
