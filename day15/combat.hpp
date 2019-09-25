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
#include <set>
#include <utility>
#include <vector>
#include "coord.hpp"
#include <stdint.h>

typedef struct PATH_INFO {
	coord_str coord;
	coord_str step1;
	bool step1_initialized;
} path_info_str;

class Combat {
  public:
	Combat(){};

	bool init();
	bool init(std::vector<std::string> test_data);
	uint32_t make_combat();
#if TEST
	void test01();
	void test02();
	void test03();
	void test04();
	void test05();
	void test06();
	void test07();
	void test08();
	void test09();
#endif

  protected:
  private:
  protected:
  private:
	void sort_fighters();
	bool one_round(uint32_t &remaining_hitpoints_sum);
	std::map<coord_str, Node> nodes_;
	std::vector<Node> fighters_;
	uint32_t width_;
	uint32_t height_;
	std::set<coord_str> visited;
	std::queue<path_info_str> queue;
	std::map<coord_str, uint32_t> distances;
	std::vector<path_info_str> matches;
	std::vector<coord_str> adjacent_diffs;

	is_wall(int32_t x, int32_t y);
	is_wall(coord_str coord);
	bool is_fighter(char node_type);
	char get_opponent_type(char node_type);
	bool decode_map_input(const std::vector<std::string> input);
	bool one_turn(coord_str node_coords);
	bool get_shortest_path(Node from, coord_str &target);
	void print_map(std::string title);
	static bool sort_by_reading_order(const Node n1, const Node n2);
	static bool compare_by_reading_order(const int32_t f1x, const int32_t f1y, const int32_t f2x, const int32_t f2y);
	static bool compare_by_reading_order(const coord_str coord1, const coord_str coord2);
	bool have_opponents(char opp_type);
	bool attack(Node n);
};
#endif // COMBAT_HPP
