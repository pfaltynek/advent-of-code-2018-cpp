#include "./../common/aoc.hpp"
#include <algorithm>

#define TEST 1

typedef struct ADJ_STATS {
	uint8_t trees;
	uint8_t open_ground;
	uint8_t lumberyard;
} adj_stats_str;

class AoC2018_day18 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint32_t get_data_index(const uint32_t x, const uint32_t y);
	void print(std::string title);
	adj_stats_str get_adjacents_stats(const uint32_t x, const uint32_t y, char& acre_current_type);
	uint32_t simulate(const uint32_t minutes);
	uint32_t get_total_resource_value();
	void reset();

	std::string data_, data_backup_;
	uint32_t input_size_ = 10;
};

void AoC2018_day18::reset() {
	data_ = data_backup_;
}

bool AoC2018_day18::init(const std::vector<std::string> lines) {
	input_size_ = lines.size();

	data_.clear();

	for (uint32_t i = 0; i < lines.size(); ++i) {
		if (lines[i].size() != input_size_) {
			std::cout << "Invalid input size at line " << i + 1 << std::endl;
			return false;
		}
		if (lines[i].find_first_not_of(".#|") != std::string::npos) {
			std::cout << "Invalid input content at line " << i + 1 << std::endl;
			return false;
		}

		data_.append(lines[i]);
	}
	data_backup_ = data_;

	return true;
}

void AoC2018_day18::print(std::string title) {

	if (!title.empty()) {
		std::cout << title << std::endl;
	}

	for (uint32_t i = 0; i < input_size_; ++i) {
		for (uint32_t j = 0; j < input_size_; ++j) {
			std::cout << data_[(i * input_size_) + j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

uint32_t AoC2018_day18::get_data_index(const uint32_t x, const uint32_t y) {
	return (y * input_size_) + x;
}

adj_stats_str AoC2018_day18::get_adjacents_stats(const uint32_t x, const uint32_t y, char& acre_current_type) {
	adj_stats_str result = {};

	uint32_t x1, x2, y1, y2;

	x1 = x2 = x;
	y1 = y2 = y;

	if (!x) {
		x2++;
	} else if (x + 1 == input_size_) {
		x1--;
	} else {
		x2++;
		x1--;
	}
	if (!y) {
		y2++;
	} else if (y + 1 == input_size_) {
		y1--;
	} else {
		y2++;
		y1--;
	}

	acre_current_type = data_[get_data_index(x, y)];

	for (uint32_t i = y1; i <= y2; ++i) {
		for (uint32_t j = x1; j <= x2; ++j) {
			if ((i == y) && (j == x)) {
				continue;
			}

			switch (data_[get_data_index(j, i)]) {
				case '.':
					result.open_ground++;
					break;
				case '#':
					result.lumberyard++;
					break;
				case '|':
					result.trees++;
					break;
			}
		}
	}

	return result;
}

uint32_t AoC2018_day18::get_total_resource_value() {
	uint32_t t, l;

	t = std::count(data_.begin(), data_.end(), '|');
	l = std::count(data_.begin(), data_.end(), '#');

	return t * l;
}

uint32_t AoC2018_day18::simulate(const uint32_t minutes) {
	std::string s2(data_);
	char c;
	adj_stats_str ass;
	std::vector<uint32_t> hashes;
	std::vector<uint32_t> rounds;
	uint32_t hash;
	int x = 0;

	hashes.clear();
	rounds.clear();

	hashes.push_back(get_total_resource_value());

	// print("Initial state:");

	for (uint32_t t = 0; t < minutes; ++t) {
		for (uint32_t i = 0; i < input_size_; ++i) {
			for (uint32_t j = 0; j < input_size_; ++j) {
				ass = get_adjacents_stats(j, i, c);

				switch (c) {
					case '.':
						if (ass.trees >= 3) {
							s2[get_data_index(j, i)] = '|';
						}
						break;
					case '|':
						if (ass.lumberyard >= 3) {
							s2[get_data_index(j, i)] = '#';
						}
						break;
					case '#':
						if (!(ass.lumberyard && ass.trees)) {
							s2[get_data_index(j, i)] = '.';
						}
						break;
				}
			}
		}
		data_ = s2;
		hash = get_total_resource_value();
		for (uint32_t h = x; h < hashes.size(); ++h) {
			if (hashes[h] == hash) {
				rounds.push_back(hash);
				rounds.push_back(x);
				rounds.push_back(h);
				rounds.push_back(t + 1);
				x = h + 1;
				if (rounds.size() == 4 * 3) {
					uint32_t diff = rounds[3] - rounds[2];
					bool ok = true;
					for (uint32_t r = 1; r < (rounds.size() / 4); ++r) {
						if ((rounds[(r * 4) + 3] - rounds[(r * 4) + 2]) != diff) {
							ok = false;
							break;
						}
					}
					if (ok) {
						while (t < minutes) {
							t += diff;
						}
						t -= diff;
						x = t;
					} else {
						rounds.clear();
					}
				}
				break;
			}
		}
		hashes.push_back(hash);

		// print("After " + std::to_string(t + 1) + (t ? " minutes:" : " minute:"));
	}

	return hash;
}

int32_t AoC2018_day18::get_aoc_day() {
	return 18;
}

int32_t AoC2018_day18::get_aoc_year() {
	return 2018;
}

void AoC2018_day18::tests() {
#if TEST
	uint64_t result1;

	if (init({".#.#...|#.", ".....#|##|", ".|..|...#.", "..|#.....#", "#.#|||#|#|", "...#.||...", ".|....|...", "||...#|.#|", "|.||||..|.", "...#.|..|."})) {
		result1 = simulate(10); // 1147
	}

#endif
}

bool AoC2018_day18::part1() {
	uint64_t result1;

	result1 = simulate(10);
	result1_ = std::to_string(result1);

	return true;
}

bool AoC2018_day18::part2() {
	uint64_t result2;

	reset();
	result2 = simulate(1000000000);
	result2_ = std::to_string(result2);

	return true;
}

int main(void) {
	AoC2018_day18 day17;

	return day17.main_execution();
}
