#include "main.hpp"

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

typedef struct REGISTERS { int32_t r[4]; } registers_str;

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

bool parse_instruction(const std::string line, instr_str& instruction) {
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

bool parse_registers(const std::string line, const bool before, registers_str& registers) {
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

bool parse_snippets(const std::vector<std::string> lines, std::vector<snippet_str>& snippets) {
	if (lines.size() % 3) {
		return false;
	}

	snippets.clear();

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
		snippets.push_back(snip);
	}
	return true;
}

bool parse_instructions(const std::vector<std::string> lines, std::vector<instr_str>& instructions) {

	instructions.clear();

	for (uint32_t i = 0; i < lines.size(); ++i) {
		instr_str instr;

		if (parse_instruction(lines[i], instr)) {
			instructions.push_back(instr);
		} else {
			std::cout << "Instruction " << i + 1 << " invalid." << std::endl;
			return false;
		}
	}

	return true;
}

bool init(const std::vector<std::string> input, std::vector<snippet_str>& debug_lines, std::vector<instr_str>& instructions) {
	int cnt = 0;
	std::vector<std::string> snippets, insts;
	bool inst_part = false;

	if (!input.size()) {
		std::cout << "Empty input." << std::endl;
	}

	for (uint32_t i = 0; i < input.size(); ++i) {
		if (input[i].empty()) {
			cnt++;
			if (cnt > 1) {
				inst_part = true;
			}
		} else {
			cnt = 0;
			if (inst_part) {
				insts.push_back(input[i]);
			} else {
				snippets.push_back(input[i]);
			}
		}
	}

	return parse_instructions(insts, instructions) && parse_snippets(snippets, debug_lines);
}

bool init(std::vector<snippet_str>& debug_lines, std::vector<instr_str>& instructions) {
	std::ifstream input;
	std::string line;
	std::vector<std::string> lines;

	input.open("input.txt", std::ifstream::in);

	if (input.fail()) {
		std::cout << "Error opening input file.\n";
		return false;
	}

	lines.clear();

	while (std::getline(input, line)) {
		lines.push_back(line);
	}

	if (input.is_open()) {
		input.close();
	}

	return init(lines, debug_lines, instructions);
}

std::vector<instruction_t> FindApplicableInstructions(const snippet_str snippet) {
	std::vector<instruction_t> result;
	result.clear();

	for (uint32_t i = 0; i < instruction_types.size(); ++i) {
		registers_str regs = snippet.before;

		switch (instruction_types[i]) {
			case INSTRUCTION_TYPE::addr:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] + regs.r[snippet.instruction.B];
				break;
			case INSTRUCTION_TYPE::addi:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] + snippet.instruction.B;
				break;
			case INSTRUCTION_TYPE::mulr:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] * regs.r[snippet.instruction.B];
				break;
			case INSTRUCTION_TYPE::muli:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] * snippet.instruction.B;
				break;
			case INSTRUCTION_TYPE::banr:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] & regs.r[snippet.instruction.B];
				break;
			case INSTRUCTION_TYPE::bani:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] & snippet.instruction.B;
				break;
			case INSTRUCTION_TYPE::borr:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] | regs.r[snippet.instruction.B];
				break;
			case INSTRUCTION_TYPE::bori:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] | snippet.instruction.B;
				break;
			case INSTRUCTION_TYPE::setr:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A];
				break;
			case INSTRUCTION_TYPE::seti:
				regs.r[snippet.instruction.C] = snippet.instruction.A;
				break;
			case INSTRUCTION_TYPE::gtir:
				regs.r[snippet.instruction.C] = snippet.instruction.A > regs.r[snippet.instruction.B] ? 1 : 0;
				break;
			case INSTRUCTION_TYPE::gtri:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] > snippet.instruction.B ? 1 : 0;
				break;
			case INSTRUCTION_TYPE::gtrr:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] > regs.r[snippet.instruction.B] ? 1 : 0;
				break;
			case INSTRUCTION_TYPE::eqir:
				regs.r[snippet.instruction.C] = snippet.instruction.A == regs.r[snippet.instruction.B] ? 1 : 0;
				break;
			case INSTRUCTION_TYPE::eqri:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] == snippet.instruction.B ? 1 : 0;
				break;
			case INSTRUCTION_TYPE::eqrr:
				regs.r[snippet.instruction.C] = regs.r[snippet.instruction.A] == regs.r[snippet.instruction.B] ? 1 : 0;
				break;
		}
		if ((regs.r[0] == snippet.after.r[0]) && (regs.r[1] == snippet.after.r[1]) && (regs.r[2] == snippet.after.r[2]) && (regs.r[3] == snippet.after.r[3])) {
			result.push_back(instruction_types[i]);
		}
	}

	return result;
}

int32_t Part1(const std::vector<snippet_str> debug_lines) {
	int32_t result = 0;
	std::vector<instruction_t> applicable, z, c;
	std::map<int32_t, std::vector<instruction_t>> x;
	std::map<int32_t, instruction_t> m;

	x.clear();

	for (uint32_t i = 0; i < debug_lines.size(); ++i) {
		applicable = FindApplicableInstructions(debug_lines[i]);

		if (applicable.size() >= 3) {
			result++;
		}

		if (!x.count(debug_lines[i].instruction.opcode)) {
			x[debug_lines[i].instruction.opcode] = applicable;
		} else {
			z = x[debug_lines[i].instruction.opcode];
			c.clear();

			for (uint32_t j = 0; j < z.size(); ++j) {
				if (std::find(applicable.begin(), applicable.end(), z[j]) != applicable.end()) {
					c.push_back(z[j]);
				}
			}
			x[debug_lines[i].instruction.opcode] = c;
		}
	}

	for (uint32_t i = 0; i < x.size(); ++i) {
		std::cout << "[" << i << "]:" << std::endl;
		for (uint32_t j = 0; j < x[i].size(); ++j) {
			std::cout << x[i][j] << " ";
		}
		std::cout << std::endl << std::endl;
	}

	return result;
}

int main(void) {
	uint64_t result1 = 0, result2 = 0;
	std::vector<snippet_str> debug_lines;
	std::vector<instr_str> instructions;

#if TEST
	if (!init({"Before: [3, 2, 1, 1]", "9 2 1 2", "After:  [3, 2, 2, 1]"}, debug_lines, instructions)) {
		return -1;
	}

	std::vector<instruction_t> result = FindApplicableInstructions(debug_lines[0]);

#endif

	if (!init(debug_lines, instructions)) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 16 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = Part1(debug_lines);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	result2 = 2;

	std::cout << "Result is " << result2 << std::endl;
}
