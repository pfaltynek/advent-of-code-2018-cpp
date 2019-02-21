#ifndef FIGHT_HPP
#define FIGHT_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fighter.hpp"

class Fight {
  public:
	Fight(){};

	bool init();
	bool init(std::vector<std::string> test_data);
	int make_fight();
	void one_turn();
	void one_round();

  protected:
  private:
	std::vector<std::string> _map;
	std::vector<Fighter> _fighters;
	bool decode_map_input(std::vector<std::string> input);
	void sort_fighters();
	void split_targets(std::vector<Fighter> &elfs, std::vector<Fighter> &goblins);
	static bool compare_fighters_position(Fighter f1, Fighter f2);
};

#endif // FIGHT_HPP
