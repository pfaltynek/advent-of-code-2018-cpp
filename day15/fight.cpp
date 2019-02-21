#include "fight.hpp"
#include "main.hpp"

bool Fight::init() {
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

bool Fight::init(std::vector<std::string> test_data) {
	return decode_map_input(test_data);
}

bool Fight::decode_map_input(std::vector<std::string> map) {
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

bool Fight::compare_fighters_position(Fighter f1, Fighter f2) {
	if (f1.get_y() < f2.get_y()) {
		return true;
	} else if (f1.get_y() > f2.get_y()) {
		return false;
	} else {
		return (f1.get_x() < f2.get_x());
	}
}

void Fight::sort_fighters() {
	std::sort(_fighters.begin(), _fighters.end(), compare_fighters_position);
}

void Fight::split_targets(std::vector<Fighter> &elfs, std::vector<Fighter> &goblins) {
	elfs.clear();
	goblins.clear();

	for (uint32_t i = 0; i < _fighters.size(); ++i) {
		if (_fighters[i].get_is_elf()) {
			elfs.push_back(_fighters[i]);
		} else {
			goblins.push_back(_fighters[i]);
		}
	}
}

int Fight::make_fight(){

	return 0;
}

void Fight::one_round(){

}

void Fight::one_turn() {
	std::vector<Fighter> elfs, goblins;

	sort_fighters();
	for (auto it = _fighters.begin(); it != _fighters.end(); ++it) {
		std::vector<std::string> map(_map);
	}

	split_targets(elfs, goblins);
}
