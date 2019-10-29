#include "main.hpp"

typedef struct STEP_INFO {
	coord_str coords;
	int32_t steps;
	int32_t doors;
} step_info_str;

class RoomsMap {
  public:
	bool init(std::string& regex);
	void print();
	int32_t get_furthest_room_passing_doors_count(int32_t& count1000);
	bool decode_map(const std::string regex);
	char get_map_symbol(const coord_str coord);
	char get_map_symbol(int32_t x, int32_t y);

  private:
	std::map<coord_str, char> map_;
	std::map<coord_str, std::vector<coord_str>> doors_map_;
	coord_str min_, doors_min_;
	coord_str max_, doors_max_;
};

bool RoomsMap::init(std::string& regex) {
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

bool RoomsMap::decode_map(const std::string regex) {
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

char RoomsMap::get_map_symbol(const coord_str coord) {
	if (map_.count(coord)) {
		return map_[coord];
	} else {
		return '#';
	}
}

char RoomsMap::get_map_symbol(int32_t x, int32_t y) {
	return get_map_symbol(coord_str(x, y));
}

void RoomsMap::print() {
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
}

int32_t RoomsMap::get_furthest_room_passing_doors_count(int32_t& count1000) {
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

int main(void) {
	int32_t result1 = 0, result2 = 0;
	std::string regex;
	RoomsMap map;
#if TEST

	regex = "^ENWWW(NEEE|SSE(EE|N))$";

	if (!map.decode_map(regex)) {
		return -1;
	}

	map.print();

	result1 = map.get_furthest_room_passing_doors_count(result2);

	regex = "^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$";

	if (!map.decode_map(regex)) {
		return -1;
	}

	map.print();

	result1 = map.get_furthest_room_passing_doors_count(result2);

	regex = "^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$";

	if (!map.decode_map(regex)) {
		return -1;
	}

	map.print();

	result1 = map.get_furthest_room_passing_doors_count(result2);

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

	regex = "^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$";

	if (!map.decode_map(regex)) {
		return -1;
	}

	map.print();

	result1 = map.get_furthest_room_passing_doors_count(result2);

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

	if (!map.init(regex)) {
		return -1;
	}

	if (!map.decode_map(regex)) {
		return -1;
	}

	// map.print();

	std::cout << "=== Advent of Code 2018 - day 20 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = map.get_furthest_room_passing_doors_count(result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
