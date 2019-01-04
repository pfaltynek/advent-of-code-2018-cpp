#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#define TEST1 0
#define TEST2 0 

typedef enum CART_HEAD { HEAD_UP = '^', HEAD_DOWN = 'v', HEAD_LEFT = '<', HEAD_RIGHT = '>' } cart_head_t;
typedef enum CART_TURN { TURN_LEFT = 0, TURN_STRAIGHT = 1, TURN_RIGHT = 2 } cart_turn_t;

typedef struct cart {
	uint32_t x;
	uint32_t y;
	cart_head_t head;
	cart_turn_t next_turn;
	bool removed;
} cart_str;

uint64_t GetCartCoord(const cart_str cart) {
	uint64_t result;

	result = cart.x;
	result += cart.y * 1000;

	return result;
}

bool SimulateCartMove(const std::vector<std::string> &map, cart_str &cart) {
	switch (cart.head) {
		case CART_HEAD::HEAD_UP:
			cart.y--;
			break;
		case CART_HEAD::HEAD_DOWN:
			cart.y++;
			break;
		case CART_HEAD::HEAD_LEFT:
			cart.x--;
			break;
		case CART_HEAD::HEAD_RIGHT:
			cart.x++;
			break;
		default:
			return false;
	}

	switch (map[cart.y][cart.x]) {
		case '-':
			if ((cart.head == CART_HEAD::HEAD_UP) || (cart.head == CART_HEAD::HEAD_DOWN)) {
				return false;
			}
			break;
		case '|':
			if ((cart.head == CART_HEAD::HEAD_LEFT) || (cart.head == CART_HEAD::HEAD_RIGHT)) {
				return false;
			}
			break;
		case '/':
			switch (cart.head) {
				case CART_HEAD::HEAD_UP:
					cart.head = CART_HEAD::HEAD_RIGHT;
					break;
				case CART_HEAD::HEAD_DOWN:
					cart.head = CART_HEAD::HEAD_LEFT;
					break;
				case CART_HEAD::HEAD_LEFT:
					cart.head = CART_HEAD::HEAD_DOWN;
					break;
				case CART_HEAD::HEAD_RIGHT:
					cart.head = CART_HEAD::HEAD_UP;
					break;
			}
			break;
		case '\\':
			switch (cart.head) {
				case CART_HEAD::HEAD_UP:
					cart.head = CART_HEAD::HEAD_LEFT;
					break;
				case CART_HEAD::HEAD_DOWN:
					cart.head = CART_HEAD::HEAD_RIGHT;
					break;
				case CART_HEAD::HEAD_LEFT:
					cart.head = CART_HEAD::HEAD_UP;
					break;
				case CART_HEAD::HEAD_RIGHT:
					cart.head = CART_HEAD::HEAD_DOWN;
					break;
			}
			break;
		case '+':
			switch (cart.next_turn) {
				case CART_TURN::TURN_LEFT:
					cart.next_turn = CART_TURN::TURN_STRAIGHT;

					switch (cart.head) {
						case CART_HEAD::HEAD_UP:
							cart.head = CART_HEAD::HEAD_LEFT;
							break;
						case CART_HEAD::HEAD_DOWN:
							cart.head = CART_HEAD::HEAD_RIGHT;
							break;
						case CART_HEAD::HEAD_LEFT:
							cart.head = CART_HEAD::HEAD_DOWN;
							break;
						case CART_HEAD::HEAD_RIGHT:
							cart.head = CART_HEAD::HEAD_UP;
							break;
					}
					break;
				case CART_TURN::TURN_STRAIGHT:
					cart.next_turn = CART_TURN::TURN_RIGHT;
					break;
				case CART_TURN::TURN_RIGHT:
					cart.next_turn = CART_TURN::TURN_LEFT;

					switch (cart.head) {
						case CART_HEAD::HEAD_UP:
							cart.head = CART_HEAD::HEAD_RIGHT;
							break;
						case CART_HEAD::HEAD_DOWN:
							cart.head = CART_HEAD::HEAD_LEFT;
							break;
						case CART_HEAD::HEAD_LEFT:
							cart.head = CART_HEAD::HEAD_UP;
							break;
						case CART_HEAD::HEAD_RIGHT:
							cart.head = CART_HEAD::HEAD_DOWN;
							break;
					}
					break;
				default:
					return false;
			}
			break;
		default:
			return false;
	}
	return true;
}

bool SimulateCartsMovementTillFirstCrash(const std::vector<std::string> &map, std::vector<cart_str> carts, uint32_t &crash_x, uint32_t &crash_y) {
	std::map<uint64_t, uint32_t> cart_order, tmp;
	uint32_t i;
	uint64_t coord;
	uint32_t cnt;

	cnt = 0;

	while (true) {
		cart_order.clear();
		tmp.clear();

		cnt++;

		for (i = 0; i < carts.size(); ++i) {
			coord = GetCartCoord(carts[i]);
			cart_order[coord] = i;
		}

		for (auto it = cart_order.begin(); it != cart_order.end(); ++it) {
			i = it->second;

			if (!SimulateCartMove(map, carts[i])) {
				return false;
			}

			coord = GetCartCoord(carts[i]);
			if (tmp.count(coord)) {
				crash_x = carts[i].x;
				crash_y = carts[i].y;
				return true;
			} else {
				tmp[coord] = i;
			}
		}
	}

	return false;
}

