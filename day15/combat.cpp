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
				nodes_[coord_pack(j, i)] = node;
				if (type != '.') {
					fighters_.push_back(node);
				}
			}
		}
	}

	return true;
}

Combat::is_wall(uint32_t x, uint32_t y) {
	return is_wall(coord_pack(x, y));
}

Combat::is_wall(uint64_t coord) {
	return (nodes_.count(coord) == 0);
}

char Combat::get_opponent_type(char node_type) {
	return (node_type == 'E' ? 'G' : 'E');
}

bool Combat::is_fighter(char node_type) {
	return (node_type == 'E') || (node_type == 'G');
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

bool Combat::compare_by_reading_order(const uint64_t coord1, const uint64_t coord2) {
	uint32_t f1x, f1y, f2x, f2y;

	coord_unpack(coord1, f1x, f1y);
	coord_unpack(coord2, f2x, f2y);
	return compare_by_reading_order(f1x, f1y, f2x, f2y);
}

void Combat::sort_fighters() {
	std::sort(fighters_.begin(), fighters_.end(), sort_by_reading_order);
}

bool Combat::have_opponents(char opp_type) {
	for (uint32_t i = 0; i < fighters_.size(); ++i) {
		if (fighters_[i].get_type() == opp_type) {
			return true;
		}
	}

	return false;
}

uint32_t Combat::make_combat() {
	uint32_t remaining_hitpoints_sum = 0, rounds = 0;
	bool game_over = false;

	// fill adjacent nodes diffs
	adjacent_diffs.clear();
	// {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
	adjacent_diffs.push_back(coord_pack(0, 0xFFFFFFFF));
	adjacent_diffs.push_back(coord_pack(0xFFFFFFFF, 0));
	adjacent_diffs.push_back(coord_pack(1, 0));
	adjacent_diffs.push_back(coord_pack(0, 1));

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
		if (fighters_[i].get_type() == '.') {
			continue;
		}

		op = get_opponent_type(fighters_[i].get_type());

		if (!have_opponents(op)) { // check if finished
			remaining_hitpoints_sum = 0;

			for (uint32_t j = 0; j < fighters_.size(); ++j) {
				remaining_hitpoints_sum += fighters_[j].get_hit_points();
			}
			return true; // combat finished
		}

		// perform one fighter's turn
		if (one_turn(coord_pack(fighters_[i].get_x(), fighters_[i].get_y()), x, y)) {
			fighters_[i].moved_to(x, y);
		}
	}

	// remove dead fighters
	i = 0;
	while (i < fighters_.size()) {
		if (fighters_[i].get_type() == '.') {
			fighters_.erase(fighters_.begin() + i);
		} else {
			i++;
		}
	}

	return false; // combat not finished yet
}

bool Combat::one_turn(uint64_t node_coords, uint32_t& new_x, uint32_t& new_y) {
	uint32_t x, y;
	Node node = nodes_[node_coords];

	if (attack(node)) {
		return false;
	}

	if (get_shortest_path(node, x, y)) {
		// TODO: correct this!
		/*
		uint64_t new_coord = coord_pack(x, y);
		Node tmp(node);
		nodes_[new_coord].type

			attack(nodes_[coord_pack(x, y)]);
		*/
		return true; // signal that fighter has moved
	}

	return false;
}

bool Combat::get_shortest_path(Node from, uint32_t& target_x, uint32_t& target_y) {
	uint64_t coord, tmp;
	char op;

	visited.clear();
	queue = {};
	distances.clear();
	matches.clear();

	coord = coord_pack(from.get_x(), from.get_y());
	queue.push(coord);
	distances[coord] = 0;
	visited.emplace(coord);
	op = get_opponent_type(nodes_[coord].get_type());

	while (!queue.empty()) {
		coord = queue.front();
		queue.pop();

		for (uint32_t i = 0; i < adjacent_diffs.size(); ++i) {
			tmp = coord + adjacent_diffs[i];

			if (is_wall(tmp) || visited.count(tmp)) {
				continue;
			}

			if (nodes_[tmp].get_type() == op) {
				matches.push_back(coord);
				continue;
			}

			if (nodes_[tmp].get_type() != '.'){
				continue;
			}

			distances[tmp] = distances[coord] + 1;
			queue.push(tmp);
			visited.emplace(tmp);
		}
	}

	if (matches.empty()){
		target_x = 0;
		target_y = 0;
		return false;
	}

	uint64_t match;
	uint32_t distance = distances[matches[0]];
	for (uint32_t i = 1; i < matches.size(); ++i){
		if (distances[matches[i]] < distance){
			distance = distances[matches[i]];
			match = matches[i];
		} else if (distances[matches[i]] == distance) {
			if (compare_by_reading_order(match, matches[i])){
				distance = distances[matches[i]];
				match = matches[i];
			}
		}
	}

	coord_unpack(match, target_x, target_y);
	return true;
}

bool Combat::attack(Node attacker) {
	uint64_t coord, nc, target_coord;
	bool found = false;
	uint32_t hp = UINT32_MAX;
	char op = get_opponent_type(attacker.get_type());

	nc = coord_pack(attacker.get_x(), attacker.get_y());

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
				std::cout << nodes_[coord_pack(j, i)].get_type();
				if (is_fighter(nodes_[coord_pack(j, i)].get_type())) {
					if (lives.empty()) {
						lives = "  ";
					}
					lives.append(" ");
					lives += nodes_[coord_pack(j, i)].get_type();
					lives.append("(");
					lives.append(std::to_string(nodes_[coord_pack(j, i)].get_hit_points()) + ")");
				}
			}
		}
		std::cout << lives << std::endl;
	}
	std::cout << std::endl;
#endif
}
