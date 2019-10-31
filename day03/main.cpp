#include "./../common/aoc.hpp"
#include <regex>
#include <unordered_map>

const uint32_t CLAIM_X = UINT32_MAX;

std::regex claim_template("^#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)$");

#define TEST 1
typedef struct claim {
	uint32_t id;
	uint32_t left;
	uint32_t top;
	uint32_t width;
	uint32_t height;
} claim_str;

class AoC2018_day03 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool parse_claim_data(const std::string claim_info, claim_str &claim);
	void fill_claim(const claim_str claim);
	bool check_claim(const claim_str claim);

	std::vector<claim_str> claims_;
	std::unordered_map<std::string, uint32_t> map_;
};

bool AoC2018_day03::init(const std::vector<std::string> lines) {
	claim_str claim;

	claims_.clear();
	map_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!parse_claim_data(lines[i], claim)) {
			std::cout << "Ivalid claim info on line " << i + 1 << std::endl;
			return false;
		} else {
			claims_.push_back(claim);
			fill_claim(claim);
		}
	}

	return true;
}

int32_t AoC2018_day03::get_aoc_day() {
	return 3;
}

int32_t AoC2018_day03::get_aoc_year() {
	return 2018;
}

void AoC2018_day03::tests() {
#if TEST
	init({"#1 @ 1,3: 4x4", "#2 @ 3,1: 4x4", "#3 @ 5,5: 2x2"});
#endif
}

bool AoC2018_day03::part1() {
	int32_t result = 0;

	for (auto it = map_.begin(); it != map_.end(); ++it) {
		if (it->second == UINT32_MAX) {
			result++;
		}
	}

	result1_ = std::to_string(result);

	return true;
}

bool AoC2018_day03::part2() {
	int32_t result = 0;

	for (unsigned int i = 0; i < claims_.size(); ++i) {
		if (check_claim(claims_[i])) {
			result = claims_[i].id;
			break;
		}
	}

	result2_ = std::to_string(result);

	return true;
}

bool AoC2018_day03::parse_claim_data(const std::string claim_info, claim_str &claim) {
	std::smatch sm;

	if (std::regex_match(claim_info, sm, claim_template)) {
		claim.id = stoi(sm.str(1));
		claim.left = stoi(sm.str(2));
		claim.top = stoi(sm.str(3));
		claim.width = stoi(sm.str(4));
		claim.height = stoi(sm.str(5));
		return true;
	}

	return false;
}

void AoC2018_day03::fill_claim(const claim_str claim) {
	for (unsigned int i = 0; i < claim.width; ++i) {
		for (unsigned int j = 0; j < claim.height; ++j) {
			std::string key = std::to_string(claim.left + i) + "x" + std::to_string(claim.top + j);

			if (map_[key]) {
				map_[key] = UINT32_MAX;
			} else {
				map_[key] = claim.id;
			}
		}
	}
}

bool AoC2018_day03::check_claim(const claim_str claim) {
	for (unsigned int i = 0; i < claim.width; ++i) {
		for (unsigned int j = 0; j < claim.height; ++j) {
			std::string key = std::to_string(claim.left + i) + "x" + std::to_string(claim.top + j);

			if (map_[key] != claim.id) {
				return false;
			}
		}
	}

	return true;
}

int main(void) {
	AoC2018_day03 day03;

	return day03.main_execution();
}
