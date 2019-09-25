#include "main.hpp"

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	Combat combat;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST
	combat.test01();
	combat.test02();
	combat.test03();
	combat.test04();
	combat.test05();
	combat.test06();
	combat.test07();
	combat.test08();
	combat.test09();
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
