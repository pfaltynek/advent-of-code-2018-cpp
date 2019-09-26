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
	uint32_t i, j;
	size_t pos;

	nodes_.clear();
	fighters_.clear();

	// fill adjacent nodes diffs
	adjacent_diffs.clear();
	// {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
	adjacent_diffs.push_back(coord_str(0, -1));
	adjacent_diffs.push_back(coord_str(-1, 0));
	adjacent_diffs.push_back(coord_str(1, 0));
	adjacent_diffs.push_back(coord_str(0, 1));

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
				coord_str coord = coord_str(j,i);

				node.init(j, i, type);
				nodes_[coord] = node;
				if (type != '.') {
					fighters_.push_back(coord);
				}
			}
		}
	}

	return true;
}

Combat::is_wall(int32_t x, int32_t y) {
	return is_wall(coord_str(x, y));
}

Combat::is_wall(coord_str coord) {
	return (nodes_.count(coord) == 0);
}

char Combat::get_opponent_type(char node_type) {
	return (node_type == 'E' ? 'G' : 'E');
}

bool Combat::is_fighter(char node_type) {
	return (node_type == 'E') || (node_type == 'G');
}

bool Combat::sort_by_reading_order(const coord_str coord1, const coord_str coord2) {
	return compare_by_reading_order(coord1, coord2);
}

bool Combat::compare_by_reading_order(const int32_t f1x, const int32_t f1y, const int32_t f2x, const int32_t f2y) {
	if (f1y < f2y) {
		return true;
	} else if (f1y > f2y) {
		return false;
	} else {
		return (f1x < f2x);
	}
}

bool Combat::compare_by_reading_order(const coord_str coord1, const coord_str coord2) {
	return compare_by_reading_order(coord1.x, coord1.y, coord2.x, coord2.y);
}

void Combat::sort_fighters() {
	std::sort(fighters_.begin(), fighters_.end(), sort_by_reading_order);
}

bool Combat::have_opponents(char opp_type) {
	for (auto it = nodes_.begin(); it != nodes_.end(); it++) {
		if (it->second.get_type() == opp_type) {
			return true;
		}
	}

	return false;
}

uint32_t Combat::make_combat() {
	uint32_t remaining_hitpoints_sum = 0, rounds = 0;
	bool game_over = false;

	print_map("Initially:");

	while (!game_over) {
		game_over = one_round(remaining_hitpoints_sum);
		if (game_over) {
			print_map("End in round " + std::to_string(rounds + 1) + ":");
			break;
		} else {
			rounds++;

			print_map("After round " + std::to_string(rounds) + ":");
		}
	}

	return (rounds * remaining_hitpoints_sum);
}

bool Combat::one_round(uint32_t& remaining_hitpoints_sum) {
	uint32_t i, x, y;
	char op;

	sort_fighters();

	for (i = 0; i < fighters_.size(); ++i) {
		if (nodes_[fighters_[i]].get_type() == '.') {
			continue;
		}

		op = get_opponent_type(nodes_[fighters_[i]].get_type());

		if (!have_opponents(op)) { // check if finished
			remaining_hitpoints_sum = 0;

			for (auto it = nodes_.begin(); it!= nodes_.end(); it++) {
				if (is_fighter(it->second.get_type()) ){
					remaining_hitpoints_sum += it->second.get_hit_points();
				}
			}
			return true; // combat finished
		}

		// perform one fighter's turn
		one_turn(fighters_[i]);
	}

	// reselect remaining fighters with actual positions
	fighters_.clear();
	for (auto it = nodes_.begin(); it != nodes_.end(); it++) {
		if (is_fighter(it->second.get_type())) {
			fighters_.push_back(it->second.get_coord());
		}
	}

	return false; // combat not finished yet
}

bool Combat::one_turn(coord_str node_coords) {
	coord_str new_pos;

	if (attack(nodes_[node_coords])) {
		return false;
	}

	if (get_shortest_path(nodes_[node_coords], new_pos)) {
		nodes_[node_coords].swap(nodes_[new_pos]);

		attack(nodes_[new_pos]);
		return true; // signal that fighter has moved
	}

	return false;
}

bool Combat::get_shortest_path(Node from, coord_str& target) {
	coord_str coord, tmp;
	char op;
	path_info_str pi, pi_new;

	visited.clear();
	queue = {};
	distances.clear();
	matches.clear();

	coord = from.get_coord();
	pi.coord = coord;
	pi.step1_initialized = false;

	queue.push(pi);
	distances[coord] = 0;
	visited.emplace(coord);
	op = get_opponent_type(nodes_[coord].get_type());

	while (!queue.empty()) {
		pi = queue.front();
		queue.pop();

		for (uint32_t i = 0; i < adjacent_diffs.size(); ++i) {
			tmp = pi.coord + adjacent_diffs[i];

			if (is_wall(tmp) || visited.count(tmp)) {
				continue;
			}

			if (nodes_[tmp].get_type() == op) {
				matches.push_back(pi);
				continue;
			}

			if (nodes_[tmp].get_type() != '.') {
				continue;
			}

			pi_new = pi;
			pi_new.coord = tmp;
			if (!pi_new.step1_initialized) {
				pi_new.step1 = tmp;
				pi_new.step1_initialized = true;
			}
			distances[tmp] = distances[pi.coord] + 1;
			queue.push(pi_new);
			visited.emplace(tmp);
		}
	}

	if (matches.empty()) {
		target = coord_str(0, 0);
		return false;
	}

	path_info_str match;
	uint32_t distance = UINT32_MAX;
	for (uint32_t i = 0; i < matches.size(); ++i) {
		if (distances[matches[i].coord] < distance) {
			distance = distances[matches[i].coord];
			match = matches[i];
		} else if (distances[matches[i].coord] == distance) {
			if (compare_by_reading_order(matches[i].coord, match.coord)) {
				distance = distances[matches[i].coord];
				match = matches[i];
			}
		}
	}

	target = match.step1;
	return true;
}

bool Combat::attack(Node attacker) {
	coord_str coord, nc, target_coord;
	bool found = false;
	uint32_t hp = UINT32_MAX;
	char op = get_opponent_type(attacker.get_type());

	nc = attacker.get_coord();

	for (uint32_t i = 0; i < adjacent_diffs.size(); ++i) {
		coord = nc + adjacent_diffs[i];
		if (!is_wall(coord)) {
			if (nodes_[coord].get_type() == op) {
				if (nodes_[coord].get_hit_points() < hp) {
					found = true;
					hp = nodes_[coord].get_hit_points();
					target_coord = coord;
				}
			}
		}
	}

	if (found) {
		nodes_[target_coord].got_attacked(attacker.get_attack_power());
		return true;
	}

	return false;
}

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
				std::cout << nodes_[coord_str(j, i)].get_type();
				if (is_fighter(nodes_[coord_str(j, i)].get_type())) {
					if (lives.empty()) {
						lives = "  ";
					}
					lives.append(" ");
					lives += nodes_[coord_str(j, i)].get_type();
					lives.append("(");
					lives.append(std::to_string(nodes_[coord_str(j, i)].get_hit_points()) + ")");
				}
			}
		}
		std::cout << lives << std::endl;
	}
	std::cout << std::endl;
#endif
}
