#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>

#define TEST 1

const std::regex nanobot_regex("^pos=<(-?\\d+),(-?\\d+),(-?\\d+)>, r=(\\d+)$");

typedef struct NANOBOT_INFO {
	int32_t x, y, z, r;

	int32_t get_range(NANOBOT_INFO other) {
		return abs(other.x - x) + abs(other.y - y) + abs(other.z - z);
	}
} nanobot_info_str;

typedef struct LIMITS {
	int32_t minx, maxx, miny, maxy, minz, maxz, minr, maxr;
} limits_str;

typedef struct CUBE_INFO {
	int32_t x, y, z, edge, nanobots;

	int32_t get_range() {
		return abs(x) + abs(y) + abs(z);
	}
} cube_info_str;

class AoC2018_day23 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	nanobot_info_str get_strongest_nanobot();
	limits_str get_data_limits(const std::vector<nanobot_info_str> data);
	int32_t get_nanobots_in_range_count();
	int32_t get_nanobots_in_range_count(const nanobot_info_str coords, const int32_t range);
	int32_t get_nanobots_in_range_count(const nanobot_info_str coords);
	int32_t get_coord_most_in_range();
	int32_t get_nanobot_to_cube_dist_1_axis(int32_t nanobot, int32_t cube_origin, int32_t cube_edge);
	int32_t get_nanobots_of_cube(cube_info_str cube);

	std::vector<nanobot_info_str> data_;
};

bool AoC2018_day23::init(const std::vector<std::string> lines) {
	std::smatch sm;
	nanobot_info_str nanobot;
	data_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, nanobot_regex)) {
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

static int get_stronger_nanobot(nanobot_info_str first, nanobot_info_str second) {
	if (first.r > second.r) {
		return 1;
	}
	return 0;
}

static int get_stronger_cube(cube_info_str first, cube_info_str second) {
	if (first.nanobots > second.nanobots) {
		return 1;
	}
	if (first.nanobots == second.nanobots) {
		if (first.get_range() < second.get_range()) {
			return 1;
		}
	}
	return 0;
}

nanobot_info_str AoC2018_day23::get_strongest_nanobot() {
	if (data_.size()) {
		std::sort(data_.begin(), data_.end(), get_stronger_nanobot);
		return data_[0];
	} else {
		return nanobot_info_str();
	}
}

limits_str AoC2018_day23::get_data_limits(const std::vector<nanobot_info_str> data) {
	limits_str result = {};
	int32_t min, max;

	if (data_.size()) {
		result.minx = result.miny = result.minz = result.minr = INT32_MAX;
		result.maxx = result.maxy = result.maxz = result.maxr = INT32_MIN;
		for (uint32_t i = 0; i < data.size(); i++) {
			min = max = data[i].x;
			min -= data[i].r;
			max += data[i].r;
			if (min < result.minx) {
				result.minx = min;
			}
			if (max > result.maxx) {
				result.maxx = max;
			}

			min = max = data[i].y;
			min -= data[i].r;
			max += data[i].r;
			if (min < result.miny) {
				result.miny = min;
			}
			if (max > result.maxy) {
				result.maxy = max;
			}

			min = max = data[i].z;
			min -= data[i].r;
			max += data[i].r;
			if (min < result.minz) {
				result.minz = min;
			}
			if (max > result.maxz) {
				result.maxz = max;
			}

			if (data[i].r < result.minr) {
				result.minr = data[i].r;
			}
			if (data[i].r > result.maxr) {
				result.maxr = data[i].r;
			}
		}
	}
	return result;
}

int32_t AoC2018_day23::get_nanobot_to_cube_dist_1_axis(int32_t nanobot, int32_t cube_origin, int32_t cube_edge) {
	// get distance to nearest edge of cube if outside of it
	// if inside it the distance is zero
	if (nanobot < cube_origin) {
		return cube_origin - nanobot;
	}
	if (nanobot > cube_origin + cube_edge - 1) {
		return nanobot - (cube_origin + cube_edge - 1);
	}
	return 0;
}

int32_t AoC2018_day23::get_nanobots_of_cube(cube_info_str cube) {
	int32_t range, result = 0;

	for (uint32_t i = 0; i < data_.size(); ++i) {
		range = get_nanobot_to_cube_dist_1_axis(data_[i].x, cube.x, cube.edge);
		range += get_nanobot_to_cube_dist_1_axis(data_[i].y, cube.y, cube.edge);
		range += get_nanobot_to_cube_dist_1_axis(data_[i].z, cube.z, cube.edge);

		if (range <= data_[i].r) {
			result++;
		}
	}

	return result;
}

int32_t AoC2018_day23::get_coord_most_in_range() {
	int32_t edge, result = 0;
	limits_str limits;
	std::vector<cube_info_str> data, tmp;
	cube_info_str cube;

	data.clear();
	limits = get_data_limits(data_);

	edge = 1;
	while ((edge < limits.maxx - limits.minx) || (edge < limits.maxy - limits.miny) || (edge < limits.maxz - limits.minz)) {
		edge *= 2;
	}

	cube.x = limits.minx;
	cube.y = limits.miny;
	cube.z = limits.minz;
	cube.edge = edge;
	cube.nanobots = get_nanobots_of_cube(cube);
	data.push_back(cube);

	while (edge > 1) {
		tmp.clear();
		edge = data[0].edge / 2;
		for (uint32_t i = 0; i < (data.size() < 10 ? data.size() : 10); i++) {
			cube = data[i];
			cube.edge = edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);

			cube = data[i];
			cube.edge = edge;
			cube.x += edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);

			cube = data[i];
			cube.edge = edge;
			cube.y += edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);

			cube = data[i];
			cube.edge = edge;
			cube.z += edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);

			cube = data[i];
			cube.edge = edge;
			cube.y += edge;
			cube.z += edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);

			cube = data[i];
			cube.edge = edge;
			cube.x += edge;
			cube.y += edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);

			cube = data[i];
			cube.edge = edge;
			cube.x += edge;
			cube.z += edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);

			cube = data[i];
			cube.edge = edge;
			cube.x += edge;
			cube.y += edge;
			cube.z += edge;
			cube.nanobots = get_nanobots_of_cube(cube);
			tmp.push_back(cube);
		}
		std::sort(tmp.begin(), tmp.end(), get_stronger_cube);
		data.swap(tmp);
	}

	result = data[0].get_range();

	return result;
}

