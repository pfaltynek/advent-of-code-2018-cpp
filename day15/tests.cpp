#include "combat.hpp"

int32_t x;

#if TEST1
void Combat::test_part1_01() {
	std::vector<std::string> test = {"#######", "#E..G.#", "#...#.#", "#.G.#G#", "#######"};

	if (init(test)) {
		print_map("Initially:");
		one_round_part1(x);
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

void Combat::test_part1_02() {
	std::vector<std::string> test = {"#######", "#.E...#", "#.....#", "#...G.#", "#######"};

	if (init(test)) {
		print_map("Initially:");
		one_round_part1(x);
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

void Combat::test_part1_03() {
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
		one_round_part1(x);
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
		one_round_part1(x);
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
		one_round_part1(x);
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

void Combat::test_part1_04() {
	std::vector<std::string> test = {"#######", "#.G...#", "#...EG#", "#.#.#G#", "#..G#E#", "#.....#", "#######"};

	if (init(test)) {
		x = make_combat_part1();
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

void Combat::test_part1_05() {
	std::vector<std::string> test = {"#######", "#G..#E#", "#E#E.E#", "#G.##.#", "#...#E#", "#...E.#", "#######"};

	if (init(test)) {
		x = make_combat_part1();
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

void Combat::test_part1_06() {
	std::vector<std::string> test = {"#######", "#E..EG#", "#.#G.E#", "#E.##E#", "#G..#.#", "#..E#.#", "#######"};

	if (init(test)) {
		x = make_combat_part1();
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

void Combat::test_part1_07() {
	std::vector<std::string> test = {"#######", "#E.G#.#", "#.#G..#", "#G.#.G#", "#G..#.#", "#...E.#", "#######"};

	if (init(test)) {
		x = make_combat_part1();
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

void Combat::test_part1_08() {
	std::vector<std::string> test = {"#######", "#.E...#", "#.#..G#", "#.###.#", "#E#G#G#", "#...#G#", "#######"};

	if (init(test)) {
		x = make_combat_part1();
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

void Combat::test_part1_09() {
	std::vector<std::string> test = {"#########", "#G......#", "#.E.#...#", "#..##..G#", "#...##..#", "#...#...#", "#.G...G.#", "#.....G.#", "#########"};

	if (init(test)) {
		x = make_combat_part1();
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

#if TEST2
void Combat::test_part2_01() {
	std::vector<std::string> test = {"#######", "#.G...#", "#...EG#", "#.#.#G#", "#..G#E#", "#.....#", "#######"};

	if (init(test)) {
		x = make_combat_part2();
		/*
		#######
		#..E..#   E(158)
		#...E.#   E(14)
		#.#.#.#
		#...#.#
		#.....#
		#######

		Combat ends after 29 full rounds
		Elves win with 172 total hit points left
		Outcome: 29 * 172 = 4988
		*/
	}
}

void Combat::test_part2_02() {
	std::vector<std::string> test = {"#######", "#E..EG#", "#.#G.E#", "#E.##E#", "#G..#.#", "#..E#.#", "#######"};

	if (init(test)) {
		x = make_combat_part2();
		/*
		#######
		#.E.E.#   E(200), E(23)
		#.#E..#   E(200)
		#E.##E#   E(125), E(200)
		#.E.#.#   E(200)
		#...#.#
		#######

		Combat ends after 33 full rounds
		Elves win with 948 total hit points left
		Outcome: 33 * 948 = 31284*/
	}
}

void Combat::test_part2_03() {
	std::vector<std::string> test = {"#######", "#E.G#.#", "#.#G..#", "#G.#.G#", "#G..#.#", "#...E.#", "#######"};

	if (init(test)) {
		x = make_combat_part2();
		/*
		#######
		#.E.#.#   E(8)
		#.#E..#   E(86)
		#..#..#
		#...#.#
		#.....#
		#######

		Combat ends after 37 full rounds
		Elves win with 94 total hit points left
		Outcome: 37 * 94 = 3478*/
	}
}

void Combat::test_part2_04() {
	std::vector<std::string> test = {"#######", "#.E...#", "#.#..G#", "#.###.#", "#E#G#G#", "#...#G#", "#######"};

	if (init(test)) {
		x = make_combat_part2();
		/*
		#######
		#...E.#   E(14)
		#.#..E#   E(152)
		#.###.#
		#.#.#.#
		#...#.#
		#######

		Combat ends after 39 full rounds
		Elves win with 166 total hit points left
		Outcome: 39 * 166 = 6474*/
	}
}

void Combat::test_part2_05() {
	std::vector<std::string> test = {"#########", "#G......#", "#.E.#...#", "#..##..G#", "#...##..#", "#...#...#", "#.G...G.#", "#.....G.#", "#########"};

	if (init(test)) {
		x = make_combat_part2();
		/*
		#########
		#.......#
		#.E.#...#   E(38)
		#..##...#
		#...##..#
		#...#...#
		#.......#
		#.......#
		#########

		Combat ends after 30 full rounds
		Elves win with 38 total hit points left
		Outcome: 30 * 38 = 1140*/
	}
}

#endif