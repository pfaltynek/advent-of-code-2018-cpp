#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <queue>
#include <regex>

#define TEST 1

const std::regex depth_regex("^depth: (\\d+)$");
const std::regex target_regex("^target: (\\d+),(\\d+)$");

typedef enum AREA_TYPE { rocky = 0, wet = 1, narrow = 2 } area_type_t;
typedef enum TOOL_TYPE { torch = 1, climbing_gear = 2, neither = 0 } tool_type_t;

typedef struct PATH_INFO {
	coord_str coord;
	int32_t time;
	tool_type_t tool;
} path_info_str;

class AoC2018_day22 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int32_t get_risk_level();
	int32_t find_path();
	int64_t get_geologic_index(int32_t x, int32_t y);
	int64_t get_geologic_index(coord_str coord);
	int32_t get_erosion_level(int64_t geo_idx);
	area_type_t get_area_type(int32_t erosion_lvl);
	std::vector<path_info_str> get_next_steps(path_info_str from);

	int32_t depth_;
	coord_str target_;
	std::map<coord_str, int64_t> cache_;
	std::map<coord_str, area_type_t> map_;
	std::map<coord_str, std::vector<path_info_str>> step_cache_;
};

bool AoC2018_day22::init(const std::vector<std::string> lines) {
	std::smatch sm;

	if (lines.size() < 2) {
		std::cout << "Incomplete input data" << std::endl;
		return false;
	}

	if (std::regex_match(lines[0], sm, depth_regex)) {
		depth_ = stoi(sm.str(1));
	} else {
		std::cout << "Invalid input - missing depth data" << std::endl;
		return false;
	}

	if (std::regex_match(lines[1], sm, target_regex)) {
		target_.x = stoi(sm.str(1));
		target_.y = stoi(sm.str(2));
	} else {
		std::cout << "Invalid input - missing target data" << std::endl;
		return false;
	}

	return true;
}

int64_t AoC2018_day22::get_geologic_index(int32_t x, int32_t y) {
	if ((x == target_.x) && (y == target_.y)) {
		return 0;
	}

	if (x == 0) {
		return y * 48271;
	}

	if (y == 0) {
		return x * 16807;
	}

	return cache_[coord_str(x, y - 1)] * cache_[coord_str(x - 1, y)];
}

int64_t AoC2018_day22::get_geologic_index(coord_str coord) {
	return get_geologic_index(coord.x, coord.y);
}

int32_t AoC2018_day22::get_erosion_level(int64_t geo_idx) {
	return (geo_idx + depth_) % 20183;
}

area_type_t AoC2018_day22::get_area_type(int32_t erosion_lvl) {
	return static_cast<area_type_t>(erosion_lvl % 3);
}

