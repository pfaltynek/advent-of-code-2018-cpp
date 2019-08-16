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

	map_.clear();
	fighters_.clear();

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
			fighters_.push_back(fighter);
			pos = map[i].find_first_of("EG", pos + 1);
		}

		map_.push_back(map[i]);
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
	std::sort(fighters_.begin(), fighters_.end(), compare_fighters_position);
}

uint32_t Combat::make_combat() {
	uint32_t remaining_hitpoints_sum = 0, rounds = 0;
	bool combat_end = false;

	while (true) {
		if (one_round(remaining_hitpoints_sum)) {
			break;
		}

		rounds++;
	}

	return (rounds * remaining_hitpoints_sum);
}

bool Combat::one_round(uint32_t &remaining_hitpoints_sum) {
	uint32_t elfs = 0, goblins = 0, idx = 0;

	sort_fighters();
	for (auto it = fighters_.begin(); it != fighters_.end(); ++it) {
		std::vector<std::string> map(map_);

		if (one_turn(*it)) {
			return true;
		}
	}

	while (idx < fighters_.size()) {
		if (fighters_[idx].is_alive()) {
			if (fighters_[idx].get_is_elf()) {
				elfs++;
			} else {
				goblins++;
			}
		} else {
			fighters_.erase(fighters_.begin() + idx);
		}
	}

	return false;
}

bool Combat::one_turn(Fighter f) {
	std::vector<Fighter> enemies;
	std::map<std::pair<uint32_t, uint32_t>, int> targets;
	std::vector<std::pair<uint32_t, uint32_t>> adjacents;
	Fighter target;
	uint32_t steps_max = UINT32_MAX;

	place_fighters_and_get_enemies(f, enemies);

	if (!enemies.size()) {
		return true;
	}

	adjacents = get_adjacents_ordered(f);

	for (auto it = adjacents.begin(); it != adjacents.end(); it++) {
		for (auto it2 = enemies.begin(); it2 != enemies.end(); it2++) {
			if ((it->first == it2->get_x()) && (it->second == it2->get_y())) {
				it2->got_attacked(f.get_attack_power());
				return false;
			}
		}
	}

	get_targets_of_enemies(enemies, targets);

	return false;
}

void Combat::place_fighters_and_get_enemies(const Fighter f, std::vector<Fighter> &enemies) {

	enemies.clear();
	tmp_map_ = map_;

	for (auto it = fighters_.begin(); it != fighters_.end(); ++it) {
		if ((it->is_alive()) && (f.get_is_elf() != it->get_is_elf())) {
			enemies.push_back(*it);
		}
		if (it->get_is_elf()) {
			tmp_map_[it->get_y()][it->get_x()] = 'E';
		} else {
			tmp_map_[it->get_y()][it->get_x()] = 'G';
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

std::vector<std::pair<uint32_t, uint32_t>> Combat::get_adjacents_ordered(uint32_t x, uint32_t y) {
	std::vector<std::pair<uint32_t, uint32_t>> result;

	result.clear();
	if (y > 0) {
		result.push_back(std::make_pair(x, y - 1));
	}
	if (x > 0) {
		result.push_back(std::make_pair(x - 1, y));
	}
	if ((x + 1) < tmp_map_[0].size()) {
		result.push_back(std::make_pair(x + 1, y));
	}
	if ((y + 1) < tmp_map_.size()) {
		result.push_back(std::make_pair(x, y + 1));
	}

	return result;
}

std::vector<std::pair<uint32_t, uint32_t>> Combat::get_adjacents_ordered(Fighter f) {
	return get_adjacents_ordered(f.get_x(), f.get_y());
}

std::vector<std::pair<uint32_t, uint32_t>> Combat::get_free_adjacents(uint32_t x, uint32_t y) {
	std::vector<std::pair<uint32_t, uint32_t>> result, adjacents;

	result.clear();
	adjacents = get_adjacents_ordered(x, y);

	for (auto it = adjacents.begin(); it != adjacents.end(); it++) {
		if (tmp_map_[it->second][it->first] == '.') {
			result.push_back(*it);
		}
	}

	return result;
}

std::vector<std::pair<uint32_t, uint32_t>> Combat::get_free_adjacents(Fighter f) {
	return get_free_adjacents(f.get_x(), f.get_y());
}

int32_t Combat::get_shortest_path(Fighter from, uint32_t target_x, uint32_t target_y, uint32_t &max_steps, direction_t &start_direction) {
	uint32_t steps = 0;
	std::string path = "|";
	std::queue<path_info_str> paths;
	path_info_str pi, tmp;
	std::vector<std::pair<uint32_t, uint32_t>> next_pos;

	while (paths.size()) {
		paths.pop(); // for sure - clear queue
	}

	pi.path = "|";
	pi.x = from.get_x();
	pi.y = from.get_y();
	pi.steps = 0;
	paths.push(pi);

	while (paths.size()) {
		tmp = paths.front();
		paths.pop();
		next_pos = get_adjacents_ordered(tmp.x, tmp.y);

	}

	return -1;
}

bool Combat::test(std::pair<uint32_t, uint32_t> next, std::pair<uint32_t, uint32_t> target, std::string &path, direction_t &start_directon, uint32_t &steps,
				  uint32_t &steps_max) {
	std::stringstream position;

	position.clear();
	position << "|" << next.first << "|" << next.second << "|";

	return false;
}
