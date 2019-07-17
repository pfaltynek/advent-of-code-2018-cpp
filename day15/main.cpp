#include "main.hpp"

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	FightNS::Fight fight;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	std::vector<std::string> test = {"#######", "#E..G.#", "#...#.#", "#.G.#G#", "#######"};

	if (!fight.init(test)) {
		return -1;
	}
#elif TEST2

#else
	if (!fight.init()) {
		return -1;
	}

#endif

	result1 = fight.make_fight();

	std::cout << "Result is " << result1 << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
