#include "main.hpp"

const std::regex immune_header_regex("^Immune System:$");
const std::regex infection_header_regex("^Infection:$");
const std::regex group_regex("^(\\d+) units each with (\\d+) hit points (\\(.*\\) )?with an attack that does (\\d+) (.*) damage at initiative (\\d+)$");
const std::regex attack_type_regex("^(bludgeoning|cold|radiation|slashing|fire)$");
const std::regex immune_regex("^\\(immune to (.*)\\) $");
const std::regex immune_weak_regex("^\\(immune to (.*); weak to (.*)\\) $");
const std::regex weak_immune_regex("^\\(weak to (.*); immune to (.*)\\) $");
const std::regex weak_regex("^\\(weak to (.*)\\) $");

typedef enum AREA_TYPE { bludgeoning = 0, cold = 1, fire = 2, slashing = 3, radiation = 4 } attack_type_t;

typedef struct GROUP {
	int32_t units;
	int32_t hit_points;
	int32_t attack_damage;
	int32_t initiative;
	attack_type_t attack_type;
	std::vector<attack_type_t> weaknesses, immunities;
	int32_t number;
	bool is_infection;

	int32_t get_effective_power() {
		return units * attack_damage;
	}

	void print() {
		std::cout << "Group " << number << " contains " << units << "units (ep " << get_effective_power() << ")" << std::endl;
	}
} group_str;

class ImmuneSystemSimulator {
  public:
	bool init(const std::vector<std::string> input);
	bool init();
	int32_t simulate();

  private:
	bool init_attack_type(const std::string input, attack_type_t& attack_type);
	bool init_weaknees_immunity(const std::string input, std::vector<attack_type_t>& weakness, std::vector<attack_type_t>& immunity);
	bool init_attack_type_list(const std::string input, std::vector<attack_type_t>& list);
	std::vector<group_str> groups_;
};

bool ImmuneSystemSimulator::init_attack_type(const std::string input, attack_type_t& attack_type) {
	std::smatch sm;

	if (std::regex_match(input, sm, attack_type_regex)) {
		switch (sm.str(1)[0]) {
			case 'b':
				attack_type = bludgeoning;
				break;
			case 'c':
				attack_type = cold;
				break;
			case 'r':
				attack_type = radiation;
				break;
			case 's':
				attack_type = slashing;
				break;
			case 'f':
				attack_type = fire;
				break;
			default:
				return false;
				break;
		}
		return true;
	} else {
		return false;
	}
}

bool ImmuneSystemSimulator::init_attack_type_list(const std::string input, std::vector<attack_type_t>& list) {
	std::smatch sm;
	attack_type_t type;
	std::string in = input;

	std::string delimiter = ", ";

	size_t pos = 0;
	std::string token;
	while ((pos = in.find(delimiter)) != std::string::npos) {
		token = in.substr(0, pos);
		in.erase(0, pos + delimiter.length());

		if (init_attack_type(token, type)) {
			list.push_back(type);
		} else {
			return false;
		}
	}

	if (init_attack_type(in, type)) {
		list.push_back(type);
	} else {
		return false;
	}

	return true;
}

bool ImmuneSystemSimulator::init_weaknees_immunity(const std::string input, std::vector<attack_type_t>& weakness, std::vector<attack_type_t>& immunity) {
	std::smatch sm;
	std::string immune, weak;

	if (std::regex_match(input, sm, weak_immune_regex)) {
		weak = sm.str(1);
		if (!init_attack_type_list(weak, weakness)) {
			return false;
		}
		immune = sm.str(2);
		if (!init_attack_type_list(immune, immunity)) {
			return false;
		}
	} else if (std::regex_match(input, sm, immune_weak_regex)) {
		immune = sm.str(1);
		if (!init_attack_type_list(immune, immunity)) {
			return false;
		}
		weak = sm.str(2);
		if (!init_attack_type_list(weak, weakness)) {
			return false;
		}
	} else if (std::regex_match(input, sm, immune_regex)) {
		immune = sm.str(1);
		if (!init_attack_type_list(immune, immunity)) {
			return false;
		}
		weakness.clear();
	} else if (std::regex_match(input, sm, weak_regex)) {
		weak = sm.str(1);
		if (!init_attack_type_list(weak, weakness)) {
			return false;
		}
		immunity.clear();
	} else {
		return false;
	}

	return true;
}

