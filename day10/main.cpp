#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>

#define TEST 1

const std::regex elv_point_template("^position=< ?(-?\\d+), +(-?\\d+)> velocity=< ?(-?\\d+), +(-?\\d+)>$");

typedef struct ELV_POINT {
	coord_str position, velocity;
} elv_point_str;

class AoC2018_day10 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool decode_elv_point(const std::string line);
	int32_t get_message_time();
	void find_limits_at_time(const uint32_t req_time, coord_str &min, coord_str &max);
	std::string generate_message(uint32_t message_time);

	std::vector<elv_point_str> points_;
};

bool AoC2018_day10::decode_elv_point(const std::string line) {
	std::smatch sm;
	elv_point_str pt;

	if (std::regex_match(line, sm, elv_point_template)) {
		pt.position.x = stoi(sm.str(1));
		pt.position.y = stoi(sm.str(2));
		pt.velocity.x = stoi(sm.str(3));
		pt.velocity.y = stoi(sm.str(4));
		points_.push_back(pt);
		return true;
	}

	return false;
}

void AoC2018_day10::find_limits_at_time(const uint32_t req_time, coord_str &min, coord_str &max) {
	int32_t val;

	min.x = INT32_MAX;
	max.x = INT32_MIN;
	min.y = INT32_MAX;
	max.y = INT32_MIN;

	for (uint32_t j = 0; j < points_.size(); ++j) {
		val = points_[j].position.x + (points_[j].velocity.x * req_time);
		if (val < min.x) {
			min.x = val;
		}
		if (val > max.x) {
			max.x = val;
		}
		val = points_[j].position.y + (points_[j].velocity.y * req_time);
		if (val < min.y) {
			min.y = val;
		}
		if (val > max.y) {
			max.y = val;
		}
	}
}

std::string AoC2018_day10::generate_message(uint32_t message_time) {
	coord_str min, max;
	uint32_t l, c, lines, columns;
	std::string result;

	find_limits_at_time(message_time, min, max);

	lines = abs(max.y - min.y) + 1;
	columns = abs(max.x - min.x) + 1;

	char display[lines][columns + 1];
	for (l = 0; l < lines; ++l) {
		for (c = 0; c < columns; ++c) {
			display[l][c] = '.';
		}
		display[l][columns] = 0;
	}

	for (uint32_t j = 0; j < points_.size(); ++j) {
		c = abs(points_[j].position.x + (points_[j].velocity.x * message_time) - min.x);
		l = abs(points_[j].position.y + (points_[j].velocity.y * message_time) - min.y);
		display[l][c] = '#';
	}

	result.clear();
	result.append("\n");
	for (l = 0; l < lines; ++l) {
		result.append(display[l]);
		result.append("\n");
	}
	return result;
}

int32_t AoC2018_day10::get_message_time() {
	uint32_t i, cnt = 0;
	uint64_t prev_size = UINT64_MAX, curr_size;

	for (i = 0; i < 20000; ++i) {
		coord_str min, max;

		find_limits_at_time(i, min, max);
		curr_size = abs(max.x - min.x) * abs(max.y - min.y);
		if (curr_size > prev_size) {
			if (++cnt >= 3) {
				return i - cnt;
			}
		} else {
			cnt = 0;
		}
		prev_size = curr_size;
	}

	return 0;
}

bool AoC2018_day10::init(std::vector<std::string> lines) {
	points_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!decode_elv_point(lines[i])) {
			std::cout << "Invalid point data at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

int32_t AoC2018_day10::get_aoc_day() {
	return 10;
}

int32_t AoC2018_day10::get_aoc_year() {
	return 2018;
}

void AoC2018_day10::tests() {
#if TEST
	init({"position=< 9,  1> velocity=< 0,  2>", "position=< 7,  0> velocity=<-1,  0>", "position=< 3, -2> velocity=<-1,  1>",
		  "position=< 6, 10> velocity=<-2, -1>", "position=< 2, -4> velocity=< 2,  2>", "position=<-6, 10> velocity=< 2, -2>",
		  "position=< 1,  8> velocity=< 1, -1>", "position=< 1,  7> velocity=< 1,  0>", "position=<-3, 11> velocity=< 1, -2>",
		  "position=< 7,  6> velocity=<-1, -1>", "position=<-2,  3> velocity=< 1,  0>", "position=<-4,  3> velocity=< 2,  0>",
		  "position=<10, -3> velocity=<-1,  1>", "position=< 5, 11> velocity=< 1, -2>", "position=< 4,  7> velocity=< 0, -1>",
		  "position=< 8, -2> velocity=< 0,  1>", "position=<15,  0> velocity=<-2,  0>", "position=< 1,  6> velocity=< 1,  0>",
		  "position=< 8,  9> velocity=< 0, -1>", "position=< 3,  3> velocity=<-1,  1>", "position=< 0,  5> velocity=< 0, -1>",
		  "position=<-2,  2> velocity=< 2,  0>", "position=< 5, -2> velocity=< 1,  2>", "position=< 1,  4> velocity=< 2,  1>",
		  "position=<-2,  7> velocity=< 2, -2>", "position=< 3,  6> velocity=<-1, -1>", "position=< 5,  0> velocity=< 1,  0>",
		  "position=<-6,  0> velocity=< 2,  0>", "position=< 5,  9> velocity=< 1, -2>", "position=<14,  7> velocity=<-2,  0>",
		  "position=<-3,  6> velocity=< 2, -1>"});

	part1();

	part2();

#endif
}

bool AoC2018_day10::part1() {
	int32_t result;

	result = get_message_time();

	if (!result) {
		std::cout << "Mesage time not found" << std::endl;
		return false;
	}
	result1_ = generate_message(result);
	result2_ = std::to_string(result);

	return true;
}

bool AoC2018_day10::part2() {

	return true;
}

int main(void) {
	AoC2018_day10 day10;

	return day10.main_execution();
}
