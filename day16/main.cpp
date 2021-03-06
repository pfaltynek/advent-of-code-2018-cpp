#include "./../common/aoc.hpp"
#include <regex>

#define TEST 1

typedef enum INSTRUCTION_TYPE {
	// Addition:
	addr, // (add register) stores into register C the result of adding register A and register B.
	addi, // (add immediate) stores into register C the result of adding register A and value B.
	// Multiplication:
	mulr, // (multiply register) stores into register C the result of multiplying register A and register B.
	muli, // (multiply immediate) stores into register C the result of multiplying register A and value B.
	// Bitwise AND:
	banr, // (bitwise AND register) stores into register C the result of the bitwise AND of register A and register B.
	bani, // (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
	// Bitwise OR:
	borr, // (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
	bori, // (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
	// Assignment:
	setr, // (set register) copies the contents of register A into register C. (Input B is ignored.)
	seti, // (set immediate) stores value A into register C. (Input B is ignored.)
	// Greater-than testing:
	gtir, // (greater-than immediate/register) sets register C to 1 if value A is greater than register B. Otherwise, register C is set to 0.
	gtri, // (greater-than register/immediate) sets register C to 1 if register A is greater than value B. Otherwise, register C is set to 0.
	gtrr, // (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
	// Equality testing:
	eqir, // (equal immediate/register) sets register C to 1 if value A is equal to register B. Otherwise, register C is set to 0.
	eqri, // (equal register/immediate) sets register C to 1 if register A is equal to value B. Otherwise, register C is set to 0.
	eqrr, // (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
} instruction_t;

typedef struct INSTRUCTION {
	int32_t opcode;
	int32_t A, B, C;
} instr_str;

typedef struct REGISTERS {
	int32_t r[4];
} registers_str;

typedef struct SNIPPET {
	registers_str before;
	instr_str instruction;
	registers_str after;
} snippet_str;

const std::regex regex_bef("^Before: \\[(\\d), (\\d), (\\d), (\\d)\\]$");
const std::regex regex_ins("^(\\d+) (\\d) (\\d) (\\d)$");
const std::regex regex_aft("^After:  \\[(\\d), (\\d), (\\d), (\\d)\\]$");

const std::vector<instruction_t> instruction_types = {INSTRUCTION_TYPE::addr, INSTRUCTION_TYPE::addi, INSTRUCTION_TYPE::mulr, INSTRUCTION_TYPE::muli,
													  INSTRUCTION_TYPE::banr, INSTRUCTION_TYPE::bani, INSTRUCTION_TYPE::borr, INSTRUCTION_TYPE::bori,
													  INSTRUCTION_TYPE::setr, INSTRUCTION_TYPE::seti, INSTRUCTION_TYPE::gtir, INSTRUCTION_TYPE::gtri,
													  INSTRUCTION_TYPE::gtrr, INSTRUCTION_TYPE::eqir, INSTRUCTION_TYPE::eqri, INSTRUCTION_TYPE::eqrr};

class AoC2018_day16 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	bool parse_snippets(const std::vector<std::string> lines);
	bool parse_instruction(const std::string line, instr_str& instruction);
	bool parse_registers(const std::string line, const bool before, registers_str& registers);
	bool parse_instructions(const std::vector<std::string> lines);
	bool analyze_stats(const std::map<int32_t, std::vector<std::vector<instruction_t>>>& stats, std::map<int32_t, instruction_t>& opcode_map);
	std::vector<instruction_t> find_applicable_instructions(const snippet_str snippet);
	void do_instruction(const instruction_t operation, const instr_str instruction, registers_str& regs);
	int32_t part1(const std::vector<snippet_str> debug_lines, std::map<int32_t, std::vector<std::vector<instruction_t>>>& stats);
	int32_t part2(const std::vector<instr_str> instructions, std::map<int32_t, instruction_t>& opcode_map);

	std::vector<snippet_str> snippets_;
	std::vector<instr_str> instructions_;
	std::map<int32_t, std::vector<std::vector<instruction_t>>> stats_;
};

bool AoC2018_day16::parse_instruction(const std::string line, instr_str& instruction) {
	std::smatch sm;

	if (std::regex_match(line, sm, regex_ins)) {
		instruction.opcode = stoi(sm.str(1));
		instruction.A = stoi(sm.str(2));
		instruction.B = stoi(sm.str(3));
		instruction.C = stoi(sm.str(4));

		if ((instruction.opcode < 0) || (instruction.opcode > 15)) {
			std::cout << "Invalid opcode in '" << line << "'" << std::endl;
			return false;
		}

		if ((instruction.A < 0) || (instruction.A > 3) || (instruction.B < 0) || (instruction.B > 3) || (instruction.C < 0) || (instruction.C > 3)) {
			std::cout << "Invalid register index in '" << line << "'" << std::endl;
			return false;
		}
		return true;
	}

	return false;
}

