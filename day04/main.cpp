
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

#define TEST1 0
#define TEST2 0

typedef struct shift {
	uint32_t guard_id;
	uint32_t year;
	uint32_t month;
	uint32_t day;
	std::vector<std::pair<uint32_t, uint32_t>> sleep_times;
	uint32_t time_sum;
} shift_str;

const std::regex shift_template("^\\[\\d{4}\\-\\d{2}\\-\\d{2} \\d{2}:\\d{2}\\] Guard #(\\d+) begins shift$");
const std::regex asleep_template("^\\[(\\d{4})\\-(\\d{2})\\-(\\d{2}) (\\d{2}):(\\d{2})\\] falls asleep$");
const std::regex wake_template("^\\[(\\d{4})\\-(\\d{2})\\-(\\d{2}) (\\d{2}):(\\d{2})\\] wakes up$");

bool FillShift(const std::vector<std::string> plans, uint32_t guard_id, std::map<uint32_t, std::vector<shift_str>> &shifts) {
	shift_str shift{};
	std::smatch sm;
	uint32_t start, end;

	if (plans.size() % 2) {
		std::cout << "Unpaired plans for Guard " << guard_id << std::endl;
	}

	shift.guard_id = guard_id;
	for (unsigned int i = 0; i < plans.size() / 2; ++i) {
		if (std::regex_match(plans[2 * i], sm, asleep_template)) {
			if (!i) {
				shift.year = stoi(sm.str(1));
				shift.month = stoi(sm.str(2));
				shift.day = stoi(sm.str(3));
			} else {
				if ((shift.year != stoi(sm.str(1))) || (shift.month != stoi(sm.str(2))) || (shift.day != stoi(sm.str(3)))) {
					std::cout << "Invalid fall asleep plan date for Guard " << guard_id << std::endl;
				}
			}
			start = (stoi(sm.str(4)) * 24) + stoi(sm.str(5));
		} else {
			std::cout << "Invalid fall asleep plan for Guard " << guard_id << std::endl;
			return false;
		}
		if (std::regex_match(plans[2 * i + 1], sm, wake_template)) {
			if ((shift.year != stoi(sm.str(1))) || (shift.month != stoi(sm.str(2))) || (shift.day != stoi(sm.str(3)))) {
				std::cout << "Invalid wake up plan date for Guard " << guard_id << std::endl;
			}
			end = (stoi(sm.str(4)) * 24) + stoi(sm.str(5));
		} else {
			std::cout << "Invalid wake up plan for Guard " << guard_id << std::endl;
			return false;
		}

		shift.sleep_times.push_back(std::pair<uint32_t, uint32_t>(start, end - start));
		shift.time_sum += end - start;
	}
	shifts[guard_id].push_back(shift);
	return true;
}

bool FillShifts(const std::vector<std::string> plans, std::map<uint32_t, std::vector<shift_str>> &shifts) {
	uint32_t i;
	std::smatch sm;
	shift_str shift;
	uint32_t id;
	std::vector<std::string> shift_data;

	i = 0;
	shifts.clear();
	shift_data.clear();

	while (i < plans.size()) {
		if (std::regex_match(plans[i], sm, shift_template)) {
			if (shift_data.size()) {
				// call to process shift
				if (!FillShift(shift_data, id, shifts)) {
					return false;
				}
				shift_data.clear();
			}

			id = stoi(sm.str(1));
		} else {
			shift_data.push_back(plans[i]);
		}

		i++;
	}

	if (shift_data.size()) {
		if (!FillShift(shift_data, id, shifts)) {
			return false;
		}
	}
	return true;
}

uint32_t GetMostAsleepGuardID(const std::map<uint32_t, std::vector<shift_str>> &shifts) {
	uint32_t id = 0, max = 0;

	for (auto it = shifts.begin(); it != shifts.end(); ++it) {
		uint32_t sum = 0;
		for (unsigned int i = 0; i < it->second.size(); ++i) {
			sum += it->second[i].time_sum;
		}
		if (sum > max) {
			id = it->first;
			max = sum;
		}
	}

	return id;
}

uint32_t GetMostAsleepMinuteOfGuardID(const std::map<uint32_t, std::vector<shift_str>> &shifts, const uint32_t guard_id, uint32_t &max_freq) {
	std::map<uint32_t, uint32_t> map;
	uint32_t max = 0, minute = 0;

	map.clear();
	for (unsigned int i = 0; i < shifts.at(guard_id).size(); ++i) {
		for (unsigned int j = 0; j < shifts.at(guard_id)[i].sleep_times.size(); ++j) {
			for (unsigned int t = 0; t < shifts.at(guard_id)[i].sleep_times[j].second; ++t) {
				map[shifts.at(guard_id)[i].sleep_times[j].first + t]++;
			}
		}
	}

	for (auto it = map.begin(); it != map.end(); ++it) {
		if (it->second > max) {
			max = it->second;
			minute = it->first;
		}
	}
	max_freq = max;

	return minute;
}

uint32_t GetMostFrequentlyAsleepMinuteAndGuardID(const std::map<uint32_t, std::vector<shift_str>> &shifts) {
	uint32_t max_freq = 0, id = 0, minute = 0, tmp_min, tmp_max;

	for (auto it = shifts.begin(); it != shifts.end(); ++it) {
		tmp_min = GetMostAsleepMinuteOfGuardID(shifts, it->first, tmp_max);
		if (tmp_max > max_freq) {
			id = it->first;
			minute = tmp_min;
			max_freq = tmp_max;
		}
	}

	return id * minute;
}

int main(void) {
	int result1 = 0, result2 = 0;
	uint32_t id = 0, minute = 0, freq = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::string> plans;
	std::map<uint32_t, std::vector<shift_str>> shifts;

	std::cout << "=== Advent of Code 2018 - day 4 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	plans.clear();

#if TEST1
	plans.push_back("[1518-11-01 00:05] falls asleep");
	plans.push_back("[1518-11-01 00:25] wakes up");
	plans.push_back("[1518-11-01 00:30] falls asleep");
	plans.push_back("[1518-11-01 00:55] wakes up");
	plans.push_back("[1518-11-01 23:58] Guard #99 begins shift");
	plans.push_back("[1518-11-02 00:40] falls asleep");
	plans.push_back("[1518-11-02 00:50] wakes up");
	plans.push_back("[1518-11-03 00:05] Guard #10 begins shift");
	plans.push_back("[1518-11-03 00:24] falls asleep");
	plans.push_back("[1518-11-03 00:29] wakes up");
	plans.push_back("[1518-11-04 00:02] Guard #99 begins shift");
	plans.push_back("[1518-11-04 00:46] wakes up");
	plans.push_back("[1518-11-05 00:03] Guard #99 begins shift");
	plans.push_back("[1518-11-05 00:45] falls asleep");
	plans.push_back("[1518-11-05 00:55] wakes up");
	plans.push_back("[1518-11-01 00:00] Guard #10 begins shift");
	plans.push_back("[1518-11-04 00:36] falls asleep");

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		plans.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	std::sort(plans.begin(), plans.end());

	if (!FillShifts(plans, shifts)) {
		return -1;
	}

	id = GetMostAsleepGuardID(shifts);

	minute = GetMostAsleepMinuteOfGuardID(shifts, id, freq);

	result1 = minute * id;
	
	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = GetMostFrequentlyAsleepMinuteAndGuardID(shifts);

	std::cout << "Result is " << result2 << std::endl;
}
