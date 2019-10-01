#include "main.hpp"

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	Combat combat;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	combat.test_part1_01();
	combat.test_part1_02();
	combat.test_part1_03();
	combat.test_part1_04();
	combat.test_part1_05();
	combat.test_part1_06();
	combat.test_part1_07();
	combat.test_part1_08();
	combat.test_part1_09();
#endif

#if TEST2
	combat.test_part2_01();
	combat.test_part2_02();
	combat.test_part2_03();
	combat.test_part2_04();
	combat.test_part2_05();
#endif

	if (!combat.init()) {
		return -1;
	}

	result1 = combat.make_combat_part1();

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = combat.make_combat_part2();

	std::cout << "Result is " << result2 << std::endl;
}
