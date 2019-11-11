#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <stack>

#define TEST 1
#define DEBUG_PRINT 0

typedef struct STEP_INFO {
	coord_str coords;
	int32_t steps;
	int32_t doors;
} step_info_str;

class AoC2018_day20 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	void print();
	int32_t get_furthest_room_passing_doors_count(int32_t& count1000);
	bool decode_map(const std::string regex);
	char get_map_symbol(const coord_str coord);
	char get_map_symbol(int32_t x, int32_t y);

	std::map<coord_str, char> map_;
	std::map<coord_str, std::vector<coord_str>> doors_map_;
	coord_str min_, doors_min_;
	coord_str max_, doors_max_;
};

bool AoC2018_day20::init(const std::vector<std::string> lines) {
	std::string regex;

	for (uint32_t i = 0; i < lines.size(); ++i) {
		regex.append(lines[i]);
	}

	return decode_map(regex);
}

bool AoC2018_day20::decode_map(const std::string regex) {
	coord_str coord, new_coord, doors_coord, new_doors_coord;
	std::stack<coord_str> stack, doors_stack;

	if (regex.size() < 3) {
		std::cout << "Invalid input" << std::endl;
		return false;
	}
	if ((regex[0] != '^') || (regex[regex.size() - 1] != '$')) {
		std::cout << "Invalid input start/end" << std::endl;
		return false;
	}

	map_.clear();
	doors_map_.clear();
	coord = doors_coord = {};
	stack = doors_stack = {};
	min_ = max_ = doors_max_ = doors_min_ = coord;
	map_[coord] = 'X';

	for (uint32_t i = 1; i < regex.size() - 1; ++i) {
		switch (regex[i]) {
			case 'N':
				new_coord = coord;
				new_coord.y++;
				map_[new_coord] = '-';
				new_coord.y++;
				map_[new_coord] = '.';
				coord = new_coord;
				new_doors_coord = doors_coord;
				new_doors_coord.y++;
				doors_map_[doors_coord].push_back(new_doors_coord);
				doors_map_[new_doors_coord].push_back(doors_coord);
				doors_coord = new_doors_coord;
				break;
			case 'W':
				new_coord = coord;
				new_coord.x--;
				map_[new_coord] = '|';
				new_coord.x--;
				map_[new_coord] = '.';
				coord = new_coord;
				new_doors_coord = doors_coord;
				new_doors_coord.x--;
				doors_map_[doors_coord].push_back(new_doors_coord);
				doors_map_[new_doors_coord].push_back(doors_coord);
				doors_coord = new_doors_coord;
				break;
			case 'S':
				new_coord = coord;
				new_coord.y--;
				map_[new_coord] = '-';
				new_coord.y--;
				map_[new_coord] = '.';
				coord = new_coord;
				new_doors_coord = doors_coord;
				new_doors_coord.y--;
				doors_map_[doors_coord].push_back(new_doors_coord);
				doors_map_[new_doors_coord].push_back(doors_coord);
				doors_coord = new_doors_coord;
				break;
			case 'E':
				new_coord = coord;
				new_coord.x++;
				map_[new_coord] = '|';
				new_coord.x++;
				map_[new_coord] = '.';
				coord = new_coord;
				new_doors_coord = doors_coord;
				new_doors_coord.x++;
				doors_map_[doors_coord].push_back(new_doors_coord);
				doors_map_[new_doors_coord].push_back(doors_coord);
				doors_coord = new_doors_coord;
				break;
			case '(':
				stack.push(coord);
				doors_stack.push(doors_coord);
				break;
			case ')':
				if (stack.empty()) {
					std::cout << "Invalid branch in regex at " << i << "" << std::endl;
				} else {
					coord = stack.top();
					stack.pop();
				}
				if (doors_stack.empty()) {
					std::cout << "Invalid branch in regex at " << i << "" << std::endl;
				} else {
					doors_coord = doors_stack.top();
					doors_stack.pop();
				}
				break;
			case '|':
				if (stack.empty()) {
					std::cout << "Invalid branch in regex at " << i << "" << std::endl;
				} else {
					coord = stack.top();
				}
				if (doors_stack.empty()) {
					std::cout << "Invalid branch in regex at " << i << "" << std::endl;
				} else {
					doors_coord = doors_stack.top();
				}
				break;
			default:
				std::cout << "Invalid char '" << regex[i] << "' in regex" << std::endl;
				break;
		}
		if (new_coord.x < min_.x) {
			min_.x = new_coord.x;
		}
		if (new_coord.y < min_.y) {
			min_.y = new_coord.y;
		}
		if (new_coord.x > max_.x) {
			max_.x = new_coord.x;
		}
		if (new_coord.y > max_.y) {
			max_.y = new_coord.y;
		}

		if (doors_coord.x < doors_min_.x) {
			doors_min_.x = doors_coord.x;
		}
		if (doors_coord.y < doors_min_.y) {
			doors_min_.y = doors_coord.y;
		}
		if (doors_coord.x > doors_max_.x) {
			doors_max_.x = doors_coord.x;
		}
		if (doors_coord.y > doors_max_.y) {
			doors_max_.y = doors_coord.y;
		}
	}

	return true;
}

