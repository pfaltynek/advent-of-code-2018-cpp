#ifndef Combat_HPP
#define Combat_HPP

#include "helpers.h"
#include "node.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
#include "coord.hpp"
#include <stdint.h>

typedef struct PATH_INFO {
	uint32_t x_;
	uint32_t y_;
	uint32_t x_1st_;
	uint32_t y_1st_;
} path_info_str;

class Combat {
  public:
	Combat(){};

	bool init();
	bool init(std::vector<std::string> test_data);
	uint32_t make_combat();

#if (TEST1 || TEST2)
	void sort_fighters();
	bool one_round(uint32_t &remaining_hitpoints_sum);

  protected:
  private:
#else
  protected:
  private:
	void sort_fighters();
	bool one_round(uint32_t &remaining_hitpoints_sum);
#endif
	std::map<uint64_t, Node> nodes_, tmp_map_;
	std::vector<Node> fighters_;
	uint32_t width_;
	uint32_t height_;
	std::unordered_set<uint64_t> visited;
	std::queue<uint64_t> queue;
	std::map<uint64_t, uint32_t> distances;
	std::vector<uint64_t> matches;
	std::vector<uint64_t> adjacent_diffs;

	is_wall(uint32_t x, uint32_t y);
	is_wall(uint64_t coord);
	bool is_fighter(char node_type);
	char get_opponent_type(char node_type);
	bool decode_map_input(const std::vector<std::string> input);
	bool one_turn(uint64_t node_coords, uint32_t& new_x, uint32_t& new_y);
	bool get_shortest_path(Node from, uint32_t &target_x, uint32_t &target_y);
	void print_map(std::string title);
	static bool sort_by_reading_order(const Node n1, const Node n2);
	static bool compare_by_reading_order(const uint32_t f1x, const uint32_t f1y, const uint32_t f2x, const uint32_t f2y);
	static bool compare_by_reading_order(const uint64_t coord1, const uint64_t coord2);
	bool have_opponents(char opp_type);
	bool attack(Node n);
};
#endif // COMBAT_HPP
