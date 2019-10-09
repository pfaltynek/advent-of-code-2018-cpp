#include "main.hpp"

int32_t input_size = 10;

typedef struct ADJ_STATS {
	uint8_t trees;
	uint8_t open_ground;
	uint8_t lumberyard;
} adj_stats_str;

bool init(const std::vector<std::string> input, std::string& data) {
	input_size = input.size();

	data.clear();

	for (uint32_t i = 0; i < input.size(); ++i) {
		if (input[i].size() != input_size) {
			std::cout << "Invalid input size al line " << i + 1 << std::endl;
			return false;
		}
		if (input[i].find_first_not_of(".#|") != std::string::npos) {
			std::cout << "Invalid input content al line " << i + 1 << std::endl;
			return false;
		}

		data.append(input[i]);
	}

	return true;
}

bool init(std::string& data) {
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

	return init(lines, data);
}

void print(std::string title, std::string data) {

	if (!title.empty()) {
		std::cout << title << std::endl;
	}

	for (uint32_t i = 0; i < input_size; ++i) {
		for (uint32_t j = 0; j < input_size; ++j) {
			std::cout << data[(i * input_size) + j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

uint32_t get_data_index(const uint32_t x, const uint32_t y) {
	return (y * input_size) + x;
}

adj_stats_str get_adjacents_stats(const std::string data, const uint8_t x, const uint8_t y, char& acre_current_type) {
	adj_stats_str result = {};

	uint8_t x1, x2, y1, y2;

	x1 = x2 = x;
	y1 = y2 = y;

	if (!x) {
		x2++;
	} else if ((x + 1) == input_size) {
		x1--;
	} else {
		x2++;
		x1--;
	}
	if (!y) {
		y2++;
	} else if ((y + 1) == input_size) {
		y1--;
	} else {
		y2++;
		y1--;
	}

	acre_current_type = data[get_data_index(x, y)];

	for (uint32_t i = y1; i <= y2; ++i) {
		for (uint32_t j = x1; j <= x2; ++j) {
			if ((i == y) && (j == x)) {
				continue;
			}

			switch (data[get_data_index(j, i)]) {
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

uint32_t get_total_resource_value(const std::string data) {
	uint32_t t, l;

	t = std::count(data.begin(), data.end(), '|');
	l = std::count(data.begin(), data.end(), '#');

	return t * l;
}

uint32_t simulate(const std::string initial, uint32_t minutes) {
	std::string s1(initial), s2(initial);
	char c;
	adj_stats_str ass;
	std::vector<uint32_t> hashes;
	std::vector<uint32_t> rounds;
	uint32_t hash, offset = 0, repeated = 0, cnt = 0;
	int x = 0;

	hashes.clear();
	rounds.clear();

	hashes.push_back(get_total_resource_value(s1));

	// print("Initial state:", s1);

	for (uint32_t t = 0; t < minutes; ++t) {
		for (uint32_t i = 0; i < input_size; ++i) {
			for (uint32_t j = 0; j < input_size; ++j) {
				ass = get_adjacents_stats(s1, j, i, c);

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
		s1 = s2;
		hash = get_total_resource_value(s1);
		for (uint32_t h = x; h < hashes.size(); ++h) {
			if (hashes[h] == hash) {
				rounds.push_back(hash);
				rounds.push_back(x);
				rounds.push_back(h);
				rounds.push_back(t + 1);
				x = h + 1;
				if (rounds.size() == 4 * 3) {
					int32_t diff = rounds[3] - rounds[2];
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

		// print("After " + std::to_string(t + 1) + (t ? " minutes:" : " minute:"), s1);
	}

	return hash;
}

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	std::string data;

#if TEST
	if (!init({".#.#...|#.", ".....#|##|", ".|..|...#.", "..|#.....#", "#.#|||#|#|", "...#.||...", ".|....|...", "||...#|.#|", "|.||||..|.", "...#.|..|."},
			  data)) {
		return -1;
	}

	final = simulate(data, 10);

	result1 = get_part1_value(final);

#endif

	if (!init(data)) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 19 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = simulate(data, 10);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = simulate(data, 1000000000);

	std::cout << "Result is " << result2 << std::endl;
}
