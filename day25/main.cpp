#include "./../common/aoc.hpp"
#include <regex>

#define TEST 1

const std::regex point_regex("^(-?\\d+),(-?\\d+),(-?\\d+),(-?\\d+)$");

typedef struct POINT {
	int32_t x, y, z, q;
	bool processed;

	int32_t get_distance() {
		return abs(x) + abs(y) + abs(z) + abs(q);
	}
	int32_t get_distance(POINT other) {
		return abs(x - other.x) + abs(y - other.y) + abs(z - other.z) + abs(q - other.q);
	}
} point_str;

class AoC2018_day25 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int32_t find_constellation();
	void check(point_str from);

	std::vector<point_str> points_;
};

bool AoC2018_day25::init(const std::vector<std::string> input) {
	std::smatch sm;
	point_str pt;

	points_.clear();

	for (uint32_t i = 0; i < input.size(); i++) {
		pt = {};

		pt.processed = false;

		if (input[i].empty()) {
			continue;
		}

		if (std::regex_match(input[i], sm, point_regex)) {
			pt.x = stoi(sm.str(1));
			pt.y = stoi(sm.str(2));
			pt.z = stoi(sm.str(3));
			pt.q = stoi(sm.str(4));
		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		points_.push_back(pt);
	}

	return true;
}

int32_t AoC2018_day25::find_constellation() {
	int32_t result = 0;

	for (uint32_t i = 0; i < points_.size(); i++) {
		if (!points_[i].processed) {
			points_[i].processed = true;
			check(points_[i]);
			result++;
		}
	}

	return result;
}

void AoC2018_day25::check(point_str from) {
	for (uint32_t i = 0; i < points_.size(); i++) {
		if (!points_[i].processed) {
			if (from.get_distance(points_[i]) <= 3) {
				points_[i].processed = true;
				check(points_[i]);
			}
		}
	}
}

int32_t AoC2018_day25::get_aoc_day() {
	return 25;
}

int32_t AoC2018_day25::get_aoc_year() {
	return 2018;
}

void AoC2018_day25::tests() {
#if TEST

	init({"0,0,0,0", "3,0,0,0", "0,3,0,0", "0,0,3,0", "0,0,0,3", "0,0,0,6", "9,0,0,0", "12,0,0,0"});

	part1(); // 2

	init({"0,0,0,0", "3,0,0,0", "0,3,0,0", "0,0,3,0", "0,0,0,3", "0,0,0,6", "9,0,0,0", "12,0,0,0", "6,0,0,0"});

	part1(); // 1

	init({"-1,2,2,0", "0,0,2,-2", "0,0,0,-2", "-1,2,0,0", "-2,-2,-2,2", "3,0,2,-1", "-1,3,2,2", "-1,0,-1,0", "0,2,1,-2", "3,0,0,0"});

	part1(); // 4

	init({"1,-1,0,1", "2,0,-1,0", "3,2,-1,0", "0,0,3,1", "0,0,-1,-1", "2,3,-2,0", "-2,2,0,0", "2,-2,0,-1", "1,-1,0,-1", "3,2,0,2"});

	part1(); // 3

	init({"1,-1,-1,-2", "-2,-2,0,1", "0,2,1,3", "-2,3,-2,1", "0,2,3,-2", "-1,-1,1,-2", "0,-2,-1,0", "-2,2,3,-1", "1,2,2,0", "-1,-2,0,-2"});

	part1(); // 8

#endif
}

bool AoC2018_day25::part1() {
	int32_t result1;

	result1 = find_constellation();

	result1_ = std::to_string(result1);

	return true;
}

bool AoC2018_day25::part2() {

	result2_.clear();

	return true;
}

int main(void) {
	AoC2018_day25 day25;

	return day25.main_execution();
}
