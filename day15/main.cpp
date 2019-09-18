#include "main.hpp"

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	Combat combat;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	//std::vector<std::string> test = {"#######", "#E..G.#", "#...#.#", "#.G.#G#", "#######"};
	std::vector<std::string> test = {"#######", "#.E...#", "#.....#", "#...G.#", "#######"};

	if (!combat.init(test)) {
		return -1;
	}
	uint32_t x;
	
	combat.one_round(x);
#elif TEST2

#else
	if (!combat.init()) {
		return -1;
	}

	result1 = combat.make_combat();
#endif


	std::cout << "Result is " << result1 << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
