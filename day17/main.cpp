#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <queue>
#include <regex>

const std::regex regex_x("^x=(\\d+), y=(\\d+)..(\\d+)$");
const std::regex regex_y("^y=(\\d+), x=(\\d+)..(\\d+)$");

#define TEST 1

class AoC2018_day17 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	void get_water_counts(uint64_t& result1, uint64_t& result2);
	void simulate_water_fall();
	coord_str find_flood_end(const coord_str from, bool left_side, char& symbol);
	bool parse_line(const std::string line);
	char get_scan_symbol(const coord_str pos);
	void print();

	std::map<coord_str, char> main_scan_;
	coord_str min_, max_, spring_;
};

bool AoC2018_day17::parse_line(const std::string line) {
	std::smatch sm;
	int32_t x1, x2, y1, y2;

	if (std::regex_match(line, sm, regex_x)) {
		x1 = stoi(sm.str(1));
		y1 = stoi(sm.str(2));
		y2 = stoi(sm.str(3));
		x2 = x1;

		if (y2 < y1) {
			std::swap(y1, y2);
		}

		for (int32_t i = y1; i <= y2; ++i) {
			main_scan_[coord_str(x1, i)] = '#';
		}

	} else if (std::regex_match(line, sm, regex_y)) {
		y1 = stoi(sm.str(1));
		x1 = stoi(sm.str(2));
		x2 = stoi(sm.str(3));
		y2 = y1;

		if (x2 < x1) {
			std::swap(x1, x2);
		}

		for (int32_t i = x1; i <= x2; ++i) {
			main_scan_[coord_str(i, y1)] = '#';
		}

	} else {
		std::cout << "Invalid scan line '" << line << "'." << std::endl;
		return false;
	}

	min_.x = std::min(min_.x, x1);
	min_.y = std::min(min_.y, y1);
	max_.x = std::max(max_.x, x2);
	max_.y = std::max(max_.y, y2);

	return true;
}

bool AoC2018_day17::init(const std::vector<std::string> lines) {
	if (!lines.size()) {
		std::cout << "Empty input." << std::endl;
		return false;
	}

	main_scan_.clear();
	spring_ = {500, 0};

	min_.x = min_.y = INT32_MAX;
	max_.x = max_.y = INT32_MIN;

	main_scan_[spring_] = '+';

	for (uint32_t i = 0; i < lines.size(); ++i) {
		if (!parse_line(lines[i])) {
			return false;
		}
	}

	min_.x--;
	max_.x++;

	return true;
}

char AoC2018_day17::get_scan_symbol(const coord_str pos) {
	if (!main_scan_.count(pos)) {
		return '.';
	} else {
		return main_scan_[pos];
	}
}

void AoC2018_day17::print() {
	for (int32_t i = min_.y; i <= max_.y; ++i) {
		for (int32_t j = min_.x; j <= max_.x; ++j) {
			coord_str coord(j, i);

			if (!main_scan_.count(coord)) {
				std::cout << '.';
			} else {
				std::cout << main_scan_[coord];
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

coord_str AoC2018_day17::find_flood_end(const coord_str from, bool left_side, char& symbol) {
	coord_str result = from, tmp;

	for (;;) {
		result.x += left_side ? -1 : 1;
		symbol = get_scan_symbol(result);
		switch (symbol) {
			case '.':
				tmp = result;
				tmp.y++;
				if (get_scan_symbol(tmp) == '.') {
					return result;
				}
				break;
			case '#':
				return result;
				break;
			case '|':
				tmp = result;
				tmp.y++;
				if (get_scan_symbol(tmp) == '|') {
					return result;
				}
				break;
			default:
				tmp = result;
				break;
		}
	}
}

void AoC2018_day17::simulate_water_fall() {
	coord_str curr = spring_, left, right;
	std::queue<coord_str> fall;
	char lbound, rbound, sym;

	main_scan_[curr] = '+';
	curr.y++;
	fall.push(curr);

	while (!fall.empty()) {
		curr = fall.front();
		fall.pop();

		if (curr.y > max_.y) {
			continue;
		}

		sym = get_scan_symbol(curr);

		if (sym == '.') {
			main_scan_[curr] = '|';
			curr.y++;
			fall.push(curr);
		} else {
			switch (sym) {
				case '~':
				case '#':
					curr.y--;
					left = find_flood_end(curr, true, lbound);
					right = find_flood_end(curr, false, rbound);

					if ((lbound == '#') && (rbound == '#')) {
						for (int32_t i = left.x + 1; i < right.x; ++i) {
							main_scan_[coord_str(i, left.y)] = '~';
						}
						fall.push(curr);
					} else {
						if ((lbound != '#') && (lbound != '|')) {
							fall.push(left);
						}
						if ((rbound != '#') && (rbound != '|')) {
							fall.push(right);
						}
						for (int32_t i = left.x + 1; i < right.x; ++i) {
							main_scan_[coord_str(i, left.y)] = '|';
						}
					}
					break;
				case '|':
					main_scan_[curr] = '|';
					break;
			}
		}
		// print(scan, min, max);
	}
}

void AoC2018_day17::get_water_counts(uint64_t& result1, uint64_t& result2) {
	result1 = result2 = 0;

	for (auto it = main_scan_.begin(); it != main_scan_.end(); it++) {
		if ((it->first.y >= min_.y) && (it->first.y <= max_.y)) {
			if ((it->second == '~') || (it->second == '|') || (it->second == '+')) {
				result1++;
				if (it->second == '~') {
					result2++;
				}
			}
		}
	}
}

int32_t AoC2018_day17::get_aoc_day() {
	return 17;
}

int32_t AoC2018_day17::get_aoc_year() {
	return 2018;
}

void AoC2018_day17::tests() {
#if TEST
	uint64_t result1, result2;

	if (init({"x=495, y=2..7", "y=7, x=495..501", "x=501, y=3..7", "x=498, y=2..4", "x=506, y=1..2", "x=498, y=10..13", "x=504, y=10..13",
			  "y=13, x=498..504"})) {
		//print();

		simulate_water_fall();

		get_water_counts(result1, result2);
	}

#endif
}

bool AoC2018_day17::part1() {
	uint64_t result1, result2;

	simulate_water_fall();

	get_water_counts(result1, result2);
	result1_ = std::to_string(result1);
	result2_ = std::to_string(result2);

	return true;
}

bool AoC2018_day17::part2() {
	return true;
}

int main(void) {
	AoC2018_day17 day17;

	return day17.main_execution();
}