char AoC2018_day20::get_map_symbol(const coord_str coord) {
	if (map_.count(coord)) {
		return map_[coord];
	} else {
		return '#';
	}
}

char AoC2018_day20::get_map_symbol(int32_t x, int32_t y) {
	return get_map_symbol(coord_str(x, y));
}

void AoC2018_day20::print() {
#if DEBUG_PRINT
	for (int32_t i = max_.y + 1; i >= min_.y - 1; --i) {
		for (int32_t j = min_.x - 1; j <= max_.x + 1; ++j) {
			coord_str coord = {j, i};
			if (!map_.count(coord)) {
				std::cout << '#';
			} else {
				std::cout << map_[coord];
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
#endif
}

int32_t AoC2018_day20::get_furthest_room_passing_doors_count(int32_t& count1000) {
	step_info_str curr, tmp;
	std::map<coord_str, int32_t> history;
	std::queue<step_info_str> queue;
	int32_t result;

	tmp.coords = {};
	tmp.doors = 0;
	tmp.steps = 0;

	queue = {};
	queue.emplace(tmp);

	while (!queue.empty()) {

		curr = queue.front();
		queue.pop();

		if (!history.count(curr.coords)) {
			history[curr.coords] = curr.doors;

			for (uint32_t i = 0; i < doors_map_[curr.coords].size(); i++) {
				tmp = curr;
				tmp.doors++;
				tmp.coords = doors_map_[curr.coords][i];
				queue.emplace(tmp);
			}
		}
	}

	result = count1000 = 0;
	for (auto it = history.begin(); it != history.end(); it++) {
		if (it->second > result) {
			result = it->second;
		}
		if (it->second >= 1000) {
			count1000++;
		}
	}

	return result;
}

int32_t AoC2018_day20::get_aoc_day() {
	return 20;
}

int32_t AoC2018_day20::get_aoc_year() {
	return 2018;
}

void AoC2018_day20::tests() {
#if TEST
	int32_t result1, result2;

	if (init({"^ENWWW(NEEE|SSE(EE|N))$"})) {
		print();
		result1 = get_furthest_room_passing_doors_count(result2);
	}

	if (init({"^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$"})) {
		print();
		result1 = get_furthest_room_passing_doors_count(result2);
	}

	if (init({"^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$"})) {
		print();
		result1 = get_furthest_room_passing_doors_count(result2);
	}
	// Furthest room requires passing 23 doors
	/*
	#############
	#.|.|.|.|.|.#
	#-#####-###-#
	#.#.|.#.#.#.#
	#-#-###-#-#-#
	#.#.#.|.#.|.#
	#-#-#-#####-#
	#.#.#.#X|.#.#
	#-#-#-###-#-#
	#.|.#.|.#.#.#
	###-#-###-#-#
	#.|.#.|.|.#.#
	#############
	*/

	if (init({"^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$"})) {
		print();
		result1 = get_furthest_room_passing_doors_count(result2);
	}
		// Furthest room requires passing 31 doors
		/*
		###############
		#.|.|.|.#.|.|.#
		#-###-###-#-#-#
		#.|.#.|.|.#.#.#
		#-#########-#-#
		#.#.|.|.|.|.#.#
		#-#-#########-#
		#.#.#.|X#.|.#.#
		###-#-###-#-#-#
		#.|.#.#.|.#.|.#
		#-###-#####-###
		#.|.#.|.|.#.#.#
		#-#-#####-#-#-#
		#.#.|.|.|.#.|.#
		###############
		*/

#endif
}

bool AoC2018_day20::part1() {
	int32_t result1, result2;

	result1 = get_furthest_room_passing_doors_count(result2);

	result1_ = std::to_string(result1);
	result2_ = std::to_string(result2);

	return true;
}

bool AoC2018_day20::part2() {
	return true;
}

int main(void) {
	AoC2018_day20 day20;

	return day20.main_execution();
}
