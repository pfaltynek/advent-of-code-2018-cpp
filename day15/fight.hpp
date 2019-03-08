#ifndef FIGHT_HPP
#define FIGHT_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fighter.hpp"
#include <utility>

class Fight {
  public:
	Fight(){};

	bool init();
	bool init(std::vector<std::string> test_data);
	int make_fight();
	void one_turn(Fighter f);
	void one_round();

  protected:
  private:
	std::vector<std::string> _map, _tmp_map;
	std::vector<Fighter> _fighters;
	bool decode_map_input(const std::vector<std::string> input);
	void sort_fighters();
	static bool compare_fighters_position(const Fighter f1, const Fighter f2);
	void place_fighters_and_get_enemies(const Fighter f, std::vector<Fighter> &enemies);
	void get_targets_of_enemies(const std::vector<Fighter> &enemies, std::map<std::pair<uint32_t, uint32_t>, int> &targets);
};

#endif // FIGHT_HPP
