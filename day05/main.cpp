
#include <fstream>
#include <iostream>
#include <sstream>

#define TEST1 0
#define TEST2 0

uint32_t GetReducedPolymerSize(const std::string polymer) {
	bool finished;
	std::stringstream r;
	std::string p;
	uint32_t i, j;

	if (polymer.size() <= 2) {
		return polymer.size();
	}

	p = polymer;
	r.str("");

	finished = false;

	while (!finished) {
		finished = true;
		i = 0;
		j = 1;
		while (j < p.size()) {
			if (((p[i] - p[j]) == 0x20) || ((p[j] - p[i]) == 0x20)) {
				finished = false;
				i = j + 1;
				j = i + 1;
			} else {
				r << p[i];
				i++;
				j++;
			}
		}
		if (i < p.size()) {
			r << p[i];
		}
		p = r.str();
		r.str("");
	}

	return p.size();
}

std::string RemoveUnitFromPolymer(const std::string polymer, char unit) {
	std::stringstream ss;
	char unit2;
	uint32_t i;

	ss.str("");

	if ((unit >= 'a') && (unit <= 'z')) {
		unit2 = unit - 0x20;
	} else if ((unit >= 'A') && (unit <= 'Z')) {
		unit2 = unit + 0x20;
	} else {
		return polymer;
	}

	i = 0;
	while (i < polymer.size()) {
		if ((polymer[i] != unit) && (polymer[i] != unit2)) {
			ss << polymer[i];
		}
		i++;
	}

	return ss.str();
}

uint32_t GetShortestPolymerLength(const std::string polymer) {
	std::string p;
	uint32_t result, tmp;

	result = polymer.size();

	for (char c = 'a'; c <= 'z'; ++c) {
		p = RemoveUnitFromPolymer(polymer, c);

		tmp = GetReducedPolymerSize(p);
		if (tmp < result) {
			result = tmp;
		}
	}

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0;
	uint32_t id = 0, minute = 0, freq = 0;
	std::ifstream input;
	std::string line, polymer;

	std::cout << "=== Advent of Code 2018 - day 5 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	polymer.clear();

#if TEST1
	polymer = "dabAcCaCBAcCcaDA";
#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		polymer.append(line);
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	result1 = GetReducedPolymerSize(polymer);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = GetShortestPolymerLength(polymer);

	std::cout << "Result is " << result2 << std::endl;
}
