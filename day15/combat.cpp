#include "combat.hpp"
#include "main.hpp"
#include "pathinfo.hpp"

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
	return compare_positions(f1.get_x(), f1.get_y(), f2.get_x(), f2.get_y());
}

bool Combat::compare_positions(uint32_t f1x, uint32_t f1y, uint32_t f2x, uint32_t f2y) {
	if (f1y < f2y) {
		return true;
	} else if (f1y > f2y) {
		return false;
	} else {
		return (f1x < f2x);
	}
}

void Combat::sort_fighters() {
	std::sort(fighters_.begin(), fighters_.end(), compare_fighters_position);
}

uint32_t Combat::make_combat() {
	uint32_t remaining_hitpoints_sum = 0, rounds = 0;
	bool combat_end = false;

	print_map("Initially:");

	while (true) {
		if (one_round(remaining_hitpoints_sum)) {
			break;
		}

		rounds++;

		print_map("After round " + std::to_string(rounds) + ":");
	}

	return (rounds * remaining_hitpoints_sum);
}

bool Combat::one_round(uint32_t &remaining_hitpoints_sum) {
	uint32_t idx, i;

	sort_fighters();

	// print_map("");

	i = 0;
	while (i < fighters_.size()) {
		if (one_turn(fighters_[i])) {
			return true;
		}

		// remove dead fighters
		idx = 0;
		while (idx < fighters_.size()) {
			if (!fighters_[idx].is_alive()) {
				if (idx <= i) {
					i--;
				}
				fighters_.erase(fighters_.begin() + idx);
			}
			idx++;
		}

		i++;
	}

	// print_map("");

	return false;
}

bool Combat::attack_if_possible(Fighter &f, std::vector<uint32_t> &enemies) {
	std::vector<std::pair<uint32_t, uint32_t>> adjacents = get_adjacents_ordered(f);
	std::vector<uint32_t>::iterator target = enemies.end();
	uint32_t hp = UINT32_MAX;

	for (auto it = adjacents.begin(); it != adjacents.end(); it++) {
		for (auto it2 = enemies.begin(); it2 != enemies.end(); it2++) {
			if ((it->first == fighters_[*it2].get_x()) && (it->second == fighters_[*it2].get_y())) {
				if (fighters_[*it2].get_hit_points() < hp) {
					target = it2;
					hp = fighters_[*it2].get_hit_points();
				}
			}
		}
	}

	if (target != enemies.end()) {
		fighters_[*target].got_attacked(f.get_attack_power());
		return true;
	}

	return false;
}

bool Combat::one_turn(Fighter &f) {
	std::vector<uint32_t> enemies;
	std::map<std::pair<uint32_t, uint32_t>, int> targets;
	uint32_t steps_max = UINT32_MAX, x1st, y1st;

	place_fighters_and_get_enemies(f, enemies);

	if (!enemies.size()) {
		return true;
	}

	if (attack_if_possible(f, enemies)) {
		return false;
	}

	get_targets_of_enemies(enemies, targets);

	bool found = false;
	for (auto it = targets.begin(); it != targets.end(); it++) {
		uint32_t x1, y1, steps;

		if (get_shortest_path(f, it->first.first, it->first.second, x1, y1, steps, steps_max)) {
			if (steps < steps_max) {
				x1st = x1;
				y1st = y1;
				steps_max = steps;
				found = true;
			} else if (steps == steps_max) {
				if (compare_positions(x1, y1, x1st, y1st)) {
					x1st = x1;
					y1st = y1;
					steps_max = steps;
					found = true;
				}
			}
		}
	}

	if (found) {
		f.move_to(x1st, y1st);
		attack_if_possible(f, enemies);
	}

	return false;
}

void Combat::place_fighters_and_get_enemies(const Fighter f, std::vector<uint32_t> &enemies) {
	enemies.clear();
	tmp_map_ = map_;

	for (uint32_t i = 0; i < fighters_.size(); ++i) {
		if (f.get_is_elf() != fighters_[i].get_is_elf()) {
			enemies.push_back(i);
		}
		if (fighters_[i].get_is_elf()) {
			tmp_map_[fighters_[i].get_y()][fighters_[i].get_x()] = 'E';
		} else {
			tmp_map_[fighters_[i].get_y()][fighters_[i].get_x()] = 'G';
		}
	}
}

void Combat::get_targets_of_enemies(const std::vector<uint32_t> &enemies, std::map<std::pair<uint32_t, uint32_t>, int> &targets) {
	targets.clear();

	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		std::vector<std::pair<uint32_t, uint32_t>> tmp = get_free_adjacents(fighters_[*it]);

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

bool Combat::get_shortest_path(Fighter from, uint32_t target_x, uint32_t target_y, uint32_t &x1, uint32_t &y1, uint32_t &steps, uint32_t max_steps) {
	std::queue<PathInfo> paths;
	std::vector<std::pair<uint32_t, uint32_t>> next_pos;
	bool found = false;

	steps = max_steps;

	while (paths.size()) {
		paths.pop(); // for sure - clear queue
	}

	paths.push(PathInfo(from.get_x(), from.get_y()));

	while (paths.size()) {
		PathInfo tmp = paths.front();
		paths.pop();

		if (tmp.get_steps() >= max_steps) {
			break;
		}

		next_pos = get_free_adjacents(tmp.get_x(), tmp.get_y());

		for (auto it = next_pos.begin(); it != next_pos.end(); it++) {
			PathInfo pi(tmp);

			if (!pi.was_at(it->first, it->second)) {
				pi.move_to(it->first, it->second);

				uint32_t steps_new = pi.get_steps();

				if ((pi.get_x() == target_x) && (pi.get_y() == target_y)) {
					if (steps_new < steps) {
						x1 = pi.get_x_1st();
						y1 = pi.get_y_1st();
						steps = steps_new;
						found = true;
						break;
					} else if (steps_new == steps) {
						if (compare_positions(pi.get_x_1st(), pi.get_y_1st(), x1, y1)) {
							x1 = pi.get_x_1st();
							y1 = pi.get_y_1st();
							steps = steps_new;
							found = true;
							break;
						}
					}
				} else {
					if (steps_new < steps) {
						paths.push(pi);
					}
				}
			}
		}
	}

	return found;
}

void Combat::print_map(std::string title) {
#if DEBUG_PRINT
	std::vector<std::string> tmp = map_, x;
	std::vector<std::vector<std::string>> lives;

	x.clear();
	lives.clear();
	for (uint32_t i = 0; i < tmp.size(); ++i) {
		lives.push_back(x);
	}

	if (!title.empty()) {
		std::cout << title << std::endl;
	}

	for (uint32_t i = 0; i < fighters_.size(); ++i) {
		std::string live = " ";
		char type;

		if (fighters_[i].get_is_elf()) {
			type = 'E';
		} else {
			type = 'G';
		}

		tmp[fighters_[i].get_y()][fighters_[i].get_x()] = type;

		live += type;

		live.append("(");
		live.append(std::to_string(fighters_[i].get_hit_points()));
		live.append(")");

		lives[fighters_[i].get_y()].push_back(live);
	}

	for (uint32_t i = 0; i < tmp.size(); ++i) {
		std::cout << tmp[i];

		if (lives[i].size()) {
			std::cout << "  ";
			for (uint32_t j = 0; j < lives[i].size(); ++j) {
				std::cout << lives[i][j];
			}
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
#endif
}
