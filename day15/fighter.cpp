#include "fighter.hpp"

Fighter::Fighter() {
	x_ = 0;
	y_ = 0;
	hitpoints_ = hitpoints_init_;
	attack_power_ = attack_power_init_;
	is_elf_ = true;
}

void Fighter::init(uint32_t x, uint32_t y, bool is_elf) {
	x_ = x;
	y_ = y;
	is_elf_ = is_elf;
}

bool Fighter::equals(Fighter &other) {
	return ((x_ == other.get_x()) && (y_ == other.get_y()));
}

bool Fighter::is_adjacent(Fighter &other, direction_t &direction) {
	if (x_ == other.get_x()) {
		if ((y_ + 1) == other.get_y()) {
			direction = Direction::DOWN;
			return true;
		}
		if (y_ == (other.get_y() + 1)) {
			direction = Direction::UP;
			return true;
		}
	} else if (y_ == other.get_y()) {
		if (((x_ + 1) == other.get_x()) || (x_ == (other.get_x() + 1))) {
			direction = Direction::RIGHT;
			return true;
		}
		if (((x_ + 1) == other.get_x()) || (x_ == (other.get_x() + 1))) {
			direction = Direction::LEFT;
			return true;
		}
	}
	return false;
}

bool Fighter::got_attacked(uint32_t by_attack_power) {
	if (by_attack_power >= hitpoints_) {
		hitpoints_ = 0;
	} else {
		hitpoints_ -= by_attack_power;
	}

	return is_alive();
}