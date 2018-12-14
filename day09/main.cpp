#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#define TEST1 0
#define TEST2 0

const std::regex marble_mania_template("^(\\d+) players; last marble is worth (\\d+) points$");

typedef struct marble {
	uint32_t value;
	marble *prev;
	marble *next;
} marble_t, *p_marble_t;

bool DecodeGameConditions(const std::string line, int32_t &players_count, int32_t &last_marble_points) {
	std::smatch sm;

	if (std::regex_match(line, sm, marble_mania_template)) {
		players_count = stoi(sm.str(1));
		last_marble_points = stoi(sm.str(2));
		return true;
	}

	return false;
}

void FreeListMem(const p_marble_t first) {
	p_marble_t next, tmp;

	next = first;
	while (next != NULL) {
		tmp = next;
		next = tmp->next;
		delete tmp;
		if (next == first) {
			next = NULL;
		}
	}
}

void PrintMarbles(p_marble_t first, p_marble_t current, uint32_t player_idx) {
	p_marble_t tmp;

	tmp = first;
	std::cout << "[" << player_idx << "] ";

	while (tmp != NULL) {
		if (tmp == current) {
			std::cout << "(" << tmp->value << ")";
		} else {
			std::cout << tmp->value;
		}
		if (tmp->next == first) {
			tmp = NULL;
		} else {
			std::cout << " ";
			tmp = tmp->next;
		}
	}

	std::cout << std::endl;
}

p_marble_t GetMarbleManiaHighScore(const int32_t players_count, const int32_t last_marble_points, const int32_t last_marble_points_multiplier,
								   uint32_t &result1, uint32_t &result2) {
	p_marble_t first = NULL, current = NULL;
	std::vector<uint32_t> scores;
	uint32_t player_idx = 0, i, j;

	result1 = 0;
	result2 = 0;
	scores.clear();
	for (i = 0; i < players_count; ++i) {
		scores.push_back(0);
	}

	current = new marble_t;
	current->value = 0;
	current->prev = current;
	current->next = current;
	first = current;

	// PrintMarbles(first, current, player_idx);

	for (i = 1; i <= (last_marble_points * last_marble_points_multiplier); ++i) {
		p_marble_t next, prev, tmp;

		if (i % 23) {
			prev = current->next;
			next = prev->next;
			tmp = new marble_t;
			tmp->value = i;
			tmp->next = next;
			tmp->prev = prev;
			prev->next = tmp;
			next->prev = tmp;
			current = tmp;
		} else {
			scores[player_idx] += i;
			for (j = 0; j < 7; ++j) {
				current = current->prev;
			}
			scores[player_idx] += current->value;
			prev = current->prev;
			next = current->next;
			prev->next = next;
			next->prev = prev;
			delete current;
			current = next;
		}

		player_idx = (++player_idx) % players_count;

		if (i == last_marble_points) {
			for (j = 0; j < players_count; ++j) {
				if (scores[j] > result1) {
					result1 = scores[j];
				}
			}
		}

		// PrintMarbles(first, current, player_idx);
	}

	for (i = 0; i < players_count; ++i) {
		if (scores[i] > result2) {
			result2 = scores[i];
		}
	}

	return first;
}

int main(void) {
	int players_count, last_marble_points;
	uint32_t result1 = 0, result2 = 0;
	std::ifstream input;
	std::string line;
	p_marble_t first;

	std::cout << "=== Advent of Code 2018 - day 9 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	first = GetMarbleManiaHighScore(9, 25, 1, result1, result2); // high score is 32
	FreeListMem(first);

	line = "10 players; last marble is worth 1618 points"; //: high score is 8317
	if (DecodeGameConditions(line, players_count, last_marble_points)) {
		first = GetMarbleManiaHighScore(players_count, last_marble_points, 1, result1, result2);
		FreeListMem(first);
	}

	line = "13 players; last marble is worth 7999 points"; //: high score is 146373
	if (DecodeGameConditions(line, players_count, last_marble_points)) {
		first = GetMarbleManiaHighScore(players_count, last_marble_points, 1, result1, result2);
		FreeListMem(first);
	}

	line = "17 players; last marble is worth 1104 points"; //: high score is 2764
	if (DecodeGameConditions(line, players_count, last_marble_points)) {
		first = GetMarbleManiaHighScore(players_count, last_marble_points, 1, result1, result2);
		FreeListMem(first);
	}

	line = "21 players; last marble is worth 6111 points"; //: high score is 54718
	if (DecodeGameConditions(line, players_count, last_marble_points)) {
		first = GetMarbleManiaHighScore(players_count, last_marble_points, 1, result1, result2);
		FreeListMem(first);
	}

	line = "30 players; last marble is worth 5807 points"; //: high score is 37305
	if (DecodeGameConditions(line, players_count, last_marble_points)) {
		first = GetMarbleManiaHighScore(players_count, last_marble_points, 1, result1, result2);
		FreeListMem(first);
	}

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		if (!DecodeGameConditions(line, players_count, last_marble_points)) {
			std::cout << "Invalid game data" << std::endl;
			return -1;
		} else {
			break;
		}
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	first = GetMarbleManiaHighScore(players_count, last_marble_points, 100, result1, result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
	std::cout << std::endl << "Please wait, clean-up in progress ..." <<std::endl;
	FreeListMem(first);
}