bool AoC2018_day16::parse_registers(const std::string line, const bool before, registers_str& registers) {
	std::smatch sm;

	if (std::regex_match(line, sm, before ? regex_bef : regex_aft)) {
		registers.r[0] = stoi(sm.str(1));
		registers.r[1] = stoi(sm.str(2));
		registers.r[2] = stoi(sm.str(3));
		registers.r[3] = stoi(sm.str(4));

		return true;
	}

	return false;
}

bool AoC2018_day16::parse_snippets(const std::vector<std::string> lines) {
	if (lines.size() % 3) {
		return false;
	}

	snippets_.clear();

	for (uint32_t i = 0; i < (lines.size() / 3); ++i) {
		snippet_str snip;

		if (!parse_registers(lines[3 * i], true, snip.before)) {
			return false;
		}
		if (!parse_instruction(lines[(3 * i) + 1], snip.instruction)) {
			return false;
		}
		if (!parse_registers(lines[(3 * i) + 2], false, snip.after)) {
			return false;
		}
		snippets_.push_back(snip);
	}
	return true;
}

bool AoC2018_day16::parse_instructions(const std::vector<std::string> lines) {

	instructions_.clear();

	for (uint32_t i = 0; i < lines.size(); ++i) {
		instr_str instr;

		if (parse_instruction(lines[i], instr)) {
			instructions_.push_back(instr);
		} else {
			std::cout << "Instruction " << i + 1 << " invalid." << std::endl;
			return false;
		}
	}

	return true;
}

bool AoC2018_day16::init(const std::vector<std::string> lines) {
	int cnt = 0;
	std::vector<std::string> snippets, insts;
	bool inst_part = false;

	if (!lines.size()) {
		std::cout << "Empty input." << std::endl;
		return false;
	}

	for (uint32_t i = 0; i < lines.size(); ++i) {
		if (lines[i].empty()) {
			cnt++;
			if (cnt > 1) {
				inst_part = true;
			}
		} else {
			cnt = 0;
			if (inst_part) {
				insts.push_back(lines[i]);
			} else {
				snippets.push_back(lines[i]);
			}
		}
	}

	return parse_instructions(insts) && parse_snippets(snippets);
}

void AoC2018_day16::do_instruction(const instruction_t operation, const instr_str instruction, registers_str& regs) {
	switch (operation) {
		case INSTRUCTION_TYPE::addr:
			regs.r[instruction.C] = regs.r[instruction.A] + regs.r[instruction.B];
			break;
		case INSTRUCTION_TYPE::addi:
			regs.r[instruction.C] = regs.r[instruction.A] + instruction.B;
			break;
		case INSTRUCTION_TYPE::mulr:
			regs.r[instruction.C] = regs.r[instruction.A] * regs.r[instruction.B];
			break;
		case INSTRUCTION_TYPE::muli:
			regs.r[instruction.C] = regs.r[instruction.A] * instruction.B;
			break;
		case INSTRUCTION_TYPE::banr:
			regs.r[instruction.C] = regs.r[instruction.A] & regs.r[instruction.B];
			break;
		case INSTRUCTION_TYPE::bani:
			regs.r[instruction.C] = regs.r[instruction.A] & instruction.B;
			break;
		case INSTRUCTION_TYPE::borr:
			regs.r[instruction.C] = regs.r[instruction.A] | regs.r[instruction.B];
			break;
		case INSTRUCTION_TYPE::bori:
			regs.r[instruction.C] = regs.r[instruction.A] | instruction.B;
			break;
		case INSTRUCTION_TYPE::setr:
			regs.r[instruction.C] = regs.r[instruction.A];
			break;
		case INSTRUCTION_TYPE::seti:
			regs.r[instruction.C] = instruction.A;
			break;
		case INSTRUCTION_TYPE::gtir:
			regs.r[instruction.C] = instruction.A > regs.r[instruction.B] ? 1 : 0;
			break;
		case INSTRUCTION_TYPE::gtri:
			regs.r[instruction.C] = regs.r[instruction.A] > instruction.B ? 1 : 0;
			break;
		case INSTRUCTION_TYPE::gtrr:
			regs.r[instruction.C] = regs.r[instruction.A] > regs.r[instruction.B] ? 1 : 0;
			break;
		case INSTRUCTION_TYPE::eqir:
			regs.r[instruction.C] = instruction.A == regs.r[instruction.B] ? 1 : 0;
			break;
		case INSTRUCTION_TYPE::eqri:
			regs.r[instruction.C] = regs.r[instruction.A] == instruction.B ? 1 : 0;
			break;
		case INSTRUCTION_TYPE::eqrr:
			regs.r[instruction.C] = regs.r[instruction.A] == regs.r[instruction.B] ? 1 : 0;
			break;
	}
}