int32_t AoC2018_day23::get_nanobots_in_range_count(const nanobot_info_str coords, const int32_t range) {
	int32_t result = 0;

	for (uint32_t i = 0; i < data_.size(); i++) {
		if (data_[i].get_range(coords) <= range) {
			result++;
		}
	}
	return result;
}

int32_t AoC2018_day23::get_nanobots_in_range_count(const nanobot_info_str coords) {
	int32_t result = 0;

	for (uint32_t i = 0; i < data_.size(); i++) {
		if (data_[i].get_range(coords) <= data_[i].r) {
			result++;
		}
	}
	return result;
}

int32_t AoC2018_day23::get_nanobots_in_range_count() {
	nanobot_info_str strongest = get_strongest_nanobot();

	return get_nanobots_in_range_count(strongest, strongest.r);
}

int32_t AoC2018_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2018_day23::get_aoc_year() {
	return 2018;
}

void AoC2018_day23::tests() {
#if TEST

	init({"pos=<0,0,0>, r=4", "pos=<1,0,0>, r=1", "pos=<4,0,0>, r=3", "pos=<0,2,0>, r=1", "pos=<0,5,0>, r=3", "pos=<0,0,3>, r=1", "pos=<1,1,1>, r=1",
		  "pos=<1,1,2>, r=1", "pos=<1,3,1>, r=1"});
	part1();

	init({"pos=<10,12,12>, r=2", "pos=<12,14,12>, r=2", "pos=<16,12,12>, r=4", "pos=<14,14,14>, r=6", "pos=<50,50,50>, r=200", "pos=<10,10,10>, r=5"});
	part2();

#endif
}

bool AoC2018_day23::part1() {
	int32_t result1;

	result1 = get_nanobots_in_range_count();

	result1_ = std::to_string(result1);

	return true;
}

bool AoC2018_day23::part2() {
	int32_t result2;

	result2 = get_coord_most_in_range();

	result2_ = std::to_string(result2);

	return true;
}

int main(void) {
	AoC2018_day23 day23;

	return day23.main_execution();
}
