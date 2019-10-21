#include "main.hpp"

const std::regex immune_header_regex("^Immune System:$");
const std::regex infection_header_regex("^Infection:$");
const std::regex group_regex("^(\\d+) units each with (\\d+) hit points \\((.*)\\) with an attack that does (\\d+) (.*) damage at initiative (\\d+)$");
// const std::regex nanobot_regex("^pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(\\d+)$");

typedef enum AREA_TYPE { bludgeoning = 0, cold = 1, fire = 2, slashing = 3, radiation = 4 } attack_type_t;

typedef struct GROUP {
	int32_t units;
	int32_t hit_points;
	int32_t attack_damage;
	int32_t initiative;
	attack_type_t attack_type;
	std::vector<attack_type_t> weaknesses, immunities;
	int32_t number;
} group_str;

class ImmuneSystemSimulator {
  public:
	bool init(const std::vector<std::string> input);
	bool init();

  private:
	bool init_attack_type(const std::string input, attack_type_t& attack_type);
	bool init_weaknees_immunity(const std::string input, std::vector<attack_type_t>& weakness, std::vector<attack_type_t>& immunity);
	std::vector<group_str> immune_, infection_;
};

bool ImmuneSystemSimulator::init_attack_type(const std::string input, attack_type_t& attack_type) {
	return false;
}
bool ImmuneSystemSimulator::init_weaknees_immunity(const std::string input, std::vector<attack_type_t>& weakness, std::vector<attack_type_t>& immunity) {
	return false;
}

bool ImmuneSystemSimulator::init(const std::vector<std::string> input) {
	std::smatch sm;
	group_str group;
	bool immune_section;
	bool in_section = false;

	immune_.clear();
	infection_.clear();

	for (uint32_t i = 0; i < input.size(); i++) {
		group = {};

		if (std::regex_match(input[i], sm, immune_header_regex)) {
			immune_section = true;
		} else if (std::regex_match(input[i], sm, infection_header_regex)) {
			immune_section = false;
		} else if (std::regex_match(input[i], sm, group_regex)) {
			if (!in_section) {
				std::cout << "Unknown section at input start" << std::endl;
				return false;
			}

			group.units = stoi(sm.str(1));
			group.hit_points = stoi(sm.str(2));
			group.attack_damage = stoi(sm.str(4));
			group.initiative = stoi(sm.str(6));

			if (!init_attack_type(sm.str(3), group.attack_type)) {
				return false;
			}
			if (!init_weaknees_immunity(sm.str(5), group.weaknesses, group.immunities)) {
				return false;
			}
		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		if (immune_section) {
			immune_.push_back(group);
		} else {
			infection_.push_back(group);
		}
	}

	if (immune_.empty() || infection_.empty()) {
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
		if (!line.empty()) {
			lines.push_back(line);
		}
	}

	if (input.is_open()) {
		input.close();
	}

	return init(lines);
}

int main(void) {
	int32_t result1 = 0, result2 = 0;
	ImmuneSystemSimulator iss;

#if TEST
	if (!iss.init({"pos=<0,0,0>, r=4", "pos=<1,0,0>, r=1", "pos=<4,0,0>, r=3", "pos=<0,2,0>, r=1", "pos=<0,5,0>, r=3", "pos=<0,0,3>, r=1", "pos=<1,1,1>, r=1",
				   "pos=<1,1,2>, r=1", "pos=<1,3,1>, r=1"})) {
		return -1;
	}

	result1 = 1;

	if (!iss.init(
			{"pos=<10,12,12>, r=2", "pos=<12,14,12>, r=2", "pos=<16,12,12>, r=4", "pos=<14,14,14>, r=6", "pos=<50,50,50>, r=200", "pos=<10,10,10>, r=5"})) {
		return -1;
	}

	result2 = 2;

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