bool ImmuneSystemSimulator::init(const std::vector<std::string> input) {
	std::smatch sm;
	group_str group;
	bool immune_section;
	bool in_section = false;
	int32_t immunes = 0, infects = 0;

	groups_.clear();

	for (uint32_t i = 0; i < input.size(); i++) {
		group = {};

		if (input[i].empty()) {
			continue;
		}

		if (std::regex_match(input[i], sm, immune_header_regex)) {
			immune_section = true;
			in_section = true;
			continue;
		} else if (std::regex_match(input[i], sm, infection_header_regex)) {
			immune_section = false;
			in_section = true;
			continue;
		} else if (std::regex_match(input[i], sm, group_regex)) {
			if (!in_section) {
				std::cout << "Unknown section at input start" << std::endl;
				return false;
			}

			group.units = stoi(sm.str(1));
			group.hit_points = stoi(sm.str(2));
			group.attack_damage = stoi(sm.str(4));
			group.initiative = stoi(sm.str(6));
			group.is_infection = !immune_section;

			if (!init_attack_type(sm.str(5), group.attack_type)) {
				std::cout << "Unknown attack type at line " << i + 1 << std::endl;
				return false;
			}
			if (!sm.str(3).empty()) {
				if (!init_weaknees_immunity(sm.str(3), group.weaknesses, group.immunities)) {
					std::cout << "Unknown weakness/immunity at line " << i + 1 << std::endl;
					return false;
				}
			}
		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		if (immune_section) {
			immunes++;
			group.number = immunes;
		} else {
			infects++;
			group.number = infects;
		}
		groups_.push_back(group);
	}

	if (!immunes || !infects) {
		std::cout << "Input incomplette (missing one section)" << std::endl;
		return false;
	}

	return true;
}

bool ImmuneSystemSimulator::init() {
	std::ifstream input;
	std::string line;
	std::vector<std::string> lines;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return false;
	}

	lines.clear();

	while (std::getline(input, line)) {
		lines.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}

	return init(lines);
}

static int sort_groups_by_effective_power(group_str first, group_str second) {
	int32_t f, s;

	f = first.get_effective_power();
	s = second.get_effective_power();
	if (f == s) {
		return (first.initiative > second.initiative);
	} else {
		return (f > s);
	}
}

int32_t ImmuneSystemSimulator::simulate() {
	std::vector<uint32_t> imm, inf;

	imm.clear();
	inf.clear();

	std::sort(groups_.begin(), groups_.end(), sort_groups_by_effective_power);

	for (uint32_t i = 0; i < groups_.size(); ++i) {
		if (groups_[i].is_infection) {
			inf.push_back(i);
		} else {
			imm.push_back(i);
		}
	}

	std::cout << "Immune System:" << std::endl;
	for (uint32_t i = 0; i < imm.size(); ++i) {
		groups_[imm[i]].print();
	}

	std::cout << "Infection:" << std::endl;
	for (uint32_t i = 0; i < inf.size(); ++i) {
		groups_[inf[i]].print();
	}
	std::cout << std::endl;
	
	return 0;
}

int main(void) {
	int32_t result1 = 0, result2 = 0;
	ImmuneSystemSimulator iss;

#if TEST
	if (!iss.init(
			{"Immune System:", "17 units each with 5390 hit points (weak to radiation, bludgeoning) with an attack that does 4507 fire damage at initiative 2",
			 "989 units each with 1274 hit points (immune to fire; weak to bludgeoning, slashing) with an attack that does 25 slashing damage at initiative 3",
			 "", "Infection:", "801 units each with 4706 hit points (weak to radiation) with an attack that does 116 bludgeoning damage at initiative 1",
			 "4485 units each with 2961 hit points (immune to radiation; weak to fire, cold) with an attack that does 12 slashing damage at initiative 4"})) {
		return -1;
	}

	result1 = iss.simulate();
/*
	if (!iss.init(
			{"pos=<10,12,12>, r=2", "pos=<12,14,12>, r=2", "pos=<16,12,12>, r=4", "pos=<14,14,14>, r=6", "pos=<50,50,50>, r=200", "pos=<10,10,10>, r=5"})) {
		return -1;
	}

	result2 = 2;
*/
#endif

	if (!iss.init()) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 24 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
