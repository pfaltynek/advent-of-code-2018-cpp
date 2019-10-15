#include "main.hpp"

const std::regex instr1("^#ip (\\d)$");
const std::regex instr2("^(addr|addi|mulr|muli|banr|bani|borr|bori|setr|seti|gtir|gtri|gtrr|eqir|eqri|eqrr) (\\d+) (\\d+) (\\d)$");

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
	instruction_t opcode;
	int32_t A, B, C;
} instruction_str;

bool init(const std::vector<std::string> input, std::vector<instruction_str>& data, int32_t& ip_reg_idx) {
	std::smatch sm;
	std::map<std::string, instruction_t> instruction_types = {
		{"addr", INSTRUCTION_TYPE::addr}, {"addi", INSTRUCTION_TYPE::addi}, {"mulr", INSTRUCTION_TYPE::mulr}, {"muli", INSTRUCTION_TYPE::muli},
		{"banr", INSTRUCTION_TYPE::banr}, {"bani", INSTRUCTION_TYPE::bani}, {"borr", INSTRUCTION_TYPE::borr}, {"bori", INSTRUCTION_TYPE::bori},
		{"setr", INSTRUCTION_TYPE::setr}, {"seti", INSTRUCTION_TYPE::seti}, {"gtir", INSTRUCTION_TYPE::gtir}, {"gtri", INSTRUCTION_TYPE::gtri},
		{"gtrr", INSTRUCTION_TYPE::gtrr}, {"eqir", INSTRUCTION_TYPE::eqir}, {"eqri", INSTRUCTION_TYPE::eqri}, {"eqrr", INSTRUCTION_TYPE::eqrr}};

	data.clear();

	if (!input.size()) {
		std::cout << "Empty input data" << std::endl;
		return false;
	}

	if (std::regex_match(input[0], sm, instr1)) {
		std::string x = sm.str(1);
		ip_reg_idx = stoi(sm.str(1));
	} else {
		std::cout << "Invalid input - missing #ip declaration" << std::endl;
		return false;
	}

	for (uint32_t i = 1; i < input.size(); ++i) {
		instruction_str inst;

		if (std::regex_match(input[i], sm, instr2)) {
			std::string inst_name = sm.str(1);
			if (instruction_types.count(inst_name)) {
				inst.opcode = instruction_types[inst_name];
				inst.A = stoi(sm.str(2));
				inst.B = stoi(sm.str(3));
				inst.C = stoi(sm.str(4));
			} else {
				std::cout << "Invalid instruction type at line " << i + 1 << std::endl;
				return false;
			}

			data.push_back(inst);
		} else {
			std::cout << "Invalid instruction format at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

bool init(std::vector<instruction_str>& data, int32_t& ip_reg_idx) {
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

	return init(lines, data, ip_reg_idx);
}

int32_t simulate(const std::vector<instruction_str> program, const int32_t ip_reg_idx, const int32_t reg0_init, int32_t& max) {
	int32_t regs[6] = {0, 0, 0, 0, 0, 0};
	int32_t ip = 0, first = 0, prev = 0;
	std::vector<int32_t> vals;

	max = -1;
	vals.clear();
	regs[0] = reg0_init;

	// program simulation modified according to its structure - waiting till program reaches last condition
	// and stores required register 3 value:
	while ((ip >= 0) && (ip < program.size())) {
		if (ip == 28) {
			if (std::find(vals.begin(), vals.end(), regs[3]) == vals.end()){
				vals.push_back(regs[3]);
			} else {
				max = vals[vals.size() - 1];
				break;
			}
		}
		regs[ip_reg_idx] = ip;

		switch (program[ip].opcode) {
			case INSTRUCTION_TYPE::addr:
				regs[program[ip].C] = regs[program[ip].A] + regs[program[ip].B];
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " + R" << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::addi:
				regs[program[ip].C] = regs[program[ip].A] + program[ip].B;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " + " << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::mulr:
				regs[program[ip].C] = regs[program[ip].A] * regs[program[ip].B];
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " * R" << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::muli:
				regs[program[ip].C] = regs[program[ip].A] * program[ip].B;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " * " << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::banr:
				regs[program[ip].C] = regs[program[ip].A] & regs[program[ip].B];
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " & R" << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::bani:
				regs[program[ip].C] = regs[program[ip].A] & program[ip].B;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " & " << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::borr:
				regs[program[ip].C] = regs[program[ip].A] | regs[program[ip].B];
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " | R" << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::bori:
				regs[program[ip].C] = regs[program[ip].A] | program[ip].B;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << " | " << program[ip].B << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::setr:
				regs[program[ip].C] = regs[program[ip].A];
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = R" << program[ip].A << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::seti:
				regs[program[ip].C] = program[ip].A;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = " << program[ip].A << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::gtir:
				regs[program[ip].C] = program[ip].A > regs[program[ip].B] ? 1 : 0;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = (" << program[ip].A << " > R" << program[ip].B << ")" << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::gtri:
				regs[program[ip].C] = regs[program[ip].A] > program[ip].B ? 1 : 0;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = (R" << program[ip].A << " > " << program[ip].B << ")" << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::gtrr:
				regs[program[ip].C] = regs[program[ip].A] > regs[program[ip].B] ? 1 : 0;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = (R" << program[ip].A << " > R" << program[ip].B << ")" << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::eqir:
				regs[program[ip].C] = program[ip].A == regs[program[ip].B] ? 1 : 0;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = (" << program[ip].A << " == R" << program[ip].B << ")" << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::eqri:
				regs[program[ip].C] = regs[program[ip].A] == program[ip].B ? 1 : 0;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = (R" << program[ip].A << " == " << program[ip].B << ")" << std::endl;
#endif
				break;
			case INSTRUCTION_TYPE::eqrr:
				regs[program[ip].C] = regs[program[ip].A] == regs[program[ip].B] ? 1 : 0;
#if PRINT_INSTRUCTION
				std::cout << ip << ": "
						  << "R" << program[ip].C << " = (R" << program[ip].A << " == R" << program[ip].B << ")" << std::endl;
#endif
				break;
		}

		ip = regs[ip_reg_idx];
		ip++;
	}

	if (vals.size()){
		return vals[0];
	} else {
		return -1;
	}
}

int main(void) {
	int32_t result1 = 0, result2 = 0;
	int32_t ip;
	std::vector<instruction_str> data;

	if (!init(data, ip)) {
		return -1;
	}

	std::cout << "=== Advent of Code 2018 - day 21 ====" << std::endl;
	std::cout << "--- part 1 ---" << std::endl;

	result1 = simulate(data, ip, 0 , result2);

	std::cout << "Result is " << result1 << std::endl;
	std::cout << "--- part 2 ---" << std::endl;

	std::cout << "Result is " << result2 << std::endl;
}
