#ifndef Combat_HPP
#define Combat_HPP

#include "coord.hpp"
#include "helpers.h"
#include "node.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

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
	int32_t make_combat_part1();
	int32_t make_combat_part2();
#if TEST1
	void test_part1_01();
	void test_part1_02();
	void test_part1_03();
	void test_part1_04();
	void test_part1_05();
	void test_part1_06();
	void test_part1_07();
	void test_part1_08();
	void test_part1_09();
#endif
#if TEST2
	void test_part2_01();
	void test_part2_02();
	void test_part2_03();
	void test_part2_04();
	void test_part2_05();
#endif
  protected:
  private:
  protected:
  private:
	void sort_fighters();
	bool one_round_part1(int32_t& remaining_hitpoints_sum);
	bool one_round_part2(int32_t& remaining_hitpoints_sum, bool& part2_failed);
	std::map<coord_str, Node> nodes_, nodes_backup_;
	std::vector<coord_str> fighters_, fighters_backup_;
	uint32_t width_;
	uint32_t height_;
	std::set<coord_str> visited;
	std::queue<path_info_str> queue;
	std::map<coord_str, uint32_t> distances;
	std::vector<path_info_str> matches;

	bool is_wall(int32_t x, int32_t y);
	bool is_wall(coord_str coord);
	bool is_fighter(char node_type);
	char get_opponent_type(char node_type);
	bool decode_map_input(const std::vector<std::string> input);
	bool one_turn(coord_str node_coords, bool& elf_killed);
	bool get_shortest_path(Node from, coord_str& target);
	void print_map(std::string title);
	static bool sort_by_reading_order(const coord_str coord1, const coord_str coord2);
	static bool compare_by_reading_order(const int32_t f1x, const int32_t f1y, const int32_t f2x, const int32_t f2y);
	static bool compare_by_reading_order(const coord_str coord1, const coord_str coord2);
	bool have_opponents(char opp_type);
	bool attack(Node n, coord_str& victim, bool& part2_failed);
	void restart(int32_t elf_attack_power);
};
#endif // COMBAT_HPP
