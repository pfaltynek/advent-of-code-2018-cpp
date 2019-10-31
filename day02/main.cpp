#include "./../common/aoc.hpp"
#include <unordered_map>

#define TEST 1
class AoC2018_day02 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool CompareIDs(const std::string &id1, const std::string &id2, std::string &common);
	void CheckID(const std::string &id, bool &two, bool &three);
	std::vector<std::string> ids_;
};

bool AoC2018_day02::init(const std::vector<std::string> lines) {
	ids_ = lines;

	return true;
}

int32_t AoC2018_day02::get_aoc_day() {
	return 2;
}

int32_t AoC2018_day02::get_aoc_year() {
	return 2018;
}

void AoC2018_day02::tests() {
#if TEST
	init({"abcdef", "bababc", "abbcde", "abcccd", "aabcdd", "abcdee", "ababab"});
	part1();

	init({"abcde", "fghij", "klmno", "pqrst", "fguij", "axcye", "wvxyz"});
	part2();
#endif
}

bool AoC2018_day02::part1() {
	uint32_t two = 0, three = 0;

	for (uint32_t i = 0; i < ids_.size(); ++i) {
		bool cnt2, cnt3;
		CheckID(ids_[i], cnt2, cnt3);
		if (cnt2) {
			two++;
		}
		if (cnt3) {
			three++;
		}
	}

	result1_ = std::to_string(two * three);

	return true;
}

bool AoC2018_day02::part2() {
	bool result2_found = false;

	for (uint32_t i = 0; i < ids_.size(); ++i) {
		for (uint32_t j = i + 1; j < ids_.size(); j++) {
			if (CompareIDs(ids_[i], ids_[j], result2_)) {
				return true;
			}
		}
	}

	return false;
}

void AoC2018_day02::CheckID(const std::string &id, bool &two, bool &three) {
	std::unordered_map<char, uint32_t> map;

	map.clear();
	for (unsigned int i = 0; i < id.size(); ++i) {
		map[id[i]]++;
	}

	two = false;
	three = false;

	for (std::unordered_map<char, uint32_t>::iterator it = map.begin(); it != map.end(); ++it) {
		if (it->second == 2) {
			two = true;
		}
		if (it->second == 3) {
			three = true;
		}
	}
}

bool AoC2018_day02::CompareIDs(const std::string &id1, const std::string &id2, std::string &common) {
	unsigned int i, idx, cnt;
	common.clear();

	if (id1.size() != id2.size()) {
		return false;
	}

	cnt = 0;

	for (i = 0; i < id1.size(); ++i) {
		if (id1[i] != id2[i]) {
			if (cnt) {
				return false;
			} else {
				cnt++;
				idx = i;
			}
		}
	}

	if (cnt) {
		common = id1.substr(0, idx) + id1.substr(idx + 1);
		return true;
	}

	return false;
}

int main(void) {
	AoC2018_day02 day02;

	return day02.main_execution();
}
