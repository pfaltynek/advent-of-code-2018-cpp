#include "main.hpp"

const std::regex regex_x("^x=(\\d+), y=(\\d+)..(\\d+)$");
const std::regex regex_y("^y=(\\d+), x=(\\d+)..(\\d+)$");

bool parse_line(const std::string line, std::map<coord_str, char>& scan, coord_str& min, coord_str& max) {
	std::smatch sm;
	int32_t x1, x2, y1, y2, tmp;

	if (std::regex_match(line, sm, regex_x)) {
		x1 = stoi(sm.str(1));
		y1 = stoi(sm.str(2));
		y2 = stoi(sm.str(3));
		x2 = x1;

		if (y2 < y1) {
			std::swap(y1, y2);
		}

		for (uint32_t i = y1; i <= y2; ++i) {
			scan[coord_str(x1, i)] = '#';
		}

	} else if (std::regex_match(line, sm, regex_y)) {
		y1 = stoi(sm.str(1));
		x1 = stoi(sm.str(2));
		x2 = stoi(sm.str(3));
		y2 = y1;

		if (x2 < x1) {
			std::swap(x1, x2);
		}

		for (uint32_t i = x1; i <= x2; ++i) {
			scan[coord_str(i, y1)] = '#';
		}

	} else {
		std::cout << "Invalid scan line '" << line << "'." << std::endl;
		return false;
	}

	min.x = std::min(min.x, x1);
	min.y = std::min(min.y, y1);
	max.x = std::max(max.x, x2);
	max.y = std::max(max.y, y2);

	return true;
}

bool init(const std::vector<std::string> input, const coord_str spring, std::map<coord_str, char>& scan, coord_str& min, coord_str& max) {
	int cnt = 0;
	std::vector<std::string> snippets, insts;
	bool inst_part = false;

	if (!input.size()) {
		std::cout << "Empty input." << std::endl;
		return false;
	}

	scan.clear();

	min.x = min.y = INT32_MAX;
	max.x = max.y = INT32_MIN;

	scan[spring] = '+';

	for (uint32_t i = 0; i < input.size(); ++i) {
		if (!parse_line(input[i], scan, min, max)) {
			return false;
		}
	}

	min.x--;
	max.x++;

	return true;
}

bool init(const coord_str spring, std::map<coord_str, char>& scan, coord_str& min, coord_str& max) {
	std::ifstream input;
	std::string line;
	std::vector<std::string> lines;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return false;
	}

	lines.clear();

	while (std::getline(input, line)) {
		lines.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}

	return init(lines, spring, scan, min, max);
}

char get_scan_symbol(const coord_str pos, std::map<coord_str, char>& scan) {
	if (!scan.count(pos)) {
		return '.';
	} else {
		return scan[pos];
	}
}

void print(std::map<coord_str, char>& main_scan, const coord_str min, const coord_str max) {
	for (uint32_t i = min.y; i <= max.y; ++i) {
		for (uint32_t j = min.x; j <= max.x; ++j) {
			coord_str coord(j, i);

			if (!main_scan.count(coord)) {
				std::cout << '.';
			} else {
				std::cout << main_scan[coord];
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

coord_str find_flood_end(const coord_str from, std::map<coord_str, char>& scan, const bool left_side, char& symbol) {
	coord_str result = from, tmp;

	for (;;) {
		result.x += left_side ? -1 : 1;
		symbol = get_scan_symbol(result, scan);
		switch (symbol) {
			case '.':
				tmp = result;
				tmp.y++;
				if (get_scan_symbol(tmp, scan) == '.') {
					return result;
				}
				break;
			case '#':
				return result;
				break;
			case '|':
				tmp = result;
				tmp.y++;
				if (get_scan_symbol(tmp, scan) == '|') {
					return result;
				}
				break;
			default:
				tmp = result;
				break;
		}
	}
}

void simulate_water_fall(const coord_str spring, std::map<coord_str, char>& scan, coord_str& min, coord_str& max) {
	coord_str curr = spring, left, right;
	std::queue<coord_str> fall;
	char lbound, rbound, sym;

	scan[curr] = '+';
	curr.y++;
	fall.push(curr);

	while (!fall.empty()) {
		curr = fall.front();
		fall.pop();

		if (curr.y > max.y) {
			continue;
		}

		sym = get_scan_symbol(curr, scan);

		if (sym == '.') {
			scan[curr] = '|';
			curr.y++;
			fall.push(curr);
		} else {
			switch (sym) {
				case '~':
				case '#':
					curr.y--;
					left = find_flood_end(curr, scan, true, lbound);
					right = find_flood_end(curr, scan, false, rbound);

					if ((lbound == '#') && (rbound == '#')) {
						for (uint32_t i = left.x + 1; i < right.x; ++i) {
							scan[coord_str(i, left.y)] = '~';
						}
						fall.push(curr);
					} else {
						if ((lbound != '#') && (lbound != '|')) {
							fall.push(left);
						}
						if ((rbound != '#') && (rbound != '|')) {
							fall.push(right);
						}
						for (uint32_t i = left.x + 1; i < right.x; ++i) {
							scan[coord_str(i, left.y)] = '|';
						}
					}
					break;
				case '|':
					scan[curr] = '|';
					break;
			}
		}
		// print(scan, min, max);
	}
}

void get_water_counts(std::map<coord_str, char>& scan, coord_str& min, coord_str& max, uint64_t& result1, uint64_t& result2) {
	result1 = result2 = 0;

	for (auto it = scan.begin(); it != scan.end(); it++) {
		if ((it->first.y >= min.y) && (it->first.y <= max.y)) {
			if ((it->second == '~') || (it->second == '|') || (it->second == '+')) {
				result1++;
				if (it->second == '~') {
					result2++;
				}
			}
		}
	}
}

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	std::map<coord_str, char> main_scan;
	coord_str min, max, spring(500, 0);

#if TEST
	if (!init({"x=495, y=2..7", "y=7, x=495..501", "x=501, y=3..7", "x=498, y=2..4", "x=506, y=1..2", "x=498, y=10..13", "x=504, y=10..13", "y=13, x=498..504"},
			  spring, main_scan, min, max)) {
		return -1;
	}

	print(main_scan, min, max);

	simulate_water_fall(spring, main_scan, min, max);

	result1 = part1(main_scan, min, max);

#endif

	if (!init(spring, main_scan, min, max)) {
		return -1;
	}

	// print(main_scan, min, max);

	std::cout << "=== Advent of Code 2018 - day 17 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	simulate_water_fall(spring, main_scan, min, max);

	get_water_counts(main_scan, min, max, result1, result2);

	// print(main_scan, min, max);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