std::vector<instruction_t> AoC2018_day16::find_applicable_instructions(const snippet_str snippet) {
	std::vector<instruction_t> result;
	result.clear();

	for (uint32_t i = 0; i < instruction_types.size(); ++i) {
		registers_str regs = snippet.before;

		do_instruction(instruction_types[i], snippet.instruction, regs);

		if ((regs.r[0] == snippet.after.r[0]) && (regs.r[1] == snippet.after.r[1]) && (regs.r[2] == snippet.after.r[2]) && (regs.r[3] == snippet.after.r[3])) {
			result.push_back(instruction_types[i]);
		}
	}

	return result;
}

int32_t AoC2018_day16::part1(const std::vector<snippet_str> debug_lines, std::map<int32_t, std::vector<std::vector<instruction_t>>>& stats) {
	int32_t result = 0;
	std::vector<instruction_t> applicable;

	stats.clear();

	for (uint32_t i = 0; i < debug_lines.size(); ++i) {
		applicable = find_applicable_instructions(debug_lines[i]);

		if (applicable.size() >= 3) {
			result++;
		}

		stats[debug_lines[i].instruction.opcode].push_back(applicable);
	}

	return result;
}

bool AoC2018_day16::analyze_stats(const std::map<int32_t, std::vector<std::vector<instruction_t>>>& stats, std::map<int32_t, instruction_t>& opcode_map) {
	std::map<int32_t, std::vector<instruction_t>> stats_filtered;
	std::vector<instruction_t> inst_filtered;
	bool common, found;
	int32_t opcode;
	instruction_t inst;

	stats_filtered.clear();

	for (auto it = stats.begin(); it != stats.end(); it++) {
		if (!it->second.size()) {
			return false;
		}

		if (it->second.size() == 1) {
			stats_filtered[it->first] = it->second[0];
		} else {
			inst_filtered.clear();

			for (uint32_t i = 0; i < it->second[0].size(); ++i) {
				common = true;
				for (uint32_t j = 1; j < it->second.size(); ++j) {
					if (std::find(it->second[j].begin(), it->second[j].end(), it->second[0][i]) == it->second[j].end()) {
						common = false;
						break;
					}
				}
				if (common) {
					inst_filtered.push_back(it->second[0][i]);
				}
			}
			stats_filtered[it->first] = inst_filtered;
		}
	}

	opcode_map.clear();

	while (stats_filtered.size()) {
		found = false;
		for (auto it = stats_filtered.begin(); it != stats_filtered.end(); it++) {
			if (it->second.size() == 1) {
				opcode = it->first;
				inst = it->second[0];
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}

		opcode_map[opcode] = inst;

		stats_filtered.erase(opcode);

		for (auto it = stats_filtered.begin(); it != stats_filtered.end(); it++) {
			inst_filtered = it->second;
			auto it2 = std::find(inst_filtered.begin(), inst_filtered.end(), inst);
			if (it2 != inst_filtered.end()) {
				inst_filtered.erase(it2);
			}
			it->second = inst_filtered;
		}
	}

	return true;
}

int32_t AoC2018_day16::part2(const std::vector<instr_str> instructions, std::map<int32_t, instruction_t>& opcode_map) {
	registers_str regs = {};
	instruction_t inst;
	int32_t opcode;

	for (uint32_t i = 0; i < instructions.size(); ++i) {
		opcode = instructions[i].opcode;
		inst = opcode_map[opcode];
		do_instruction(inst, instructions[i], regs);
	}

	return regs.r[0];
}

int32_t AoC2018_day16::get_aoc_day() {
	return 16;
}

int32_t AoC2018_day16::get_aoc_year() {
	return 2018;
}

void AoC2018_day16::tests() {
#if TEST
	if (init({"Before: [3, 2, 1, 1]", "9 2 1 2", "After:  [3, 2, 2, 1]"})) {
		std::vector<instruction_t> result = find_applicable_instructions(snippets_[0]);
	}

#endif
}

bool AoC2018_day16::part1() {
	int32_t result1 = part1(snippets_, stats_);

	result1_ = std::to_string(result1);

	return true;
}

bool AoC2018_day16::part2() {
	int32_t result2;
	std::map<int32_t, instruction_t> opcode_map;

	if (analyze_stats(stats_, opcode_map)) {
		result2 = part2(instructions_, opcode_map);
		result2_ = std::to_string(result2);

		return true;
	}
	return false;
}

int main(void) {
	AoC2018_day16 day16;

	return day16.main_execution();
}
