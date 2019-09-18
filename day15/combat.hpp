#ifndef Combat_HPP
#define Combat_HPP

#include "fighter.hpp"
#include "helpers.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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
	std::vector<std::string> map_, tmp_map_;
	std::vector<Fighter> fighters_;

	bool decode_map_input(const std::vector<std::string> input);
	bool one_turn(Fighter &f);
	void place_fighters_and_get_enemies(const Fighter f, std::vector<uint32_t> &enemies);
	void get_targets_of_enemies(const std::vector<uint32_t> &enemies, std::map<std::pair<uint32_t, uint32_t>, int> &targets);
	bool get_shortest_path(Fighter from, uint32_t target_x, uint32_t target_y, uint32_t &x1, uint32_t &y1, uint32_t steps, uint32_t max_steps);
	std::vector<std::pair<uint32_t, uint32_t>> get_adjacents_ordered(Fighter f);
	std::vector<std::pair<uint32_t, uint32_t>> get_adjacents_ordered(uint32_t x, uint32_t y);
	std::vector<std::pair<uint32_t, uint32_t>> get_free_adjacents(Fighter f);
	std::vector<std::pair<uint32_t, uint32_t>> get_free_adjacents(uint32_t x, uint32_t y);
	bool attack_if_possible(Fighter &f, std::vector<uint32_t> &enemies);

	static bool compare_fighters_position(const Fighter f1, const Fighter f2);
};
#endif // COMBAT_HPP
