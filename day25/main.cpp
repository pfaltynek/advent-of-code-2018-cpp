#include "main.hpp"

const std::regex point_regex("^(-?\\d+),(-?\\d+),(-?\\d+),(-?\\d+)$");

typedef struct POINT {
	int32_t x, y, z, q;

	int32_t get_distance() {
		return abs(x) + abs(y) + abs(z) + abs(q);
	}
	int32_t get_distance(POINT other) {
		return abs(x - other.x) + abs(y - other.y) + abs(z - other.z) + abs(q - other.q);
	}
} point_str;

class Adventure4D {
  public:
	bool init(const std::vector<std::string> input);
	bool init();
	int32_t find_constellation();

  private:
	std::vector<point_str> points_;
};

bool Adventure4D::init(const std::vector<std::string> input) {
	std::smatch sm;
	point_str pt;

	points_.clear();

	for (uint32_t i = 0; i < input.size(); i++) {
		pt = {};

		if (input[i].empty()) {
			continue;
		}

		if (std::regex_match(input[i], sm, point_regex)) {
			pt.x = stoi(sm.str(1));
			pt.y = stoi(sm.str(2));
			pt.z = stoi(sm.str(3));
			pt.q = stoi(sm.str(4));
		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		points_.push_back(pt);
	}

	return true;
}

bool Adventure4D::init() {
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

int32_t Adventure4D::find_constellation() {
	std::vector<std::vector<point_str>> constellations;
	std::vector<int32_t> idxs;

	constellations.clear();

	for (uint32_t i = 0; i < points_.size(); i++) {
		idxs.clear();

		for (uint32_t j = 0; j < constellations.size(); j++) {
			for (uint32_t k = 0; k < constellations[j].size(); k++) {
				if (constellations[j][k].get_distance(points_[i]) <= 3) {
					idxs.push_back(j);
					break;
				}
			}
		}

		if (idxs.size()) {
			uint32_t main_idx = idxs[0];

			constellations[main_idx].push_back(points_[i]);
			idxs.erase(idxs.begin());
			std::sort(idxs.begin(), idxs.end());
			for (uint32_t j = 0; j < idxs.size(); j++) {
				constellations[main_idx].insert(constellations[main_idx].end(), constellations[idxs[j]].begin(), constellations[idxs[j]].end());
				constellations.erase(constellations.begin() + idxs[j]);
			}
		} else {
			std::vector<point_str> tmp;

			tmp.clear();
			tmp.push_back(points_[i]);
			constellations.push_back(tmp);
		}
	}

	return constellations.size();
}

int main(void) {
	int32_t result1 = 0, result2 = 0;
	Adventure4D a4d;

#if TEST
	if (!a4d.init({"0,0,0,0", "3,0,0,0", "0,3,0,0", "0,0,3,0", "0,0,0,3", "0,0,0,6", "9,0,0,0", "12,0,0,0"})) {
		return -1;
	}

	result1 = a4d.find_constellation(); // 2

	if (!a4d.init({"0,0,0,0", "3,0,0,0", "0,3,0,0", "0,0,3,0", "0,0,0,3", "0,0,0,6", "9,0,0,0", "12,0,0,0", "6,0,0,0"})) {
		return -1;
	}

	result1 = a4d.find_constellation(); // 1

	if (!a4d.init({"-1,2,2,0", "0,0,2,-2", "0,0,0,-2", "-1,2,0,0", "-2,-2,-2,2", "3,0,2,-1", "-1,3,2,2", "-1,0,-1,0", "0,2,1,-2", "3,0,0,0"})) {
		return -1;
	}

	result1 = a4d.find_constellation(); // 4

	if (!a4d.init({"1,-1,0,1", "2,0,-1,0", "3,2,-1,0", "0,0,3,1", "0,0,-1,-1", "2,3,-2,0", "-2,2,0,0", "2,-2,0,-1", "1,-1,0,-1", "3,2,0,2"})) {
		return -1;
	}

	result1 = a4d.find_constellation(); // 3

	if (!a4d.init({"1,-1,-1,-2", "-2,-2,0,1", "0,2,1,3", "-2,3,-2,1", "0,2,3,-2", "-1,-1,1,-2", "0,-2,-1,0", "-2,2,3,-1", "1,2,2,0", "-1,-2,0,-2"})) {
		return -1;
	}

	result1 = a4d.find_constellation(); // 8

	result2 = 2;
#endif

	if (!a4d.init()) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 25 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = a4d.find_constellation(); //319? too low

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
