#include <fstream>
#include <iostream>
#include <vector>

#define TEST1 1
#define TEST2 0

int32_t GetMarbleManiaHighScore(const int32_t players_count, const int32_t last_marble_points) {
	std::vector<int32_t> marbles;
	std::vector<int32_t> scores;
	int32_t result = 0;
	uint32_t i, curr_marble_idx, player_idx;

	marbles.clear();
	scores.clear();

	for (i = 0; i < players_count; ++i) {
		scores.push_back(0);
	}

	marbles.push_back(0);
	curr_marble_idx = 0;
	player_idx = 0;

	for (i = 1; i <= last_marble_points; ++i) {
		uint32_t next;

		if (i % 23) {
			if (i == 1) {
				marbles.push_back(i);
				curr_marble_idx = 1;
			} else {
				next = (curr_marble_idx + 2) % (marbles.size() + 1);
				marbles.insert(marbles.begin() + next, i);
				curr_marble_idx = next;
			}
		} else {
			scores[player_idx] += i;

			next = curr_marble_idx;
			while (next < 7) {
				next += marbles.size();
			}
			next -= 7;

			scores[player_idx] += marbles[next];
			marbles.erase(marbles.begin() + next);

			curr_marble_idx = (++next) % marbles.size();
		}

		player_idx = (++player_idx) % players_count;
	}

	for (i = 0; i < players_count; ++i) {
		if (scores[i] > result) {
			result = scores[i];
		}
	}

	return result;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line;

	std::cout << "=== Advent of Code 2018 - day 9 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

#if TEST1
	result1 = GetMarbleManiaHighScore(9, 25);

	line = "10 players; last marble is worth 1618 points"; //: high score is 8317
	line = "13 players; last marble is worth 7999 points"; //: high score is 146373
	line = "17 players; last marble is worth 1104 points"; //: high score is 2764
	line = "21 players; last marble is worth 6111 points"; //: high score is 54718
	line = "30 players; last marble is worth 5807 points"; //: high score is 37305
#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		cnt++;
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
