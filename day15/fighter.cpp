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

bool Fighter::equals(Fighter &other) {
	return ((_x == other.get_x()) && (_y == other.get_y()));
}

bool Fighter::is_adjacent(Fighter &other) {
	if (_x == other.get_x()) {
		if (((_y + 1) == other.get_y()) || (_y == (other.get_y() + 1))) {
			return true;
		}
	} else if (_y == other.get_y()) {
		if (((_x + 1) == other.get_x()) || (_x == (other.get_x() + 1))) {
			return true;
		}
	}
	return false;
}