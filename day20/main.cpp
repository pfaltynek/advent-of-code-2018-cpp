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
	coord_str min_;
	coord_str max_;
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

	map_.clear();
	coord = {};
	while (!stack.empty()) {
		stack.pop();
	}

	min_ = max_ = coord;
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
				break;
			case 'W':
				new_coord = coord;
				new_coord.x--;
				map_[new_coord] = '|';
				new_coord.x--;
				map_[new_coord] = '.';
				coord = new_coord;
				break;
			case 'S':
				new_coord = coord;
				new_coord.y--;
				map_[new_coord] = '-';
				new_coord.y--;
				map_[new_coord] = '.';
				coord = new_coord;
				break;
			case 'E':
				new_coord = coord;
				new_coord.x++;
				map_[new_coord] = '|';
				new_coord.x++;
				map_[new_coord] = '.';
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
	coord_str pt = {}, room;
	step_info_str curr, tmp;
	std::queue<step_info_str> queue;
	std::map<coord_str, bool> history;
	std::queue<coord_str> rooms;
	std::map<coord_str, std::vector<step_info_str>> results;
	std::vector<coord_str> nexts = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
	char c;
	int32_t result = 0;

	count1000 = 0;
	rooms = {};
	results.clear();

	for (int32_t i = min_.x; i <= max_.x; i++) {
		for (int32_t j = min_.y; j <= max_.y; j++) {
			coord_str coord(i, j);

			if (get_map_symbol(coord) == '.') {
				rooms.emplace(coord);
			}
		}
	}

	while (!rooms.empty()) {
		room = rooms.front();
		rooms.pop();
		history.clear();
		queue = {};

		curr.coords = pt;
		curr.doors = 0;
		curr.steps = 0;
		queue.emplace(curr);
		history[pt] = true;

		while (!queue.empty()) {
			curr = queue.front();
			queue.pop();
			history[curr.coords] = true;

			if (curr.coords == room) {
				results[room].push_back(curr);
			} else {
				for (uint32_t i = 0; i < nexts.size(); i++) {
					tmp = curr;
					tmp.coords.x += nexts[i].x;
					tmp.coords.y += nexts[i].y;
					c = get_map_symbol(tmp.coords);
					if ((c == '-') || (c == '|')) {
						tmp.coords.x += nexts[i].x;
						tmp.coords.y += nexts[i].y;
						if (!history.count(tmp.coords)) {
							c = get_map_symbol(tmp.coords);
							if (c == '.') {
								tmp.doors++;
								tmp.steps += 2;
								queue.emplace(tmp);
							}
						}
					}
				}
			}
		}
	}
	for (auto it = results.begin(); it != results.end(); it++) {
		int32_t steps = INT32_MAX, doors = 0;

		for (uint32_t i = 0; i < it->second.size(); ++i) {
			if (it->second[i].steps < steps) {
				steps = it->second[i].steps;
				doors = it->second[i].doors;
			}
		}
		if (doors > result) {
			result = doors;
		}
		if (doors >=1000){
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

	//map.print();

	std::cout << "=== Advent of Code 2018 - day 20 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = map.get_furthest_room_passing_doors_count(result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
