#include "main.hpp"

const std::regex nanobot_regex("^pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(\\d+)$");

typedef enum AREA_TYPE { rocky = 0, wet = 1, narrow = 2 } area_type_t;
typedef enum TOOL_TYPE { torch = 1, climbing_gear = 2, neither = 0 } tool_type_t;

typedef struct NANOBOT_INFO {
	int32_t x, y, z, r;

	int32_t get_range(NANOBOT_INFO other) {
		return abs(other.x - x) + abs(other.y - y) + abs(other.z - z);
	}
} nanobot_info_str;

class ExperimentalEmergencyTransportation {
  public:
	bool init(const std::vector<std::string> input);
	bool init();
	nanobot_info_str get_strongest_nanobot();
	int32_t get_nanobots_in_range_count();
	int32_t get_nanobots_in_range_count(const nanobot_info_str coords, const int32_t range);
	int32_t get_nanobots_in_range_count(const nanobot_info_str coords);
	int32_t get_coord_most_in_range();

  private:
	std::vector<nanobot_info_str> data_;
};

bool ExperimentalEmergencyTransportation::init(const std::vector<std::string> input) {
	std::smatch sm;
	nanobot_info_str nanobot;
	data_.clear();

	for (uint32_t i = 0; i < input.size(); i++) {
		if (std::regex_match(input[i], sm, nanobot_regex)) {
			nanobot.x = stoi(sm.str(1));
			nanobot.y = stoi(sm.str(2));
			nanobot.z = stoi(sm.str(3));
			nanobot.r = stoi(sm.str(4));
		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		data_.push_back(nanobot);
	}

	return true;
}

bool ExperimentalEmergencyTransportation::init() {
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

static int get_stronger_nanobot(nanobot_info_str first, nanobot_info_str second) {
	if (first.r > second.r) {
		return 1;
	}
	return 0;
}

nanobot_info_str ExperimentalEmergencyTransportation::get_strongest_nanobot() {
	if (data_.size()) {
		std::sort(data_.begin(), data_.end(), get_stronger_nanobot);
		return data_[0];
	} else {
		return nanobot_info_str();
	}
}

int32_t ExperimentalEmergencyTransportation::get_coord_most_in_range() {
	int32_t minx, maxx, miny, maxy, minz, maxz, min, max, range, tmp;
	nanobot_info_str result, coord;

	minx = miny = minz = INT32_MIN;
	maxx = maxy = maxz = INT32_MAX;
	for (uint32_t i = 0; i < data_.size(); i++) {
		min = max = data_[i].x;
		min -= data_[i].r;
		max += data_[i].r;
		if (min > minx) {
			minx = min;
		}
		if (max < maxx) {
			maxx = max;
		}

		min = max = data_[i].y;
		min -= data_[i].r;
		max += data_[i].r;
		if (min > miny) {
			miny = min;
		}
		if (max < maxy) {
			maxy = max;
		}

		min = max = data_[i].z;
		min -= data_[i].r;
		max += data_[i].r;
		if (min > minz) {
			minz = min;
		}
		if (max < maxz) {
			maxz = max;
		}
	}

	range = 0;

	for (int32_t x = minx; x <= maxx; x++) {
		for (int32_t y = miny; y <= maxy; y++) {
			for (int32_t z = minz; z <= maxz; z++) {
				coord.x = x;
				coord.y = y;
				coord.z = z;
				coord.r = 0;

				tmp = get_nanobots_in_range_count(coord);

				if (tmp > range){
					range = tmp;
					result = coord;
				}
			}
		}
	}

	return result.get_range(nanobot_info_str());
}

int32_t ExperimentalEmergencyTransportation::get_nanobots_in_range_count(const nanobot_info_str coords, const int32_t range) {
	int32_t result = 0;

	for (uint32_t i = 0; i < data_.size(); i++) {
		if (data_[i].get_range(coords) <= range) {
			result++;
		}
	}
	return result;
}

int32_t ExperimentalEmergencyTransportation::get_nanobots_in_range_count(const nanobot_info_str coords) {
	int32_t result = 0;

	for (uint32_t i = 0; i < data_.size(); i++) {
		if (data_[i].get_range(coords) <= data_[i].r) {
			result++;
		}
	}
	return result;
}

int32_t ExperimentalEmergencyTransportation::get_nanobots_in_range_count() {
	int32_t result = 0;

	nanobot_info_str strongest = get_strongest_nanobot();

	return get_nanobots_in_range_count(strongest, strongest.r);
}

int main(void) {
	int32_t result1 = 0, result2 = 0;
	ExperimentalEmergencyTransportation eet;

#if TEST
	if (!eet.init({"pos=<0,0,0>, r=4", "pos=<1,0,0>, r=1", "pos=<4,0,0>, r=3", "pos=<0,2,0>, r=1", "pos=<0,5,0>, r=3", "pos=<0,0,3>, r=1", "pos=<1,1,1>, r=1",
				   "pos=<1,1,2>, r=1", "pos=<1,3,1>, r=1"})) {
		return -1;
	}

	result1 = eet.get_nanobots_in_range_count();

	if (!eet.init(
			{"pos=<10,12,12>, r=2", "pos=<12,14,12>, r=2", "pos=<16,12,12>, r=4", "pos=<14,14,14>, r=6", "pos=<50,50,50>, r=200", "pos=<10,10,10>, r=5"})) {
		return -1;
	}

	result2 = eet.get_coord_most_in_range();

#endif

	if (!eet.init()) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 23 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = eet.get_nanobots_in_range_count();

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = eet.get_coord_most_in_range();

	std::cout << "Result is " << result2 << std::endl;
}
