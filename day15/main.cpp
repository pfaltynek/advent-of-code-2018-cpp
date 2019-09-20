#include "main.hpp"

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	Combat combat;

	std::cout << "=== Advent of Code 2018 - day 15 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	std::vector<std::string> test = {"#######", "#E..G.#", "#...#.#", "#.G.#G#", "#######"};

	if (!combat.init(test)) {
		return -1;
	}
	uint32_t x;

	combat.one_round(x);
/*
After round 1:
#######
#.EG..#
#.G.#.#
#...#G#
#######
*/
#elif TEST2
	std::vector<std::string> test = {"#######", "#.E...#", "#.....#", "#...G.#", "#######"};

	if (!combat.init(test)) {
		return -1;
	}
	uint32_t x;

	combat.one_round(x);
/*
After round 1:
#######
#..E..#
#...G.#
#.....#
#######
*/
#elif TEST3
	std::vector<std::string> test = {"#########", "#G..G..G#", "#.......#", "#.......#", "#G..E..G#", "#.......#", "#.......#", "#G..G..G#", "#########"};

	if (!combat.init(test)) {
		return -1;
	}
	uint32_t x;

	result1 = combat.make_combat();
/*
Initially:
#########
#G..G..G#
#.......#
#.......#
#G..E..G#
#.......#
#.......#
#G..G..G#
#########

After round 1:
#########
#.G...G.#
#...G...#
#...E..G#
#.G.....#
#.......#
#G..G..G#
#.......#
#########

After round 2:
#########
#..G.G..#
#...G...#
#.G.E.G.#
#.......#
#G..G..G#
#.......#
#.......#
#########

After round 3:
#########
#.......#
#..GGG..#
#..GEG..#
#G..G...#
#......G#
#.......#
#.......#
#########
*/
#elif TEST4
	std::vector<std::string> test = {"#######", "#.G...#", "#...EG#", "#.#.#G#", "#..G#E#", "#.....#", "#######"};

	if (!combat.init(test)) {
		return -1;
	}

	result1 = combat.make_combat();
/*
*/
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