int32_t AoC2018_day22::get_risk_level() {
	int32_t result = 0, size, el;
	std::map<coord_str, int64_t> cache;
	coord_str pt;
	int64_t gi;
	area_type_t at;

	cache_.clear();
	cache.clear();

	cache_[coord_str(0, 0)] = get_erosion_level(0);
	result += get_area_type(get_erosion_level(get_geologic_index(0, 0)));

	size = target_.x + target_.y;
	size = size * 11 / 10;

	for (int32_t i = 1; i <= size; ++i) {
		for (int32_t j = 0; j <= i; j++) {
			pt = {j, i - j};
			gi = get_geologic_index(pt);
			el = get_erosion_level(gi);
			cache[pt] = el;
			at = get_area_type(el);
			map_[pt] = at;
			if ((pt.x <= target_.x) && (pt.y <= target_.y)) {
				result += at;
			}
		}
		cache.swap(cache_);
		cache.clear();
	}

	cache_.clear();

	return result;
}
std::vector<path_info_str> AoC2018_day22::get_next_steps(const path_info_str from) {
	std::vector<path_info_str> result = {};
	path_info_str next;
	std::vector<coord_str> adjacents = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
	std::string direction = "DRLU";

	for (uint32_t i = 0; i < adjacents.size(); i++) {
		next = from;
		next.coord = next.coord + adjacents[i];

		if (!map_.count(next.coord)) {
			continue;
		}

		next.time++;

		switch (map_[next.coord]) {
			case rocky:
				switch (next.tool) {
					case torch:
					case climbing_gear:
						result.push_back(next);
						break;
					case neither:
						break;
				}
				break;
			case wet:
				switch (next.tool) {
					case neither:
					case climbing_gear:
						result.push_back(next);
						break;
					case torch:
						break;
				}
				break;
			case narrow:
				switch (next.tool) {
					case torch:
					case neither:
						result.push_back(next);
						break;
					case climbing_gear:
						break;
				}
				break;
		}
	}

	next = from;
	next.time += 7;

	switch (map_[from.coord]) {
		case rocky:
			switch (from.tool) {
				case torch:
					next.tool = climbing_gear;
					result.push_back(next);
					break;
				case climbing_gear:
					next.tool = torch;
					result.push_back(next);
					break;
				case neither:
					break;
			}
			break;
		case wet:
			switch (from.tool) {
				case neither:
					next.tool = climbing_gear;
					result.push_back(next);
					break;
				case climbing_gear:
					next.tool = neither;
					result.push_back(next);
					break;
				case torch:
					break;
			}
			break;
		case narrow:
			switch (from.tool) {
				case torch:
					next.tool = neither;
					result.push_back(next);
					break;
				case neither:
					next.tool = torch;
					result.push_back(next);
					break;
				case climbing_gear:
					break;
			}
			break;
	}

	step_cache_[from.coord] = result;

	return result;
}

int32_t AoC2018_day22::find_path() {
	std::map<coord_str, std::map<tool_type_t, int32_t>> history = {};
	std::queue<path_info_str> q = {};
	path_info_str pi, npi;
	coord_str pt = {};
	int32_t result = INT32_MAX;
	std::vector<path_info_str> next;
	std::vector<std::string> paths = {};

	history[pt][torch] = 0;
	history[pt][neither] = 0;
	history[pt][climbing_gear] = 0;
	pi.coord = pt;
	pi.time = 0;
	pi.tool = torch;

	q.emplace(pi);

	while (!q.empty()) {
		pi = q.front();
		q.pop();

		next = get_next_steps(pi);

		for (uint32_t i = 0; i < next.size(); i++) {
			if ((next[i].coord == target_) && (next[i].tool == torch)) {
				if (result > next[i].time) {
					result = next[i].time;
				}
				continue;
			}
			if (next[i].time > result) {
				continue;
			}

			if (history.count(next[i].coord)) {
				if (history[next[i].coord].count(next[i].tool)) {
					if (next[i].time < history[next[i].coord][next[i].tool]) {
						history[next[i].coord][next[i].tool] = next[i].time;
						q.emplace(next[i]);
					}
				} else {
					history[next[i].coord][next[i].tool] = next[i].time;
					q.emplace(next[i]);
				}
			} else {
				history[next[i].coord][next[i].tool] = next[i].time;
				q.emplace(next[i]);
			}
		}
	}

	return result;
}

int32_t AoC2018_day22::get_aoc_day() {
	return 22;
}

int32_t AoC2018_day22::get_aoc_year() {
	return 2018;
}

void AoC2018_day22::tests() {
#if TEST

	init({"depth: 510", "target: 10,10"});
	part1(); // 114
	part2(); // 45

#endif
}

bool AoC2018_day22::part1() {
	int32_t result1;

	result1 = get_risk_level();

	result1_ = std::to_string(result1);

	return true;
}

bool AoC2018_day22::part2() {
	int32_t result2;

	result2 = find_path();

	result2_ = std::to_string(result2);

	return true;
}

int main(void) {
	AoC2018_day22 day22;

	return day22.main_execution();
}
