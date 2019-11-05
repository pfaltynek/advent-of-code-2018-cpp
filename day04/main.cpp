
#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <regex>

#define TEST 1

typedef struct shift {
	int32_t guard_id;
	int32_t year;
	int32_t month;
	int32_t day;
	std::vector<std::pair<uint32_t, uint32_t>> sleep_times;
	int32_t time_sum;
} shift_str;

const std::regex shift_template("^\\[\\d{4}\\-\\d{2}\\-\\d{2} \\d{2}:\\d{2}\\] Guard #(\\d+) begins shift$");
const std::regex asleep_template("^\\[(\\d{4})\\-(\\d{2})\\-(\\d{2}) (\\d{2}):(\\d{2})\\] falls asleep$");
const std::regex wake_template("^\\[(\\d{4})\\-(\\d{2})\\-(\\d{2}) (\\d{2}):(\\d{2})\\] wakes up$");

class AoC2018_day04 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool fill_shift(const std::vector<std::string> plans, uint32_t guard_id);
	uint32_t get_most_frequently_asleep_minute_and_guardid();
	uint32_t get_most_asleep_minute_of_guardid(const uint32_t guard_id, uint32_t &max_freq);
	uint32_t get_most_asleep_guardid();

	std::map<uint32_t, std::vector<shift_str>> shifts_;
};

bool AoC2018_day04::fill_shift(const std::vector<std::string> plans, uint32_t guard_id) {
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
	shifts_[guard_id].push_back(shift);
	return true;
}

bool AoC2018_day04::init(std::vector<std::string> lines) {
	uint32_t i;
	std::smatch sm;
	shift_str shift;
	uint32_t id;
	std::vector<std::string> shift_data;

	std::sort(lines.begin(), lines.end());

	i = 0;
	shifts_.clear();
	shift_data.clear();

	while (i < lines.size()) {
		if (std::regex_match(lines[i], sm, shift_template)) {
			if (shift_data.size()) {
				// call to process shift
				if (!fill_shift(shift_data, id)) {
					return false;
				}
				shift_data.clear();
			}

			id = stoi(sm.str(1));
		} else {
			shift_data.push_back(lines[i]);
		}

		i++;
	}

	if (shift_data.size()) {
		if (!fill_shift(shift_data, id)) {
			return false;
		}
	}
	return true;
}

uint32_t AoC2018_day04::get_most_asleep_guardid() {
	uint32_t id = 0, max = 0;

	for (auto it = shifts_.begin(); it != shifts_.end(); ++it) {
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

uint32_t AoC2018_day04::get_most_asleep_minute_of_guardid(const uint32_t guard_id, uint32_t &max_freq) {
	std::map<uint32_t, uint32_t> map;
	uint32_t max = 0, minute = 0;

	map.clear();
	for (unsigned int i = 0; i < shifts_.at(guard_id).size(); ++i) {
		for (unsigned int j = 0; j < shifts_.at(guard_id)[i].sleep_times.size(); ++j) {
			for (unsigned int t = 0; t < shifts_.at(guard_id)[i].sleep_times[j].second; ++t) {
				map[shifts_.at(guard_id)[i].sleep_times[j].first + t]++;
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

uint32_t AoC2018_day04::get_most_frequently_asleep_minute_and_guardid() {
	uint32_t max_freq = 0, id = 0, minute = 0, tmp_min, tmp_max, gid;

	for (auto it = shifts_.begin(); it != shifts_.end(); ++it) {
		gid = it->first;
		tmp_min = get_most_asleep_minute_of_guardid(gid, tmp_max);
		if (tmp_max > max_freq) {
			id = it->first;
			minute = tmp_min;
			max_freq = tmp_max;
		}
	}

	return id * minute;
}

int32_t AoC2018_day04::get_aoc_day() {
	return 4;
}

int32_t AoC2018_day04::get_aoc_year() {
	return 2018;
}

void AoC2018_day04::tests() {
#if TEST
	init({"[1518-11-01 00:05] falls asleep", "[1518-11-01 00:25] wakes up", "[1518-11-01 00:30] falls asleep", "[1518-11-01 00:55] wakes up",
		  "[1518-11-01 23:58] Guard #99 begins shift", "[1518-11-02 00:40] falls asleep", "[1518-11-02 00:50] wakes up",
		  "[1518-11-03 00:05] Guard #10 begins shift", "[1518-11-03 00:24] falls asleep", "[1518-11-03 00:29] wakes up",
		  "[1518-11-04 00:02] Guard #99 begins shift", "[1518-11-04 00:46] wakes up", "[1518-11-05 00:03] Guard #99 begins shift",
		  "[1518-11-05 00:45] falls asleep", "[1518-11-05 00:55] wakes up", "[1518-11-01 00:00] Guard #10 begins shift", "[1518-11-04 00:36] falls asleep"});

	part1();

	part2();

#endif
}

bool AoC2018_day04::part1() {
	int32_t result = 0;
	uint32_t id = 0, minute = 0, freq = 0;

	id = get_most_asleep_guardid();

	minute = get_most_asleep_minute_of_guardid(id, freq);

	result = minute * id;

	result1_ = std::to_string(result);

	return true;
}

bool AoC2018_day04::part2() {
	int32_t result = 0;

	result = get_most_frequently_asleep_minute_and_guardid();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2018_day04 day04;

	return day04.main_execution();
}
