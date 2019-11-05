#include "./../common/aoc.hpp"
#include <regex>

#define TEST 0

const std::regex marble_mania_template("^(\\d+) players; last marble is worth (\\d+) points$");

typedef struct marble {
	uint32_t value;
	marble *prev;
	marble *next;
} marble_t, *p_marble_t;

class AoC2018_day09 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();
	void clean_up();

  private:
	bool decode_game_conditions(const std::string line);
	void print(p_marble_t current, uint32_t player_idx);
	void free_list_mem();
	p_marble_t get_marble_mania_high_score(const int32_t last_marble_points_multiplier, uint32_t &result1, uint32_t &result2);

	int32_t players_count_, last_marble_points_;
	p_marble_t first_;
};

bool AoC2018_day09::decode_game_conditions(const std::string line) {
	std::smatch sm;

	if (std::regex_match(line, sm, marble_mania_template)) {
		players_count_ = stoi(sm.str(1));
		last_marble_points_ = stoi(sm.str(2));
		return true;
	}

	return false;
}

void AoC2018_day09::free_list_mem() {
	p_marble_t next, tmp;

	next = first_;
	while (next != NULL) {
		tmp = next;
		next = tmp->next;
		delete tmp;
		if (next == first_) {
			next = NULL;
		}
	}

	first_ = NULL;
}

void AoC2018_day09::print(p_marble_t current, uint32_t player_idx) {
	p_marble_t tmp;

	tmp = first_;
	std::cout << "[" << player_idx << "] ";

	while (tmp != NULL) {
		if (tmp == current) {
			std::cout << "(" << tmp->value << ")";
		} else {
			std::cout << tmp->value;
		}
		if (tmp->next == first_) {
			tmp = NULL;
		} else {
			std::cout << " ";
			tmp = tmp->next;
		}
	}

	std::cout << std::endl;
}

p_marble_t AoC2018_day09::get_marble_mania_high_score(const int32_t last_marble_points_multiplier, uint32_t &result1, uint32_t &result2) {
	p_marble_t first = NULL, current = NULL;
	std::vector<uint32_t> scores;
	int32_t player_idx = 0, i, j;

	result1 = 0;
	result2 = 0;
	scores.clear();
	for (i = 0; i < players_count_; ++i) {
		scores.push_back(0);
	}

	current = new marble_t;
	current->value = 0;
	current->prev = current;
	current->next = current;
	first = current;

	// print(current, player_idx);

	for (i = 1; i <= (last_marble_points_ * last_marble_points_multiplier); ++i) {
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

		player_idx++;
		player_idx %= players_count_;

		if (i == last_marble_points_) {
			for (j = 0; j < players_count_; ++j) {
				if (scores[j] > result1) {
					result1 = scores[j];
				}
			}
		}

		// print(current, player_idx);
	}

	for (i = 0; i < players_count_; ++i) {
		if (scores[i] > result2) {
			result2 = scores[i];
		}
	}

	return first;
}

bool AoC2018_day09::init(std::vector<std::string> lines) {
	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!decode_game_conditions(lines[i])) {
			std::cout << "Invalid instruction at line " << i + 1 << std::endl;
		}
	}

	return true;
}

int32_t AoC2018_day09::get_aoc_day() {
	return 9;
}

int32_t AoC2018_day09::get_aoc_year() {
	return 2018;
}

void AoC2018_day09::tests() {
#if TEST
	init({"9 players; last marble is worth 25 points"}); // high score is 32
	part1();
	part2();

	init({"10 players; last marble is worth 1618 points"}); //: high score is 8317
	part1();
	part2();

	init({"13 players; last marble is worth 7999 points"}); //: high score is 146373
	part1();
	part2();

	init({"17 players; last marble is worth 1104 points"}); //: high score is 2764
	part1();
	part2();

	init({"21 players; last marble is worth 6111 points"}); //: high score is 54718
	part1();
	part2();

	init({"30 players; last marble is worth 5807 points"}); //: high score is 37305
	part1();
	part2();

#endif
}

bool AoC2018_day09::part1() {
	uint32_t result1 = 0, result2 = 0;

	first_ = get_marble_mania_high_score(100, result1, result2);

	result1_ = std::to_string(result1);
	result2_ = std::to_string(result2);

	return true;
}

bool AoC2018_day09::part2() {

	return true;
}

void AoC2018_day09::clean_up() {
	std::cout << std::endl << "Please wait, clean-up in progress ..." <<std::endl;
	free_list_mem();
}

int main(void) {
	AoC2018_day09 day09;

	return day09.main_execution();
}
