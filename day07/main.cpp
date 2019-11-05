#include "./../common/aoc.hpp"
#include <regex>
#include <string>
#include <unordered_map>

#define TEST 1

const std::regex condition_template("^Step ([A-Z]) must be finished before step ([A-Z]) can begin.$");

class AoC2018_day07 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint32_t get_max_workers();
	int32_t get_step_base_time();
	bool decode_instruction(const std::string line);
	int32_t get_total_time(const uint32_t workers_max, const int32_t step_base_time);
	std::string get_instructions_order();
	std::string get_steps_ready_initially();

	std::string ready_;
	std::unordered_map<char, std::string> rules_;
	std::map<char, int32_t> counts_;
};

std::string AoC2018_day07::get_steps_ready_initially() {
	std::string result;
	std::unordered_map<char, std::string>::iterator itr;

	result.clear();

	for (auto it = counts_.begin(); it != counts_.end(); ++it) {
		itr = rules_.find(it->first);
		if (itr == rules_.end()) {
			result += it->first;
		}
	}

	return result;
}

std::string AoC2018_day07::get_instructions_order() {
	std::string result, done, ready;
	std::unordered_map<char, std::string>::iterator itr;
	std::unordered_map<char, std::string> rules;

	result.clear();
	done.clear();
	rules = rules_;
	ready = ready_;

	while (rules.size()) {
		char next;

		if (!ready.size()) {
			break;
		}

		std::sort(ready.begin(), ready.end());
		next = ready[0];

		for (auto it = rules.begin(); it != rules.end(); ++it) {
			std::size_t pos;

			pos = it->second.find(next);

			if (pos != std::string::npos) {
				if (it->second.size() == 1) {
					ready += it->first;
					it->second.clear();
					done += it->first;
				} else {
					it->second = it->second.substr(0, pos) + it->second.substr(pos + 1);
				}
			}
		}
		result += next;
		ready = ready.substr(1);

		for (uint32_t i = 0; i < done.size(); ++i) {
			rules.erase(done[i]);
		}

		done.clear();

		std::sort(ready.begin(), ready.end());
	}

	if (ready.size()) {
		std::sort(ready.begin(), ready.end());
		result.append(ready);
	}

	return result;
}

int32_t AoC2018_day07::get_total_time(const uint32_t workers_max, const int32_t step_base_time) {
	std::string step_order, done, workers_done;
	std::map<char, uint32_t> workers;
	int32_t total_time;
	std::string ready;

	workers.clear();
	step_order.clear();
	done.clear();
	workers_done.clear();
	total_time = 0;
	ready = ready_;

	while (rules_.size() || workers.size() || ready.size()) {
		char next;

		while ((workers.size() < workers_max) && (ready.size())) {
			workers[ready[0]] = step_base_time + ready[0] - 'A' + 1;
			ready = ready.substr(1);
		}

		for (auto it = workers.begin(); it != workers.end(); ++it) {
			if (!(--it->second)) {
				workers_done += it->first;
			}
		}
		total_time++;

		for (uint32_t i = 0; i < workers_done.size(); ++i) {
			next = workers_done[i];

			for (auto it = rules_.begin(); it != rules_.end(); ++it) {
				std::size_t pos;

				pos = it->second.find(next);

				if (pos != std::string::npos) {
					if (it->second.size() == 1) {
						ready += it->first;
						it->second.clear();
						done += it->first;
					} else {
						it->second = it->second.substr(0, pos) + it->second.substr(pos + 1);
					}
				}
			}
			step_order += workers_done[i];
			workers.erase(workers_done[i]);
		}

		workers_done.clear();

		for (uint32_t i = 0; i < done.size(); ++i) {
			rules_.erase(done[i]);
		}

		done.clear();
	}

	return total_time;
}

bool AoC2018_day07::decode_instruction(const std::string line) {
	std::smatch sm;
	char x, y;

	if (std::regex_match(line, sm, condition_template)) {
		x = sm.str(1)[0];
		y = sm.str(2)[0];
		counts_[x]++;
		counts_[y]++;
		rules_[y] += x;
		return true;
	}
	return false;
}

uint32_t AoC2018_day07::get_max_workers() {
	if (in_testing) {
		return 2;
	} else {
		return 5;
	}
}

int32_t AoC2018_day07::get_step_base_time() {
	if (in_testing) {
		return 0;
	} else {
		return 60;
	}
}

bool AoC2018_day07::init(std::vector<std::string> lines) {
	counts_.clear();
	rules_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!decode_instruction(lines[i])) {
			std::cout << "Invalid instruction at line " << i + 1 << std::endl;
		}
	}

	return true;
}

int32_t AoC2018_day07::get_aoc_day() {
	return 7;
}

int32_t AoC2018_day07::get_aoc_year() {
	return 2018;
}

void AoC2018_day07::tests() {
#if TEST
	init({"Step C must be finished before step A can begin.", "Step C must be finished before step F can begin.",
		  "Step A must be finished before step B can begin.", "Step A must be finished before step D can begin.",
		  "Step B must be finished before step E can begin.", "Step D must be finished before step E can begin.",
		  "Step F must be finished before step E can begin."});

	part1();

	part2();

#endif
}

bool AoC2018_day07::part1() {

	ready_ = get_steps_ready_initially();

	result1_ = get_instructions_order();

	return true;
}

bool AoC2018_day07::part2() {
	int32_t result = 0;

	result = get_total_time(get_max_workers(), get_step_base_time());

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2018_day07 day07;

	return day07.main_execution();
}
