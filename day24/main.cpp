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
	uint32_t index;
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
		std::cout << "Group " << number << " contains " << units << " units (ep " << get_effective_power() << ")" << std::endl;
	}

	void print_possible_attack_target(GROUP possible_target, int32_t possible_damage) {
		if (is_infection) {
			std::cout << "Infection";
		} else {
			std::cout << "Immune System";
		}
		std::cout << " group " << number << " would deal defending group " << possible_target.number << " " << possible_damage << " damage" << std::endl;
	}

	void print_attack_info(GROUP target, int32_t killed) {
		if (is_infection) {
			std::cout << "Infection";
		} else {
			std::cout << "Immune System";
		}
		std::cout << " group " << number << " attacks defending group " << target.number << ", killing " << killed << " units" << std::endl;
	}

	bool is_immune_to(attack_type_t attack_type) {
		for (uint32_t i = 0; i < immunities.size(); ++i) {
			if (immunities[i] == attack_type) {
				return true;
			}
		}
		return false;
	}

	bool is_weak_to(attack_type_t attack_type) {
		for (uint32_t i = 0; i < weaknesses.size(); ++i) {
			if (weaknesses[i] == attack_type) {
				return true;
			}
		}
		return false;
	}

	int compare_as_target(GROUP other) {
		int ep, ep_other;

		ep = get_effective_power();
		ep_other = other.get_effective_power();

		if (ep > ep_other) {
			return 1;
		} else if (ep == ep_other) {
			if (initiative > other.initiative) {
				return 1;
			} else if (initiative == other.initiative) {
				return 0;
			}
		}
		return -1;
	}

	int32_t get_damage(GROUP target) {
		int32_t dmg = get_effective_power();

		if (target.is_immune_to(attack_type)) {
			dmg = 0;
		}
		if (target.is_weak_to(attack_type)) {
			dmg *= 2;
		}

		return dmg;
	}

	int32_t got_damaged_by(int32_t damage) {
		int32_t killed = damage / hit_points;
		if (units >= killed) {
			units -= killed;
		} else {
			killed = units;
			units = 0;
		}

		return killed;
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
	void target_selection(const std::vector<uint32_t>& attackers, std::vector<uint32_t> defenders, std::map<uint32_t, uint32_t>& attack_plan);

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
		group.index = groups_.size();
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

static int sort_groups_by_iniciative(group_str first, group_str second) {
	return (first.initiative > second.initiative);
}

int32_t ImmuneSystemSimulator::simulate() {
	std::vector<uint32_t> imm, inf, all;
	std::map<uint32_t, uint32_t> attack_plan;
	std::vector<group_str> groups;
	uint32_t idx;
	bool finished = false;

	while (true) {
		groups = groups_;
		idx = 0;

		while (idx < groups.size()) {
			if (groups[idx].units <= 0) {
				groups.erase(groups.begin() + idx);
			} else {
				idx++;
			}
		}

		imm.clear();
		inf.clear();

		for (uint32_t i = 0; i < groups.size(); ++i) {
			if (groups[i].is_infection) {
				inf.push_back(groups[i].index);
			} else {
				imm.push_back(groups[i].index);
			}
		}

		std::cout << "Immune System:" << std::endl;
		if (imm.empty()) {
			std::cout << "No groups remain." << std::endl;
			finished = true;
		} else {
			for (uint32_t i = 0; i < imm.size(); ++i) {
				groups_[imm[i]].print();
			}
		}

		std::cout << "Infection:" << std::endl;
		if (inf.empty()) {
			std::cout << "No groups remain." << std::endl;
			finished = true;
		} else {
			for (uint32_t i = 0; i < inf.size(); ++i) {
				groups_[inf[i]].print();
			}
		}
		std::cout << std::endl;

		if (finished) {
			int32_t result = 0;
			for (uint32_t i = 0; i < groups.size(); ++i) {
				result += groups[i].units;
			}

			return result;
		}

		imm.clear();
		inf.clear();

		std::sort(groups.begin(), groups.end(), sort_groups_by_effective_power);

		for (uint32_t i = 0; i < groups.size(); ++i) {
			if (groups[i].is_infection) {
				inf.push_back(groups[i].index);
			} else {
				imm.push_back(groups[i].index);
			}
		}

		attack_plan.clear();

		target_selection(inf, imm, attack_plan);
		target_selection(imm, inf, attack_plan);

		std::sort(groups.begin(), groups.end(), sort_groups_by_iniciative);

		std::cout << std::endl;

		for (uint32_t i = 0; i < groups.size(); ++i) {
			uint32_t idx = groups[i].index;
			if (groups_[idx].units) {
				if (attack_plan.count(idx)) {
					uint32_t idxt = attack_plan[idx];
					int32_t damage = groups_[idx].get_damage(groups_[idxt]);
					int32_t killed = groups_[idxt].got_damaged_by(damage);
					groups_[idx].print_attack_info(groups_[idxt], killed);
				}
			}
		}

		std::cout << std::endl;
	}
}

void ImmuneSystemSimulator::target_selection(const std::vector<uint32_t>& attackers, std::vector<uint32_t> defenders,
											 std::map<uint32_t, uint32_t>& attack_plan) {
	int32_t damage, dmg, tmp_idx, tmp_grp_idx;
	group_str attacker, target, adept;
	bool target_found;

	for (uint32_t i = 0; i < attackers.size(); ++i) {
		attacker = groups_[attackers[i]];
		target_found = false;
		damage = -1;
		tmp_idx = -1;
		for (uint32_t j = 0; j < defenders.size(); ++j) {
			adept = groups_[defenders[j]];
			dmg = attacker.get_damage(adept);
			attacker.print_possible_attack_target(adept, dmg);

			if (target_found) {
				if (dmg < damage) {
					continue;
				} else if (dmg == damage) {
					int comparation = target.compare_as_target(adept);

					if (comparation > 0) {
						continue;
					} else if (comparation == 0) {
						target_found = false;
						continue;
					}
				}
			} else {
				if (dmg <= damage) {
					continue;
				}
			}

			target_found = true;
			target = adept;
			tmp_idx = j;
			tmp_grp_idx = adept.index;
			damage = dmg;
		}

		if (target_found) {
			attack_plan[attacker.index] = tmp_grp_idx;
			defenders.erase(defenders.begin() + tmp_idx);
		}
	}
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

	result1 = iss.simulate();

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
