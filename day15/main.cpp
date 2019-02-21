#include "main.hpp"

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	Fight fight;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1

#elif TEST2

#else
	if (!fight.init()) {
		return -1;
	}

	result1 = fight.make_fight();
#endif

	std::cout << "Result is " << result1 << std::endl;

	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