bool SimulateCartsMovementTillLastRemaining(const std::vector<std::string> &map, std::vector<cart_str> carts, uint32_t &last_x, uint32_t &last_y) {
	std::map<uint64_t, uint32_t> cart_order, tmp;
	uint32_t i, j, x1, x2;
	uint64_t coord;
	uint32_t cnt;
	cnt = 0;

	while (carts.size() >= 2) {
		cart_order.clear();
		tmp.clear();

		cnt++;

		for (i = 0; i < carts.size(); ++i) {
			coord = GetCartCoord(carts[i]);
			cart_order[coord] = i;
		}

		for (auto it = cart_order.begin(); it != cart_order.end(); ++it) {
			i = it->second;

			if (!carts[i].removed) {
				if (!SimulateCartMove(map, carts[i])) {
					return false;
				}
			}

			for (j = 0; j < carts.size(); ++j) {
				if (j != i) {
					if ((carts[i].x == carts[j].x) && (carts[i].y == carts[j].y)) {
						carts[i].removed = true;
						carts[j].removed = true;
						break;
					}
				}
			}
		}

		i = 0;
		while (i < carts.size()) {
			if (carts[i].removed) {
				carts.erase(carts.begin() + i);
			} else {
				i++;
			}
		}
	}

	if (carts.size()) {
		last_x = carts[0].x;
		last_y = carts[0].y;
		return true;
	}

	return false;
}

bool DecodeMap(std::vector<std::string> &map, std::vector<cart_str> &carts) {
	uint32_t map_width;
	std::string cart_head;

	cart_head.clear();
	cart_head += (char)HEAD_UP;
	cart_head += (char)HEAD_DOWN;
	cart_head += (char)HEAD_LEFT;
	cart_head += (char)HEAD_RIGHT;

	for (uint32_t i = 0; i < map.size(); ++i) {
		size_t pos = 0;

		if (i) {
			if (map[i].size() != map_width) {
				std::cout << "Invalid input size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			map_width = map[i].size();
		}

		pos = map[i].find_first_of(cart_head, pos);

		while (pos != std::string::npos) {
			cart_str cart = {};
			cart.x = pos;
			cart.y = i;
			cart.head = (cart_head_t)map[i][pos];
			cart.next_turn = CART_TURN::TURN_LEFT;
			cart.removed = false;
			carts.push_back(cart);

			if ((map[i][pos] == HEAD_DOWN) || (map[i][pos] == HEAD_UP)) {
				map[i][pos] = '|';
			}

			if ((map[i][pos] == HEAD_LEFT) || (map[i][pos] == HEAD_RIGHT)) {
				map[i][pos] = '-';
			}

			pos = map[i].find_first_of(cart_head, pos);
		}
	}

	return true;
}

int main(void) {
	int cnt = 0;
	uint32_t result1x = 0, result1y = 0, result2x = 0, result2y = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::string> map;
	std::vector<cart_str> carts;

	std::cout << "=== Advent of Code 2018 - day 13 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	map.clear();
	carts.clear();

#if TEST1
	/*
	/->-\
	|   |  /----\
	| /-+--+-\  |
	| | |  | v  |
	\-+-/  \-+--/
	  \------/
	*/
	map.push_back("/->-\\        ");
	map.push_back("|   |  /----\\");
	map.push_back("| /-+--+-\\  |");
	map.push_back("| | |  | v  |");
	map.push_back("\\-+-/  \\-+--/");
	map.push_back("  \\------/   ");
#elif TEST2
	/*
	/>-<\
	|   |
	| /<+-\
	| | | v
	\>+</ |
	  |   ^
	  \<->/
	*/
	map.push_back("/>-<\\  ");
	map.push_back("|   |  ");
	map.push_back("| /<+-\\");
	map.push_back("| | | v");
	map.push_back("\\>+</ |");
	map.push_back("  |   ^");
	map.push_back("  \\<->/");
#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {

		map.push_back(line);
		cnt++;
	}

	if (input.is_open()) {
		input.close();
	}

#endif

	if (!DecodeMap(map, carts)) {
		return -1;
	}

	if (!SimulateCartsMovementTillFirstCrash(map, carts, result1x, result1y)) {
		std::cout << "Simulation failed" << std::endl;
		return -1;
	}

	std::cout << "Result is " << result1x << "," << result1y << std::endl;

	if (!SimulateCartsMovementTillLastRemaining(map, carts, result2x, result2y)) {
		std::cout << "Simulation failed" << std::endl;
		return -1;
	}

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2x << "," << result2y << std::endl;
}
