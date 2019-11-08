#ifndef Combat_HPP
#define Combat_HPP

#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include "node.hpp"
#include <map>
#include <queue>
#include <set>
#include <algorithm>

typedef struct PATH_INFO {
	coord_str coord;
	coord_str step1;
	bool step1_initialized;
} path_info_str;

#define TEST 0
#define DEBUG_PRINT 0

class AoC2018_day15 : public AoC {
  public:

  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int32_t make_combat_part1();
	int32_t make_combat_part2();
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
