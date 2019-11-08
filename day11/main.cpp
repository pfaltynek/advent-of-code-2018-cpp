#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"

#define TEST 0

class AoC2018_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int32_t get_power_level(const coord_str coord);
	void get_largest_total_power(const int32_t grid_size, const int32_t area_size, coord_str &result1, coord_str &result2, int32_t &result_size);

	int32_t grid_sn_;
};

bool AoC2018_day11::init(std::vector<std::string> lines) {

	for (uint32_t i = 0; i < lines.size(); i++) {
		grid_sn_ = stoi(lines[i]);
	}

	return true;
}

int32_t AoC2018_day11::get_power_level(const coord_str coord) {
	int32_t rack_id, result;

	rack_id = coord.x + 10;
	result = rack_id * coord.y;
	result += grid_sn_;
	result *= rack_id;
	result = ((result / 100) % 10) - 5;

	return result;
}

void AoC2018_day11::get_largest_total_power(const int32_t grid_size, const int32_t area_size, coord_str &result1, coord_str &result2, int32_t &result_size) {
	int32_t grid[grid_size][grid_size][3];
	int32_t max1 = INT32_MIN, max2 = INT32_MIN, sum;
	int32_t i, j, i1, j1, s, idx1, idx2;

	result1 = result2 = {};
	result_size = 0;

	for (i = 0; i < grid_size; ++i) {
		for (j = 0; j < grid_size; ++j) {
			sum = get_power_level({i + 1, j + 1});
			grid[i][j][0] = sum;
			if (sum > max2) {
				max2 = sum;
				result2.x = i + 1;
				result2.y = j + 1;
				result_size = 1;
			}
		}
	}

	idx1 = 0;
	idx2 = 1;

	for (s = 2; s <= grid_size; ++s) {
		for (i = 0; i < (grid_size - s + 1); ++i) {
			for (j = 0; j < (grid_size - s + 1); ++j) {
				sum = grid[i][j][idx1];
				for (i1 = 0; i1 < s - 1; ++i1) {
					sum += grid[i + i1][j + s - 1][0];
				}
				for (j1 = 0; j1 < s - 1; ++j1) {
					sum += grid[i + i1][j + j1][0];
				}

				sum += grid[i + s - 1][j + s - 1][0];
				
				grid[i][j][idx2] = sum;

				if ((s == area_size) && (sum > max1)) {
					max1 = sum;
					result1.x = i + 1;
					result1.y = j + 1;
				}

				if (sum > max2) {
					max2 = sum;
					result2.x = i + 1;
					result2.y = j + 1;
					result_size = s;
				}
			}
		}

		if (s == 2) {
			idx1 = 1;
			idx2 = 2;
		} else {
			std::swap(idx1, idx2);
		}
	}
}

int32_t AoC2018_day11::get_aoc_day() {
	return 11;
}

int32_t AoC2018_day11::get_aoc_year() {
	return 2018;
}

void AoC2018_day11::tests() {
#if TEST
	int32_t test;
	coord_str result1, result2;

	init({"8"});
	test = get_power_level({3, 5}); // 4
	init({"57"});
	test = get_power_level({122, 79}); // -5
	init({"39"});
	test = get_power_level({217, 196}); // 0
	init({"71"});
	test = get_power_level({101, 153}); // 4

	init({"18"});
	get_largest_total_power(300, 3, result1, result2, test); // {33,45},{90,269},16
	init({"42"});
	get_largest_total_power(300, 3, result1, result2, test); // {21,61},{232,251},12
#endif
}

bool AoC2018_day11::part1() {
	int32_t size;
	coord_str result1, result2;

	get_largest_total_power(300, 3, result1, result2, size);

	result1_ = std::to_string(result1.x) + "," + std::to_string(result1.y);

	result2_ = std::to_string(result2.x) + "," + std::to_string(result2.y) + "," + std::to_string(size);

	return true;
}

bool AoC2018_day11::part2() {
	return true;
}

int main(void) {
	AoC2018_day11 day11;

	return day11.main_execution();
}
