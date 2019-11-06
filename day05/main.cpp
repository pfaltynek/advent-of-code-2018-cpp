
#include "./../common/aoc.hpp"
#include <sstream>

#define TEST 1

class AoC2018_day05 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::string reduce_polymer_size(const std::string& polymer);
	std::string remove_unit_from_polymer(const std::string polymer, char unit);
	uint32_t get_shortest_polymer_length(const std::string polymer);

	std::string polymer_;
};

bool AoC2018_day05::init(std::vector<std::string> lines) {
	polymer_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		polymer_.append(lines[i]);
	}

	return true;
}

std::string AoC2018_day05::reduce_polymer_size(const std::string& polymer) {
	bool finished;
	std::stringstream r;
	std::string p;
	uint32_t i, j;

	if (polymer.size() <= 2) {
		return polymer;
	}

	p = polymer;
	r.str("");

	finished = false;

	while (!finished) {
		finished = true;
		i = 0;
		j = 1;
		while (j < p.size()) {
			if ((p[i] ^ p[j]) == 0x20) {
				finished = false;
				i = j + 1;
				j = i + 1;
			} else {
				r << p[i];
				i++;
				j++;
			}
		}
		if (i < p.size()) {
			r << p[i];
		}
		p = r.str();
		r.str("");
	}

	return p;
}

std::string AoC2018_day05::remove_unit_from_polymer(const std::string polymer, char unit) {
	std::stringstream ss;
	char unit2;
	uint32_t i;

	ss.str("");

	if (((unit >= 'a') && (unit <= 'z')) || ((unit >= 'A') && (unit <= 'Z'))) {
		unit2 = unit ^ 0x20;
	} else {
		return polymer;
	}

	i = 0;
	while (i < polymer.size()) {
		if ((polymer[i] != unit) && (polymer[i] != unit2)) {
			ss << polymer[i];
		}
		i++;
	}

	return ss.str();
}

uint32_t AoC2018_day05::get_shortest_polymer_length(const std::string polymer) {
	std::string p;
	uint32_t result;

	result = polymer.size();

	for (char c = 'a'; c <= 'z'; ++c) {
		p = remove_unit_from_polymer(polymer, c);

		p = reduce_polymer_size(p);
		if (p.size() < result) {
			result = p.size();
		}
	}

	return result;
}

int32_t AoC2018_day05::get_aoc_day() {
	return 5;
}

int32_t AoC2018_day05::get_aoc_year() {
	return 2018;
}

void AoC2018_day05::tests() {
#if TEST
	init({"dabAcCaCBAcCcaDA"});

	part1();

	part2();

#endif
}

bool AoC2018_day05::part1() {
	polymer_ = reduce_polymer_size(polymer_);

	result1_ = std::to_string(polymer_.size());

	return true;
}

bool AoC2018_day05::part2() {
	int32_t result = 0;

	result = get_shortest_polymer_length(polymer_);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2018_day05 day05;

	return day05.main_execution();
}
