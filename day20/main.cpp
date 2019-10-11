#include "main.hpp"

bool init(std::string& regex) {
	std::ifstream input;
	std::string line;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return false;
	}

	regex.clear();

	while (std::getline(input, line)) {
		regex.append(line);
	}

	if (input.is_open()) {
		input.close();
	}

	return true;
}

bool decode_map(const std::string regex, std::map<coord_str, char>& map, coord_str& min, coord_str& max) {
	coord_str coord, new_coord;
	std::stack<coord_str> stack;

	if (regex.size() < 3) {
		std::cout << "Invalid input" << std::endl;
		return false;
	}
	if ((regex[0] != '^') || (regex[regex.size() - 1] != '$')) {
		std::cout << "Invalid input start/end" << std::endl;
		return false;
	}

	map.clear();
	coord = {};
	while (!stack.empty()) {
		stack.pop();
	}

	min = max = coord;
	map[coord] = 'X';

	for (uint32_t i = 1; i < regex.size() - 1; ++i) {
		switch (regex[i]) {
			case 'N':
				new_coord = coord;
				new_coord.y++;
				map[new_coord] = '-';
				new_coord.y++;
				map[new_coord] = '.';
				coord = new_coord;
				break;
			case 'W':
				new_coord = coord;
				new_coord.x--;
				map[new_coord] = '|';
				new_coord.x--;
				map[new_coord] = '.';
				coord = new_coord;
				break;
			case 'S':
				new_coord = coord;
				new_coord.y--;
				map[new_coord] = '-';
				new_coord.y--;
				map[new_coord] = '.';
				coord = new_coord;
				break;
			case 'E':
				new_coord = coord;
				new_coord.x++;
				map[new_coord] = '|';
				new_coord.x++;
				map[new_coord] = '.';
				coord = new_coord;
				break;
			case '(':
				stack.push(coord);
				break;
			case ')':
				if (stack.empty()) {
					std::cout << "Invalid branch in regex at " << i << "" << std::endl;
				} else {
					coord = stack.top();
					stack.pop();
				}
				break;
			case '|':
				if (stack.empty()) {
					std::cout << "Invalid branch in regex at " << i << "" << std::endl;
				} else {
					coord = stack.top();
				}
				break;
			default:
				std::cout << "Invalid char '" << regex[i] << "' in regex" << std::endl;
				break;
		}
		if (new_coord.x < min.x) {
			min.x = new_coord.x;
		}
		if (new_coord.y < min.y) {
			min.y = new_coord.y;
		}
		if (new_coord.x > max.x) {
			max.x = new_coord.x;
		}
		if (new_coord.y > max.y) {
			max.y = new_coord.y;
		}
	}

	return true;
}

void print(std::map<coord_str, char>& map, const coord_str min, const coord_str max) {
	for (int32_t i = max.y + 1; i >= min.y - 1; --i) {
		for (int32_t j = min.x - 1; j <= max.x + 1; ++j) {
			coord_str coord = {j, i};
			if (!map.count(coord)) {
				std::cout << '#';
			} else {
				std::cout << map[coord];
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	std::string regex;
	std::map<coord_str, char> map;
	coord_str min, max;

#if TEST

	regex = "^ENWWW(NEEE|SSE(EE|N))$";

	if (!decode_map(regex, map, min, max)) {
		return -1;
	}

	print(map, min, max);

	regex = "^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$";

	if (!decode_map(regex, map, min, max)) {
		return -1;
	}

	print(map, min, max);

	result1 = 1;

#endif

	if (!init(regex)) {
		return -1;
	}

	if (!decode_map(regex, map, min, max)) {
		return -1;
	}

	print(map, min, max);

	std::cout << "=== Advent of Code 2018 - day 20 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
