#include <fstream>
#include <iostream>
#include <stdint.h>
#include <time.h>
#include <vector>

class AoC {
  private:
	time_t start_, end_;

	void start_time_measurement() {
		time(&start_);
	}

	int32_t end_time_measurement() {
		time(&end_);
		return static_cast<int32_t>(difftime(end_, start_));
	}

	virtual bool init() {
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
	};

  protected:
	virtual bool init(const std::vector<std::string> lines) = 0;
	virtual bool part1() = 0;
	virtual bool part2() = 0;
	virtual void tests() = 0;
	virtual int32_t get_aoc_day() = 0;
	virtual int32_t get_aoc_year() = 0;

	std::string result1_, result2_;

  public:
	int main_execution() {
		int32_t time;

		if (!init()) {
			std::cout << "Input decoding failed" << std::endl;
			return -1;
		}

		std::cout << "=== Advent of Code " << get_aoc_year() << " - day " << get_aoc_day() << " ====" << std::endl;
		std::cout << "--- part 1 ---" << std::endl;

		start_time_measurement();

		if (!part1()) {
			std::cout << "Part 1 failed" << std::endl;
			return -1;
		}

		std::cout << "Result is " << result1_ << std::endl;
		std::cout << "--- part 2 ---" << std::endl;

		if (!part2()) {
			std::cout << "Part 2 failed" << std::endl;
			return -1;
		}

		std::cout << "Result is " << result2_ << std::endl;

		time = end_time_measurement();

		if (time) {
			std::cout << "Execution time " << time << " sec." << std::endl;
		} else {
			std::cout << "Execution time less than 1 sec." << std::endl;
		}
		return 0;
	}
};