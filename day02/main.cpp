#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#define TEST 0

void CheckID(const std::string id, bool &two, bool &three) {
	std::unordered_map<char, uint32_t> map;

	map.clear();
	for (unsigned int i = 0; i < id.size(); ++i) {
		map[id[i]]++;
	}

	two = false;
	three = false;

	for (std::unordered_map<char, uint32_t>::iterator it = map.begin(); it != map.end(); ++it) {
		if (it->second == 2) {
			two = true;
		}
		if (it->second == 3) {
			three = true;
		}
	}

}

int main(void) {
	int result1 = 0, result2 = 0;
	uint32_t two = 0, three = 0;
	std::ifstream input;
	std::string line;
	std::string captcha;
	std::vector<std::string> ids;
	unsigned int i;

	std::cout << "=== Advent of Code 2018 - day 2 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	ids.clear();

#if TEST
	ids.push_back("abcdef");
	ids.push_back("bababc");
	ids.push_back("abbcde");
	ids.push_back("abcccd");
	ids.push_back("aabcdd");
	ids.push_back("abcdee");
	ids.push_back("ababab");
#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		ids.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}
#endif
	for (i = 0; i < ids.size(); ++i) {
		bool cnt2, cnt3;
		CheckID(ids[i], cnt2, cnt3);
		if (cnt2){
			two++;
		}
		if (cnt3) {
			three++;
		}
	}
	result1 = two * three;

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
