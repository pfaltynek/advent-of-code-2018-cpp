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

	_no_fighters = map;

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

int Fight::make_fight() {

	return 0;
}

void Fight::one_round() {
	sort_fighters();
	for (auto it = _fighters.begin(); it != _fighters.end(); ++it) {
		std::vector<std::string> map(_map);
	}
}

void Fight::one_turn(Fighter f) {
	std::vector<std::string> map(_map);
	std::vector<Fighter> enemies;
	std::map<std::pair<uint32_t, uint32_t>, int> targets;
	Fighter target;
	bool target_found = false;

	enemies.clear();

	for (auto it = _fighters.begin(); it != _fighters.end(); ++it) {
		if (f.get_is_elf() != it->get_is_elf()) {
			enemies.push_back(*it);
		}
		if (it->get_is_elf()) {
			map[it->get_y()][it->get_x()] = 'E';
		} else {
			map[it->get_y()][it->get_x()] = 'G';
		}
	}

	targets.clear();

	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		uint32_t x, y;

		x = it->get_x();
		y = it->get_y();

		if (y > 0) {
			if (map[y - 1][x] == '.') {
				targets[std::make_pair(x, y - 1)] = 0;
			}
		}
		if (x > 0) {
			if (map[y][x - 1] == '.') {
				targets[std::make_pair(x - 1, y)] = 0;
			}
		}
		if ((x + 1) < map[0].size()) {
			if (map[y][x + 1] == '.') {
				targets[std::make_pair(x + 1, y)] = 0;
			}
		}
		if ((y + 1) < map.size()) {
			if (map[y + 1][x] == '.') {
				targets[std::make_pair(x, y + 1)] = 0;
			}
		}
	}
}
