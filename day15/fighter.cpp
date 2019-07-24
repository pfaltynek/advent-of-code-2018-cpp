#include "fighter.hpp"

Fighter::Fighter() {
	_x = 0;
	_y = 0;
	_hitpoints = _hitpoints_init;
	_attack_power = _attack_power_init;
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

bool Fighter::is_adjacent(Fighter &other, direction_t &direction) {
	if (_x == other.get_x()) {
		if ((_y + 1) == other.get_y()) {
			direction = Direction::DOWN;
			return true;
		}
		if (_y == (other.get_y() + 1)) {
			direction = Direction::UP;
			return true;
		}
	} else if (_y == other.get_y()) {
		if (((_x + 1) == other.get_x()) || (_x == (other.get_x() + 1))) {
			direction = Direction::RIGHT;
			return true;
		}
		if (((_x + 1) == other.get_x()) || (_x == (other.get_x() + 1))) {
			direction = Direction::LEFT;
			return true;
		}
	}
	return false;
}

bool Fighter::got_attacked(uint32_t by_attack_power) {
	if (by_attack_power >= _hitpoints) {
		_hitpoints = 0;
	} else {
		_hitpoints -= by_attack_power;
	}

	return is_alive();
}