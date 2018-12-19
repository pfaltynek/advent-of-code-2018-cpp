#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#define TEST1 0
#define TEST2 0

char track_map[200][200];

int main(void) {
	int cnt = 0;
	int32_t result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	std::vector<std::string> map;

	std::cout << "=== Advent of Code 2018 - day 13 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	map.clear();

#if TEST1
/*
/->-\        
|   |  /----\
| /-+--+-\  |
| | |  | v  |
\-+-/  \-+--/
  \------/  
*/
	map.push_back("/->-\\        ");
	map.push_back("|   |  /----\\");
	map.push_back("| /-+--+-\\  |");
	map.push_back("| | |  | v  |");
	map.push_back("\\-+-/  \\-+--/");
	map.push_back("  \\------/   ");
#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {

		map.push_back(line);
		cnt++;
	}

	if (input.is_open()) {
		input.close();
	}

#endif

	std::cout << "Result is " << result1 << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
