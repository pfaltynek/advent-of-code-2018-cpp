#include "./../common/aoc.hpp"
#include <regex>
#include <unordered_map>

#define TEST 1

const std::regex init_state_template("^initial state: ([#.]+)$");
const std::regex rule_template("^([#.]{5}) => ([#.])$");

class AoC2018_day12 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	void make_rules_comlete();
	int64_t get_plant_numbers_sum(const uint64_t generations);
	std::unordered_map<std::string, char> rules_;
	std::string init_state_;
};

bool AoC2018_day12::init(std::vector<std::string> lines) {
	std::smatch sm;
	bool init_ok = false, rule_ok = false;

	rules_.clear();
	init_state_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		switch (i) {
			case 0:
				if (std::regex_match(lines[i], sm, init_state_template)) {
					init_state_ = sm.str(1);
					init_ok = true;
				} else {
					std::cout << "invalid init state format" << std::endl;
					return false;
				}
				break;
			case 1:

				break;
			case 2:
			default:
				if (std::regex_match(lines[i], sm, rule_template)) {
					rules_[sm.str(1)] = sm.str(2)[0];
					rule_ok = true;
				} else {
					std::cout << "invalid rule format on line " << i + 1 << std::endl;
					return false;
				}
				break;
		}
	}

	if (init_ok && rule_ok) {
		make_rules_comlete();
		return true;
	}

	return false;
}

void AoC2018_day12::make_rules_comlete() {
	uint32_t i[5], j;
	std::string rule;

	for (i[0] = 0; i[0] < 2; i[0]++) {
		for (i[1] = 0; i[1] < 2; i[1]++) {
			for (i[2] = 0; i[2] < 2; i[2]++) {
				for (i[3] = 0; i[3] < 2; i[3]++) {
					for (i[4] = 0; i[4] < 2; i[4]++) {
						rule = "";
						for (j = 0; j < 5; ++j) {
							rule += i[j] ? '.' : '#';
						}

						if (rules_.find(rule) == rules_.end()) {
							rules_[rule] = '.';
						}
					}
				}
			}
		}
	}
}

int64_t AoC2018_day12::get_plant_numbers_sum(const uint64_t generations) {
	uint64_t zero_pot_idx = 0;
	std::string pots = init_state_, next;
	int64_t result = 0, prev_result = 0, diff = 0, prev_diff = 0, diff_cnt = 0, tip = 0;

	for (uint64_t i = 0; i < generations; ++i) {
		while (pots.substr(0, 5).compare(".....") != 0) {
			pots.insert(0, ".");
			zero_pot_idx++;
		}

		while (pots.substr(pots.size() - 5).compare(".....") != 0) {
			pots.append(".");
		}

		next = pots;

		for (uint64_t j = 2; j < (pots.size() - 2); ++j) {
			std::string part;
			char x;

			part = pots.substr(j - 2, 5);
			x = rules_.at(part);
			next[j] = x;
		}
		pots = next;

		prev_result = result;
		result = 0;

		for (uint64_t i = 0; i < pots.size(); ++i) {
			if (pots[i] == '#') {
				result += i - zero_pot_idx;
			}
		}
		prev_diff = diff;
		diff = result - prev_result;

		if (diff == prev_diff) {
			diff_cnt++;
			if ((diff_cnt >= 5) && !tip) {
				tip = result + ((generations - i - 1) * diff);
				return tip;
			}
		} else {
			diff_cnt = 0;
		}

		// std::cout << i << ": Sum: " << result << " Prev. sum: " << prev_result << " Diff: " << diff << " Tip: " << tip << std::endl;
	}

	return result;
}

int32_t AoC2018_day12::get_aoc_day() {
	return 12;
}

int32_t AoC2018_day12::get_aoc_year() {
	return 2018;
}

void AoC2018_day12::tests() {
#if TEST
	init({"initial state: #..#.#..##......###...###", "", "...## => #", "..#.. => #", ".#... => #", ".#.#. => #", ".#.## => #", ".##.. => #", ".#### => #",
		  "#.#.# => #", "#.### => #", "##.#. => #", "##.## => #", "###.. => #", "###.# => #", "####. => #"});

	part1();

	part2();

#endif
}

bool AoC2018_day12::part1() {
	int64_t result;

	result = get_plant_numbers_sum(20);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2018_day12::part2() {
	int64_t result;

	result = get_plant_numbers_sum(50000000000);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2018_day12 day12;

	return day12.main_execution();
}
