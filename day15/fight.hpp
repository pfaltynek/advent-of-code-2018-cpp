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
	std::vector<std::string> _map, _no_fighters;
	std::vector<Fighter> _fighters;
	bool decode_map_input(std::vector<std::string> input);
	void sort_fighters();
	static bool compare_fighters_position(Fighter f1, Fighter f2);
};

#endif // FIGHT_HPP
