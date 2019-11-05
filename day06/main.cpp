#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <regex>
#include <string>
#include <vector>

#define TEST 1

const std::regex coord_template("(\\d+), (\\d+)");

class AoC2018_day06 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool decode_coord(const std::string line);
	void fill_closest_coordinate_map();
	int32_t get_region_size(const int32_t total_distance);
	int32_t get_largest_finite_area_size();
	int32_t get_total_distance();

	std::vector<coord_str> coords_;
	std::map<coord_str, coord_str> map_;
	coord_str min_, max_;
	bool in_test_;
	const int32_t extra_ = 100;
};

bool AoC2018_day06::decode_coord(const std::string line) {
	std::smatch sm;
	coord_str coord;

	if (std::regex_match(line, sm, coord_template)) {
		coord.x = stoi(sm.str(1));
		coord.y = stoi(sm.str(2));
		coords_.push_back(coord);

		if (coord.x > max_.x) {
			max_.x = coord.x;
		}
		if (coord.y > max_.y) {
			max_.y = coord.y;
		}
		if (coord.x < min_.x) {
			min_.x = coord.x;
		}
		if (coord.y < min_.y) {
			min_.y = coord.y;
		}

		return true;
	}
	return false;
}

bool AoC2018_day06::init(std::vector<std::string> lines) {
	max_.x = max_.y = INT32_MIN;
	min_.x = min_.y = INT32_MAX;

	coords_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!decode_coord(lines[i])) {
			std::cout << "Invalid coord at line " << i + 1 << std::endl;
			return false;
		}
	}

	in_test_ = false;

	return true;
}

void AoC2018_day06::fill_closest_coordinate_map() {
	int32_t j, k, j1, j2, k1, k2;
	uint32_t i;

	map_.clear();

	k1 = min_.y - extra_;
	k2 = max_.y + extra_;
	j1 = min_.x - extra_;
	j2 = max_.x + extra_;

	for (i = 0; i < coords_.size(); ++i) {
		for (j = j1; j <= j2; ++j) {
			for (k = k1; k <= k2; ++k) {
				int32_t distance = abs(coords_[i].x - j) + abs(coords_[i].y - k);
				coord_str coord;
				coord.x = j;
				coord.y = k;

				if (!i) {
					map_[coord].x = i + 1;
					map_[coord].y = distance;
				} else {
					if (map_[coord].y > distance) {
						map_[coord].x = i + 1;
						map_[coord].y = distance;
					} else if (map_[coord].y == distance) {
						map_[coord].x = 0;
					}
				}
			}
		}
	}
}

int32_t AoC2018_day06::get_region_size(const int32_t total_distance) {
	int32_t result = 0, j, k, k1, k2, j1, j2;
	uint32_t i;

	k1 = min_.y - extra_;
	k2 = max_.y + extra_;
	j1 = min_.x - extra_;
	j2 = max_.x + extra_;

	for (k = k1; k <= k2; ++k) {
		for (j = j1; j <= j2; ++j) {
			int32_t sum = 0;

			for (i = 0; i < coords_.size(); ++i) {
				sum += abs(k - coords_[i].y) + abs(j - coords_[i].x);
			}

			if (sum < total_distance) {
				result++;
			}
		}
	}

	return result;
}

int32_t AoC2018_day06::get_largest_finite_area_size() {
	int32_t result = 0, j, k, j1, j2, k1, k2;
	std::map<int32_t, coord_str> sizes;

	sizes.clear();

	k1 = min_.y - extra_;
	k2 = max_.y + extra_;
	j1 = min_.x - extra_;
	j2 = max_.x + extra_;

	for (k = k1; k <= k2; ++k) {
		for (j = j1; j <= j2; ++j) {
			coord_str coord;
			coord.x = j;
			coord.y = k;
			if (map_[coord].x) {
				sizes[map_[coord].x].x++;
			}
		}
	}

	k1 = min_.y - (extra_ / 2);
	k2 = max_.y + (extra_ / 2);
	j1 = min_.x - (extra_ / 2);
	j2 = max_.x + (extra_ / 2);

	for (k = k1; k <= k2; ++k) {
		for (j = j1; j <= j2; ++j) {
			coord_str coord;
			coord.x = j;
			coord.y = k;
			if (map_[coord].x) {
				sizes[map_[coord].x].y++;
			}
		}
	}

	for (auto it = sizes.begin(); it != sizes.end(); ++it) {
		if (it->second.x == it->second.y) {
			if (it->second.x > result) {
				result = it->second.x;
			}
		}
	}

	return result;
}

int32_t AoC2018_day06::get_aoc_day() {
	return 6;
}

int32_t AoC2018_day06::get_aoc_year() {
	return 2018;
}

void AoC2018_day06::tests() {
#if TEST
	init({"1, 1", "1, 6", "8, 3", "3, 4", "5, 5", "8, 9"});

	in_test_ = true;

	part1();

	part2();
	in_test_ = false;

#endif
}

int32_t AoC2018_day06::get_total_distance() {
	if (in_test_) {
		return 32;
	} else {
		return 10000;
	}
}

bool AoC2018_day06::part1() {
	int32_t result = 0;
	fill_closest_coordinate_map();

	result = get_largest_finite_area_size();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2018_day06::part2() {
	int32_t result = 0;

	result = get_region_size(get_total_distance());

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2018_day06 day06;

	return day06.main_execution();
}
