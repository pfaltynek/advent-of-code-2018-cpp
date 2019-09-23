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
	uint32_t i, j;
	size_t pos;

	nodes.clear();
	fighters_.clear();

	height_ = map.size();

	for (i = 0; i < map.size(); ++i) {
		if (i) {
			if (map[i].size() != width_) {
				std::cout << "Invalid input map line " << i + 1 << std::endl;
				return false;
			}
		} else {
			width_ = map[i].size();
		}

		pos = map[i].find_first_not_of("EG#.");
		if (pos != std::string::npos) {
			std::cout << "Invalid input map content at line " << i + 1 << std::endl;
			return false;
		}

		for (j = 0; j < width_; ++j) {
			char type = map[i][j];

			if (type != '#') {
				Node node;

				node.init(j, i, type);
				nodes[coord_pack(j, i)] = node;
				if (type != '.') {
					fighters_.push_back(node);
				}
			}
		}
	}

	return true;
}

Combat::is_wall(uint32_t x, uint32_t y) {
	return (nodes.count(coord_pack(x, y)) == 0);
}

bool Combat::sort_by_reading_order(const Node n1, const Node n2) {
	return compare_by_reading_order(n1.get_x(), n1.get_y(), n2.get_x(), n2.get_y());
}

bool Combat::compare_by_reading_order(const uint32_t f1x, const uint32_t f1y, const uint32_t f2x, const uint32_t f2y) {
	if (f1y < f2y) {
		return true;
	} else if (f1y > f2y) {
		return false;
	} else {
		return (f1x < f2x);
	}
}

void Combat::sort_fighters() {
	std::sort(fighters_.begin(), fighters_.end(), sort_by_reading_order);
}

uint32_t Combat::make_combat() {
	uint32_t remaining_hitpoints_sum = 0, rounds = 0;
	/*
		print_map("Initially:");

		while (true) {
			if (one_round(remaining_hitpoints_sum)) {
				print_map("End in round " + std::to_string(rounds + 1) + ":");
				break;
			} else {
				rounds++;

				print_map("After round " + std::to_string(rounds) + ":");
	#if TEST3
				if (rounds >= 3) {
					break;
				}
	#endif
			}
		}
	*/
	return (rounds * remaining_hitpoints_sum);
}
/*
bool Combat::one_round(uint32_t &remaining_hitpoints_sum) {
	uint32_t idx, i;

	sort_fighters();

#if TEST1 || TEST2
	print_map("Initially:");
#endif

	i = 0;
	while (i < fighters_.size()) {
		if (one_turn(fighters_[i])) {
			remaining_hitpoints_sum = 0;
			for (auto it = fighters_.begin(); it != fighters_.end(); ++it) {
				remaining_hitpoints_sum += it->get_hit_points();
			}
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

#if TEST1 || TEST2
	print_map("After round 1:");
#endif

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
	std::unordered_set<uint32_t> visited;
	bool found = false;

	steps = max_steps;

	visited.clear();
	visited.emplace(get_coord(from.get_x(), from.get_y()));

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
			uint32_t coords = get_coord(it->first, it->second);

			if (visited.find(coords) == visited.end()) {
				visited.emplace(coords);
				PathInfo pi(tmp);
				pi.move_to(it->first, it->second);
				uint32_t steps_new = pi.get_steps();

				if ((pi.get_x() == target_x) && (pi.get_y() == target_y)) {
					if (steps_new < steps) {
						x1 = pi.get_x_1st();
						y1 = pi.get_y_1st();
						steps = steps_new;
						found = true;
						// return true;
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
*/
void Combat::print_map(std::string title) {
#if DEBUG_PRINT
	std::string lives;

	if (!title.empty()) {
		std::cout << title << std::endl;
	}

	for (uint32_t i = 0; i < height_; ++i) {
		lives.clear();
		for (uint32_t j = 0; j < width_; j++) {
			if (is_wall(j, i)) {
				std::cout << '#';
			} else {
				std::cout << nodes[coord_pack(j, i)].get_type();
				if ((nodes[coord_pack(j, i)].get_type() == 'E') || (nodes[coord_pack(j, i)].get_type() == 'G')) {
					if (lives.empty()) {
						lives = "  ";
					}
					lives.append(" ");
					lives+=nodes[coord_pack(j, i)].get_type();
					lives.append("(");
					lives.append(std::to_string(nodes[coord_pack(j, i)].get_hit_points()) + ")");
				}
			}
		}
		std::cout << lives<< std::endl;
	}
	std::cout << std::endl;
#endif
}
