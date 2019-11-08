#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>

#define TEST 1

typedef enum CART_HEAD { HEAD_UP = '^', HEAD_DOWN = 'v', HEAD_LEFT = '<', HEAD_RIGHT = '>' } cart_head_t;
typedef enum CART_TURN { TURN_LEFT = 0, TURN_STRAIGHT = 1, TURN_RIGHT = 2 } cart_turn_t;

typedef struct cart {
	coord_str coord;
	cart_head_t head;
	cart_turn_t next_turn;
	bool removed;
	uint64_t get_coord() {
		uint64_t result = coord.x;
		result += coord.y * 1000;
		return result;
	}
} cart_str;

class AoC2018_day13 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool simulate_cart_move(cart_str &cart);
	bool simulate_carts_movement_till_first_crash(coord_str &crash);
	bool simulate_carts_movement_till_last_remaining(coord_str &last);
	std::vector<cart_str> carts_;
	std::vector<std::string> map_;
};

bool AoC2018_day13::init(std::vector<std::string> lines) {
	uint32_t map_width;
	std::string cart_head;

	cart_head.clear();
	cart_head += (char)HEAD_UP;
	cart_head += (char)HEAD_DOWN;
	cart_head += (char)HEAD_LEFT;
	cart_head += (char)HEAD_RIGHT;

	carts_.clear();

	for (uint32_t i = 0; i < lines.size(); ++i) {
		size_t pos = 0;

		if (i) {
			if (lines[i].size() != map_width) {
				std::cout << "Invalid input size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			map_width = lines[i].size();
		}

		pos = lines[i].find_first_of(cart_head, pos);

		while (pos != std::string::npos) {
			cart_str cart = {};
			cart.coord.x = pos;
			cart.coord.y = i;
			cart.head = (cart_head_t)lines[i][pos];
			cart.next_turn = CART_TURN::TURN_LEFT;
			cart.removed = false;
			carts_.push_back(cart);

			if ((lines[i][pos] == HEAD_DOWN) || (lines[i][pos] == HEAD_UP)) {
				lines[i][pos] = '|';
			}

			if ((lines[i][pos] == HEAD_LEFT) || (lines[i][pos] == HEAD_RIGHT)) {
				lines[i][pos] = '-';
			}

			pos = lines[i].find_first_of(cart_head, pos);
		}
	}

	map_ = lines;

	return true;
}

bool AoC2018_day13::simulate_cart_move(cart_str &cart) {
	switch (cart.head) {
		case CART_HEAD::HEAD_UP:
			cart.coord.y--;
			break;
		case CART_HEAD::HEAD_DOWN:
			cart.coord.y++;
			break;
		case CART_HEAD::HEAD_LEFT:
			cart.coord.x--;
			break;
		case CART_HEAD::HEAD_RIGHT:
			cart.coord.x++;
			break;
		default:
			return false;
	}

	switch (map_[cart.coord.y][cart.coord.x]) {
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

bool AoC2018_day13::simulate_carts_movement_till_first_crash(coord_str &crash) {
	std::map<uint64_t, uint32_t> cart_order, tmp;
	uint32_t i;
	std::vector<cart_str> carts = carts_;

	while (true) {
		cart_order.clear();
		tmp.clear();

		for (i = 0; i < carts.size(); ++i) {
			cart_order[carts[i].get_coord()] = i;
		}

		for (auto it = cart_order.begin(); it != cart_order.end(); ++it) {
			i = it->second;

			if (!simulate_cart_move(carts[i])) {
				return false;
			}

			if (tmp.count(carts[i].get_coord())) {
				crash = carts[i].coord;
				return true;
			} else {
				tmp[carts[i].get_coord()] = i;
			}
		}
	}

	return false;
}

bool AoC2018_day13::simulate_carts_movement_till_last_remaining(coord_str &last) {
	std::map<uint64_t, uint32_t> cart_order, tmp;
	uint32_t i, j;
	std::vector<cart_str> carts = carts_;

	while (carts.size() >= 2) {
		cart_order.clear();
		tmp.clear();

		for (i = 0; i < carts.size(); ++i) {
			cart_order[carts[i].get_coord()] = i;
		}

		for (auto it = cart_order.begin(); it != cart_order.end(); ++it) {
			i = it->second;

			if (!carts[i].removed) {
				if (!simulate_cart_move(carts[i])) {
					return false;
				}
			}

			for (j = 0; j < carts.size(); ++j) {
				if (j != i) {
					if ((carts[i].coord.x == carts[j].coord.x) && (carts[i].coord.y == carts[j].coord.y)) {
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
		last = carts[0].coord;
		return true;
	}

	return false;
}

int32_t AoC2018_day13::get_aoc_day() {
	return 13;
}

int32_t AoC2018_day13::get_aoc_year() {
	return 2018;
}

void AoC2018_day13::tests() {
#if TEST
	/*
/->-\
|   |  /----\
| /-+--+-\  |
| | |  | v  |
\-+-/  \-+--/
  \------/
*/
	init({"/->-\\        ", "|   |  /----\\", "| /-+--+-\\  |", "| | |  | v  |", "\\-+-/  \\-+--/", "  \\------/   "});
	part1();

	/*
/>-<\
|   |
| /<+-\
| | | v
\>+</ |
  |   ^
  \<->/
*/
	init({"/>-<\\  ", "|   |  ", "| /<+-\\", "| | | v", "\\>+</ |", "  |   ^", "  \\<->/"});
	part2();

#endif
}

bool AoC2018_day13::part1() {
	coord_str result;

	if (simulate_carts_movement_till_first_crash(result)) {
		result1_ = std::to_string(result.x) + "," + std::to_string(result.y);
		return true;
	}

	return false;
}

bool AoC2018_day13::part2() {
	coord_str result;

	if (simulate_carts_movement_till_last_remaining(result)) {
		result2_ = std::to_string(result.x) + "," + std::to_string(result.y);
		return true;
	}

	return false;
}

int main(void) {
	AoC2018_day13 day13;

	return day13.main_execution();
}
