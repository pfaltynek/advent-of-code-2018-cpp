#include "./../common/aoc.hpp"
#include <unordered_set>

class AoC2018_day01 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<int32_t> freq_diffs_;
	std::unordered_set<int32_t> result_freq_;
};

bool AoC2018_day01::init(const std::vector<std::string> lines) {
	freq_diffs_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		freq_diffs_.push_back(std::stol(lines[i], nullptr));
	}

	return true;
}

int32_t AoC2018_day01::get_aoc_day() {
	return 1;
}

int32_t AoC2018_day01::get_aoc_year() {
	return 2018;
}

void AoC2018_day01::tests() {
}

bool AoC2018_day01::part1() {
	int32_t result = 0;

	for (uint32_t i = 0; i < freq_diffs_.size(); ++i) {
		result += freq_diffs_[i];
	}

	result1_ = std::to_string(result);

	return true;
}

bool AoC2018_day01::part2() {
	std::unordered_set<int32_t> result_freq;
	bool result2_found = false;
	uint32_t i = 0;
	int32_t result = 0;

	result_freq.clear();

	while (!result2_found) {
		result += freq_diffs_[i];
		if (result_freq.count(result)) {
			result2_found = true;
			break;
		} else {
			result_freq.insert(result);
		}
		i++;
		i = (i % freq_diffs_.size());
	}

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2018_day01 day01;

	return day01.main_execution();
}
