#include "./../common/aoc.hpp"
#include <sstream>
#include <vector>

#define TEST 1

class AoC2018_day08 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	void get_metadata_sum(std::stringstream &num, int32_t &result1, int32_t &result2);
	std::string numbers_;
};

void AoC2018_day08::get_metadata_sum(std::stringstream &num, int32_t &result1, int32_t &result2) {
	int32_t children, metadata_size, metadata;
	int32_t value = 0, r = 0;

	num >> children;
	num >> metadata_size;

	if (children) {
		std::vector<int32_t> children_values(children);

		for (int32_t i = 0; i < children; ++i) {
			r = 0;
			get_metadata_sum(num, result1, r);
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

bool AoC2018_day08::init(std::vector<std::string> lines) {

	numbers_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		numbers_.append(lines[i]);
	}


	return true;
}

int32_t AoC2018_day08::get_aoc_day() {
	return 8;
}

int32_t AoC2018_day08::get_aoc_year() {
	return 2018;
}

void AoC2018_day08::tests() {
#if TEST
	init({"2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2"});

	part1();

	part2();

#endif
}

bool AoC2018_day08::part1() {
	int32_t result1 = 0, result2 = 0;
	std::stringstream num;

	num.str(numbers_);

	get_metadata_sum(num, result1, result2);

	result1_ = std::to_string(result1);
	result2_ = std::to_string(result2);

	return true;
}

bool AoC2018_day08::part2() {

	return true;
}

int main(void) {
	AoC2018_day08 day08;

	return day08.main_execution();
}
