#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

#define TEST1 0
#define TEST2 0

const uint32_t CLAIM_X = 0 - 1;

std::regex claim_template("^#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)$");

typedef struct claim {
	uint32_t id;
	uint32_t left;
	uint32_t top;
	uint32_t width;
	uint32_t height;
} claim_str;

bool GetClaimData(const std::string claim_info, claim_str &claim) {
	std::smatch sm;

	if (std::regex_match(claim_info, sm, claim_template)) {
		claim.id = stoi(sm.str(1));
		claim.left = stoi(sm.str(2));
		claim.top = stoi(sm.str(3));
		claim.width = stoi(sm.str(4));
		claim.height = stoi(sm.str(5));
		return true;
	}

	return false;
}

void FillClaim(const claim_str claim, std::unordered_map<std::string, uint32_t> &map) {
	for (unsigned int i = 0; i < claim.width; ++i) {
		for (unsigned int j = 0; j < claim.height; ++j) {
			std::string key = std::to_string(claim.left + i) + "x" + std::to_string(claim.top + j);

			if (map[key]) {
				map[key] = CLAIM_X;
			} else {
				map[key] = claim.id;
			}
		}
	}
}

bool CheckClaim(const claim_str claim, std::unordered_map<std::string, uint32_t> &map) {
	for (unsigned int i = 0; i < claim.width; ++i) {
		for (unsigned int j = 0; j < claim.height; ++j) {
			std::string key = std::to_string(claim.left + i) + "x" + std::to_string(claim.top + j);

			if (map[key] != claim.id) {
				return false;
			}
		}
	}

	return true;
}

int main(void) {
	int result1 = 0, result2 = 0, cnt = 0;
	std::ifstream input;
	std::string line;
	std::vector<claim_str> claims;
	std::unordered_map<std::string, uint32_t> map;

	std::cout << "=== Advent of Code 2018 - day 3 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	claims.clear();

#if TEST1
	claim_str claim;

	if (!GetClaimData("#1 @ 1,3: 4x4", claim)) {
		std::cout << "Ivalid claim info on line 1" << std::endl;
		return -1;
	} else {
		claims.push_back(claim);
	}
	if (!GetClaimData("#2 @ 3,1: 4x4", claim)) {
		std::cout << "Ivalid claim info on line 2" << std::endl;
		return -1;
	} else {
		claims.push_back(claim);
	}
	if (!GetClaimData("#3 @ 5,5: 2x2", claim)) {
		std::cout << "Ivalid claim info on line 3" << std::endl;
		return -1;
	} else {
		claims.push_back(claim);
	}

#elif TEST2

#else
	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return -1;
	}

	while (std::getline(input, line)) {
		claim_str claim;

		cnt++;
		if (!GetClaimData(line, claim)) {
			input.close();
			std::cout << "Ivalid claim info on line " << cnt << std::endl;
			return -1;
		} else {
			claims.push_back(claim);
		}
	}

	if (input.is_open()) {
		input.close();
	}
#endif

	map.clear();

	for (unsigned int i = 0; i < claims.size(); ++i) {
		FillClaim(claims[i], map);
	}

	for (auto it = map.begin(); it != map.end(); ++it) {
		if (it->second == CLAIM_X) {
			result1++;
		}
	}

	for (unsigned int i = 0; i < claims.size(); ++i) {
		if (CheckClaim(claims[i], map)) {
			result2 = claims[i].id;
			break;
		}
	}

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;
	std::cout << "Result is " << result2 << std::endl;
}
