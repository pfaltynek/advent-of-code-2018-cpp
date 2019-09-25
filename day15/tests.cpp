#include "combat.hpp"

uint32_t x;

#if TEST
void Combat::test01() {
	std::vector<std::string> test = {"#######", "#E..G.#", "#...#.#", "#.G.#G#", "#######"};

	if (init(test)) {
		print_map("Initially:");
		one_round(x);
		/*
		After round 1:
		#######
		#.EG..#
		#.G.#.#
		#...#G#
		#######
		*/
		print_map("After round 1");
	}
}

void Combat::test02() {
	std::vector<std::string> test = {"#######", "#.E...#", "#.....#", "#...G.#", "#######"};

	if (init(test)) {
		print_map("Initially:");
		one_round(x);
		/*
		After round 1:
		#######
		#..E..#
		#...G.#
		#.....#
		#######
		*/
		print_map("After round 1");
	}
}

void Combat::test03() {
	std::vector<std::string> test = {"#########", "#G..G..G#", "#.......#", "#.......#", "#G..E..G#", "#.......#", "#.......#", "#G..G..G#", "#########"};

	if (init(test)) {
		print_map("Initially:");
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
		*/
		one_round(x);
		/*
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
		*/
		print_map("After round 1");
		one_round(x);
		/*
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
		*/
		print_map("After round 2");
		one_round(x);
		/*
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
		print_map("After round 3");
	}
}

void Combat::test04() {
	std::vector<std::string> test = {"#######", "#.G...#", "#...EG#", "#.#.#G#", "#..G#E#", "#.....#", "#######"};

	if (init(test)) {
		x = make_combat();
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
		std::cout << "Result is " << x << "." << std::endl;
	}
}

void Combat::test05() {
	std::vector<std::string> test = {"#######", "#G..#E#", "#E#E.E#", "#G.##.#", "#...#E#", "#...E.#", "#######"};

	if (init(test)) {
		x = make_combat();
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
		std::cout << "Result is " << x << "." << std::endl;
	}
}

void Combat::test06() {
	std::vector<std::string> test = {"#######", "#E..EG#", "#.#G.E#", "#E.##E#", "#G..#.#", "#..E#.#", "#######"};

	if (init(test)) {
		x = make_combat();
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
		std::cout << "Result is " << x << "." << std::endl;
	}
}

void Combat::test07() {
	std::vector<std::string> test = {"#######", "#E.G#.#", "#.#G..#", "#G.#.G#", "#G..#.#", "#...E.#", "#######"};

	if (init(test)) {
		x = make_combat();
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
		std::cout << "Result is " << x << "." << std::endl;
	}
}

void Combat::test08() {
	std::vector<std::string> test = {"#######", "#.E...#", "#.#..G#", "#.###.#", "#E#G#G#", "#...#G#", "#######"};

	if (init(test)) {
		x = make_combat();
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
		std::cout << "Result is " << x << "." << std::endl;
	}
}

void Combat::test09() {
	std::vector<std::string> test = {"#########", "#G......#", "#.E.#...#", "#..##..G#", "#...##..#", "#...#...#", "#.G...G.#", "#.....G.#", "#########"};

	if (init(test)) {
		x = make_combat();
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
		std::cout << "Result is " << x << "." << std::endl;
	}
}

#endif