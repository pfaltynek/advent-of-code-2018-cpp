#include "fighter.hpp"

Fighter::Fighter() {
	_x = 0;
	_y = 0;
	_hitpoints = 200;
	_attack_power = 3;
	_is_elf = true;
}

void Fighter::init(uint32_t x, uint32_t y, bool is_elf) {
	_x = x;
	_y = y;
	_is_elf = is_elf;
}
