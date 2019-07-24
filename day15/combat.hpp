#ifndef Combat_HPP
#define Combat_HPP

#include "fighter.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "helpers.h"
#include <queue>

typedef struct PathInfo {
	direction_t first_step;
	uint32_t x;
	uint32_t y;
	uint32_t steps;
	std::string path;
} path_info_str;

class Combat {
  public:
	Combat(){};

	bool init();
	bool init(std::vector<std::string> test_data);
	uint32_t make_combat();

#if (TEST1 || TEST2)
		void sort_fighters();
  protected:
  private:
#else
  protected:
  private:
		void sort_fighters();
#endif
	std::vector<std::string> _map, _tmp_map;
	std::vector<Fighter> _fighters;

	bool decode_map_input(const std::vector<std::string> input);
	bool one_round(uint32_t &remaining_hitpoints_sum);
	void one_turn(Fighter f);
	static bool compare_fighters_position(const Fighter f1, const Fighter f2);
	void place_fighters_and_get_enemies(const Fighter f, std::vector<Fighter> &enemies);
	void get_targets_of_enemies(const std::vector<Fighter> &enemies, std::map<std::pair<uint32_t, uint32_t>, int> &targets);
	int32_t get_shortest_path(Fighter from, Fighter to, direction_t &start_direction);
	std::vector<std::pair<uint32_t, uint32_t>> get_free_adjacents(Fighter f);
	std::vector<std::pair<uint32_t, uint32_t>> get_free_adjacents(uint32_t x, uint32_t y);
	bool test(std::pair<uint32_t, uint32_t> next, std::pair<uint32_t, uint32_t> target, std::string &path, direction_t &start_directon, uint32_t &steps,
			  uint32_t &steps_max);
};
#endif // COMBAT_HPP
