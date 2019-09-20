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
	After round 47:
	#######
	#G....#   G(200)
	#.G...#   G(131)
	#.#.#G#   G(59)
	#...#.#
	#....G#   G(200)
	#######
	remaining hitpoints sum = 590;
	result outcome = 27730
	*/
#elif TEST5
	std::vector<std::string> test = {"#######", "#G..#E#", "#E#E.E#", "#G.##.#", "#...#E#", "#...E.#", "#######"};
	if (!combat.init(test)) {
		return -1;
	}

	result1 = combat.make_combat();
	/*
	#######       #######
	#G..#E#       #...#E#   E(200)
	#E#E.E#       #E#...#   E(197)
	#G.##.#  -->  #.E##.#   E(185)
	#...#E#       #E..#E#   E(200), E(200)
	#...E.#       #.....#
	#######       #######

	Combat ends after 37 full rounds
	Elves win with 982 total hit points left
	Outcome: 37 * 982 = 36334
	*/
#elif TEST6
	std::vector<std::string> test = {"#######", "#E..EG#", "#.#G.E#", "#E.##E#", "#G..#.#", "#..E#.#", "#######"};

	if (!combat.init(test)) {
		return -1;
	}

	result1 = combat.make_combat();
/*
#######       #######   
#E..EG#       #.E.E.#   E(164), E(197)
#.#G.E#       #.#E..#   E(200)
#E.##E#  -->  #E.##.#   E(98)
#G..#.#       #.E.#.#   E(200)
#..E#.#       #...#.#   
#######       #######   

Combat ends after 46 full rounds
Elves win with 859 total hit points left
Outcome: 46 * 859 = 39514
*/
#elif TEST7
	std::vector<std::string> test = {"#######", "#E.G#.#", "#.#G..#", "#G.#.G#", "#G..#.#", "#...E.#", "#######"};

	if (!combat.init(test)) {
		return -1;
	}

	result1 = combat.make_combat();
/*
#######       #######   
#E.G#.#       #G.G#.#   G(200), G(98)
#.#G..#       #.#G..#   G(200)
#G.#.G#  -->  #..#..#   
#G..#.#       #...#G#   G(95)
#...E.#       #...G.#   G(200)
#######       #######   

Combat ends after 35 full rounds
Goblins win with 793 total hit points left
Outcome: 35 * 793 = 27755
*/
#elif TEST8
	std::vector<std::string> test = {"#######", "#.E...#", "#.#..G#", "#.###.#", "#E#G#G#", "#...#G#", "#######"};

	if (!combat.init(test)) {
		return -1;
	}

	result1 = combat.make_combat();
/*
#######       #######   
#.E...#       #.....#   
#.#..G#       #.#G..#   G(200)
#.###.#  -->  #.###.#   
#E#G#G#       #.#.#.#   
#...#G#       #G.G#G#   G(98), G(38), G(200)
#######       #######   

Combat ends after 54 full rounds
Goblins win with 536 total hit points left
Outcome: 54 * 536 = 28944
*/
#elif TEST9
	std::vector<std::string> test = {"#########", "#G......#", "#.E.#...#", "#..##..G#", "#...##..#", "#...#...#", "#.G...G.#", "#.....G.#", "#########"};

	if (!combat.init(test)) {
		return -1;
	}

	result1 = combat.make_combat();
/*
#########       #########   
#G......#       #.G.....#   G(137)
#.E.#...#       #G.G#...#   G(200), G(200)
#..##..G#       #.G##...#   G(200)
#...##..#  -->  #...##..#   
#...#...#       #.G.#...#   G(200)
#.G...G.#       #.......#   
#.....G.#       #.......#   
#########       #########   

Combat ends after 20 full rounds
Goblins win with 937 total hit points left
Outcome: 20 * 937 = 18740
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
