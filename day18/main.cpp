#include "main.hpp"

/*
#include <string>
#include <unordered_map>

std::size_t h1 = std::hash<std::string>{}("MyString");
std::size_t h2 = std::hash<double>{}(3.14159);
*/

int32_t input_size = 10;

bool init(const std::vector<std::string> input, std::string& data) {
	input_size = input.size();

	data.clear();

	for (uint32_t i = 0; i < input.size(); ++i) {
		if (input[i].size() != input_size) {
			std::cout << "Invalid input size al line " << i + 1 << std::endl;
			return false;
		}
		if (input[i].find_first_not_of(".#|") != std::string::npos) {
			std::cout << "Invalid input content al line " << i + 1 << std::endl;
			return false;
		}

		data.append(input[i]);
	}

	return true;
}

bool init(std::string& data) {
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

	return init(lines, data);
}

void print(std::string data) {

	for (uint32_t i = 0; i < input_size; ++i) {
		for (uint32_t j = 0; j < input_size; ++j) {
			std::cout << data[(i * input_size) + j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	std::string data;

#if TEST
	if (!init({".#.#...|#.", ".....#|##|", ".|..|...#.", "..|#.....#", "#.#|||#|#|", "...#.||...", ".|....|...", "||...#|.#|", "|.||||..|.", "...#.|..|."},
			  data)) {
		return -1;
	}

	print(data);

	result1 = 1;

#endif

	if (!init(data)) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 18 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = 1;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
