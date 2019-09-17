#include "main.hpp"

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	Combat combat;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	std::vector<std::string> test = {"#######", "#E..G.#", "#...#.#", "#.G.#G#", "#######"};
	std::string jouza = "Jouza";

	std::cout << jouza << std::endl;

	for (int i = 0; i < test.size(); i++){
		std::cout << test[i] << std::endl;
	}

	if (!combat.init(test)) {
		return -1;
	}

	combat.make_combat();
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
