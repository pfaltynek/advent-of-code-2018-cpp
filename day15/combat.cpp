#include "combat.hpp"
#include "main.hpp"

bool Combat::init() {
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

bool Combat::init(std::vector<std::string> test_data) {
	return decode_map_input(test_data);
}

bool Combat::decode_map_input(std::vector<std::string> map) {
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

bool Combat::compare_fighters_position(Fighter f1, Fighter f2) {
	if (f1.get_y() < f2.get_y()) {
		return true;
	} else if (f1.get_y() > f2.get_y()) {
		return false;
	} else {
		return (f1.get_x() < f2.get_x());
	}
}

void Combat::sort_fighters() {
	std::sort(_fighters.begin(), _fighters.end(), compare_fighters_position);
}

int Combat::make_combat() {
	uint32_t elfs = 0, goblins = 0, idx = 0;

	return 0;
}

void Combat::one_round() {
	uint32_t elfs = 0, goblins = 0, idx = 0;

	sort_fighters();
	for (auto it = _fighters.begin(); it != _fighters.end(); ++it) {
		std::vector<std::string> map(_map);
	}

	while (idx < _fighters.size()) {
		if (_fighters[idx].is_alive()) {
			if (_fighters[idx].get_is_elf()) {
				elfs++;
			} else {
				goblins++;
			}
		} else {
			_fighters.erase(_fighters.begin() + idx);
		}
	}
}

void Combat::one_turn(Fighter f) {
	std::vector<Fighter> enemies;
	std::map<std::pair<uint32_t, uint32_t>, int> targets;
	Fighter target;
	bool target_found = false;

	place_fighters_and_get_enemies(f, enemies);
	get_targets_of_enemies(enemies, targets);
}

void Combat::place_fighters_and_get_enemies(const Fighter f, std::vector<Fighter> &enemies) {

	enemies.clear();
	_tmp_map = _map;

	for (auto it = _fighters.begin(); it != _fighters.end(); ++it) {
		if ((it->is_alive()) && (f.get_is_elf() != it->get_is_elf())) {
			enemies.push_back(*it);
		}
		if (it->get_is_elf()) {
			_tmp_map[it->get_y()][it->get_x()] = 'E';
		} else {
			_tmp_map[it->get_y()][it->get_x()] = 'G';
		}
	}
}

void Combat::get_targets_of_enemies(const std::vector<Fighter> &enemies, std::map<std::pair<uint32_t, uint32_t>, int> &targets) {
	targets.clear();

	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		std::vector<std::pair<uint32_t, uint32_t>> tmp = get_free_adjacents(*it);

		for (auto itv = tmp.begin(); itv != tmp.end(); ++itv) {
			targets[*itv] = 0;
		}
	}
}

std::vector<std::pair<uint32_t, uint32_t>> Combat::get_free_adjacents(uint32_t x, uint32_t y) {
	std::vector<std::pair<uint32_t, uint32_t>> result;

	result.clear();
	if (y > 0) {
		if (_tmp_map[y - 1][x] == '.') {
			result.push_back(std::make_pair(x, y - 1));
		}
	}
	if (x > 0) {
		if (_tmp_map[y][x - 1] == '.') {
			result.push_back(std::make_pair(x - 1, y));
		}
	}
	if ((x + 1) < _tmp_map[0].size()) {
		if (_tmp_map[y][x + 1] == '.') {
			result.push_back(std::make_pair(x + 1, y));
		}
	}
	if ((y + 1) < _tmp_map.size()) {
		if (_tmp_map[y + 1][x] == '.') {
			result.push_back(std::make_pair(x, y + 1));
		}
	}

	return result;
}

std::vector<std::pair<uint32_t, uint32_t>> Combat::get_free_adjacents(Fighter f) {
	return get_free_adjacents(f.get_x(), f.get_y());
}

int32_t Combat::get_shortest_path(Fighter from, Fighter to, direction_t &start_direction) {
	if (from.is_adjacent(to, start_direction)) {
		return 0;
	}

	uint32_t steps = 0, steps_max = UINT32_MAX;
	std::string path = "|";
	std::queue<path_info_str> paths;
	path_info_str pi;

	while (paths.size()) {
		paths.pop();
	}

	pi.path = "|";
	pi.x = from.get_x();
	pi.y = from.get_y();
	pi.steps = 0;
	paths.push(pi);

	return -1;
}

bool Combat::test(std::pair<uint32_t, uint32_t> next, std::pair<uint32_t, uint32_t> target, std::string &path, direction_t &start_directon, uint32_t &steps,
				 uint32_t &steps_max) {
	std::stringstream position;

	position.clear();
	position << "|" << next.first << "|" << next.second << "|";

	return false;
}
