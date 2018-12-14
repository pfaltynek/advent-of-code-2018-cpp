#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#define TEST1 0
#define TEST2 0

const std::regex elv_point_template("^position=< ?(-?\\d+), +(-?\\d+)> velocity=< ?(-?\\d+), +(-?\\d+)>$");

typedef std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>> elv_point;

bool DecodeElvPoint(const std::string line, std::vector<elv_point> &points) {
	std::smatch sm;
	int32_t px, py, vx, vy;

	if (std::regex_match(line, sm, elv_point_template)) {
		px = stoi(sm.str(1));
		py = stoi(sm.str(2));
		vx = stoi(sm.str(3));
		vy = stoi(sm.str(4));
		points.push_back(elv_point(std::pair<int32_t, int32_t>(px, py), std::pair<int32_t, int32_t>(vx, vy)));
		return true;
	}

	return false;
}

void FindLimitsAtTime(const std::vector<elv_point> &points, const uint32_t req_time, int32_t &xmin, int32_t &xmax, int32_t &ymin, int32_t &ymax) {
	int32_t val;

	xmin = INT32_MAX;
	xmax = INT32_MIN;
	ymin = INT32_MAX;
	ymax = INT32_MIN;

	for (uint32_t j = 0; j < points.size(); ++j) {
		val = points[j].first.first + (points[j].second.first * req_time);
		if (val < xmin) {
			xmin = val;
		}
		if (val > xmax) {
			xmax = val;
		}
		val = points[j].first.second + (points[j].second.second * req_time);
		if (val < ymin) {
			ymin = val;
		}
		if (val > ymax) {
			ymax = val;
		}
	}
}

void GenerateMessage(const std::vector<elv_point> &points, uint32_t message_time) {
	int32_t xmin, xmax, ymin, ymax;
	int lines, columns;
	uint32_t l, c, j;

	FindLimitsAtTime(points, message_time, xmin, xmax, ymin, ymax);

	lines = abs(ymax - ymin) + 1;
	columns = abs(xmax - xmin) + 1;

	char display[lines][columns + 1];
	for (l = 0; l < lines; ++l) {
		for (c = 0; c < columns; ++c) {
			display[l][c] = '.';
		}
		display[l][columns] = 0;
	}

	for (uint32_t j = 0; j < points.size(); ++j) {
		c = abs(points[j].first.first + (points[j].second.first * message_time) - xmin);
		l = abs(points[j].first.second + (points[j].second.second * message_time) - ymin);
		display[l][c] = '#';
	}
	for (l = 0; l < lines; ++l) {
		std::cout << display[l] << std::endl;
	}
}

uint32_t GetMessageTime(const std::vector<elv_point> &points) {
	uint32_t i, cnt = 0;
	uint64_t prev_size = UINT64_MAX, curr_size;

	for (i = 0; i < 20000; ++i) {
		int32_t xmin, xmax, ymin, ymax;

		FindLimitsAtTime(points, i, xmin, xmax, ymin, ymax);
		curr_size = abs(xmax - xmin) * abs(ymax - ymin);
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

int main(void) {
	int cnt = 0;
	uint32_t result = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::pair<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>>> points;

	points.clear();

	std::cout << "=== Advent of Code 2018 - day 10 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1

	if (!DecodeElvPoint("position=< 9,  1> velocity=< 0,  2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 7,  0> velocity=<-1,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 3, -2> velocity=<-1,  1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 6, 10> velocity=<-2, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 2, -4> velocity=< 2,  2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-6, 10> velocity=< 2, -2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 1,  8> velocity=< 1, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 1,  7> velocity=< 1,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-3, 11> velocity=< 1, -2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 7,  6> velocity=<-1, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-2,  3> velocity=< 1,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-4,  3> velocity=< 2,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<10, -3> velocity=<-1,  1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 5, 11> velocity=< 1, -2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 4,  7> velocity=< 0, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 8, -2> velocity=< 0,  1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<15,  0> velocity=<-2,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 1,  6> velocity=< 1,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 8,  9> velocity=< 0, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 3,  3> velocity=<-1,  1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 0,  5> velocity=< 0, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-2,  2> velocity=< 2,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 5, -2> velocity=< 1,  2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 1,  4> velocity=< 2,  1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-2,  7> velocity=< 2, -2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 3,  6> velocity=<-1, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 5,  0> velocity=< 1,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-6,  0> velocity=< 2,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=< 5,  9> velocity=< 1, -2>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<14,  7> velocity=<-2,  0>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}
	if (!DecodeElvPoint("position=<-3,  6> velocity=< 2, -1>", points)) {
		std::cout << "Invalid point data" << std::endl;
		return -1;
	}

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
		if (!DecodeElvPoint(line, points)) {
			std::cout << "Invalid point data at line " << cnt << std::endl;
			return -1;
		}
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	result = GetMessageTime(points);
	if (!result) {
		std::cout << "Mesage time not found" << std::endl;
		return -1;
	}
	std::cout << "Result is " << std::endl;
	
	GenerateMessage(points, result);

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result << std::endl;
}
