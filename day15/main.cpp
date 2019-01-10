#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#define TEST1 0
#define TEST2 0

class Fighter {
  public:
	Fighter();
	void init(uint32_t x, uint32_t y, bool is_elf);
	uint32_t get_x() {
		return _x;
	}
	uint32_t get_y() {
		return _y;
	}
	uint32_t get_hit_points() {
		return _hitpoints;
	}
	bool get_is_elf() {
		return _is_elf;
	}

  private:
	uint32_t _x;
	uint32_t _y;
	uint32_t _hitpoints;
	uint32_t _attack_power;
	bool _is_elf;
};

Fighter::Fighter() {
	_x = 0;
	_y = 0;
	_hitpoints = 200;
	_attack_power = 3;
	_is_elf = true;
}

void Fighter::init(uint32_t x, uint32_t y, bool is_elf) {
	_x = x;
	_y = y;
	_is_elf = is_elf;
}

class AOC2018Day15 {
  public:
	AOC2018Day15(){};

	bool init();
	bool init(std::vector<std::string> test_data);
	void turn();

  protected:
  private:
	std::vector<std::string> _map;
	std::vector<Fighter> _fighters;
	bool decode_map_input(std::vector<std::string> input);
	void sort_fighters();
	void split_tartets(std::vector<Fighter> &elfs, std::vector<Fighter> &goblins);
};

bool AOC2018Day15::init() {
	int cnt;
	std::ifstream input;
	std::string line;
	std::vector<std::string> map;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return false;
	}

	cnt = 0;

	while (std::getline(input, line)) {
		map.push_back(line);
		cnt++;
	}

	if (input.is_open()) {
		input.close();
	}
	return decode_map_input(map);
}

bool AOC2018Day15::init(std::vector<std::string> test_data) {
	return decode_map_input(test_data);
}

void AOC2018Day15::turn() {
	std::vector<Fighter> elfs, goblins;

	sort_fighters();
	split_tartets(elfs, goblins);
}

bool AOC2018Day15::decode_map_input(std::vector<std::string> map) {
	uint32_t i, w;
	size_t pos;

	_map.clear();
	_fighters.clear();

	for (i = 0; i < map.size(); ++i) {
		if (i) {
			if (map[i].size() != w) {
				std::cout << "Invalid input map line " << i + 1 << std::endl;
				return false;
			}
		} else {
			w = map[i].size();
		}

		pos = map[i].find_first_not_of("EG#.");
		if (pos != std::string::npos) {
			std::cout << "Invalid input map content at line " << i + 1 << std::endl;
			return false;
		}

		pos = map[i].find_first_of("EG");
		while (pos != std::string::npos) {
			Fighter fighter;
			fighter.init(pos, i, map[i][pos] == 'E');
			map[i][pos] = '.';
			_fighters.push_back(fighter);
			pos = map[i].find_first_of("EG", pos + 1);
		}

		_map.push_back(map[i]);
	}

	return true;
}

bool compare_fighters_position(Fighter f1, Fighter f2) {
	if (f1.get_y() < f2.get_y()) {
		return true;
	} else if (f1.get_y() > f2.get_y()) {
		return false;
	} else {
		return (f1.get_x() < f2.get_x());
	}
}

void AOC2018Day15::sort_fighters() {
	std::sort(_fighters.begin(), _fighters.end(), compare_fighters_position);
}

void AOC2018Day15::split_tartets(std::vector<Fighter>& elfs, std::vector<Fighter>& goblins){
	elfs.clear();
	goblins.clear();

	for (uint32_t i = 0; i < _fighters.size(); ++i) {
		if (_fighters[i].get_is_elf()){
			elfs.push_back(_fighters[i]);
		} else {
			goblins.push_back(_fighters[i]);
		}
	}
}

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	AOC2018Day15 obj;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1

#elif TEST2

#else
	if (!obj.init()) {
		return -1;
	}

	obj.turn();
#endif

	std::cout << "Result is " << result1 << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
