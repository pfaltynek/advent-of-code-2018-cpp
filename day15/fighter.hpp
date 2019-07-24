#ifndef FIGHTER_HPP
#define FIGHTER_HPP

#include <stdint.h>
#include "helpers.h"

class Fighter {
  public:
	Fighter();
	void init(uint32_t x, uint32_t y, bool is_elf);
	uint32_t get_x() const {
		return _x;
	}
	uint32_t get_y() const {
		return _y;
	}
	uint32_t get_hit_points() {
		return _hitpoints;
	}
	bool get_is_elf() const {
		return _is_elf;
	}
	bool is_alive() {
		return (_hitpoints > 0);
	}
	bool got_attacked(uint32_t by_attack_power);

	bool equals(Fighter &other);
	bool is_adjacent(Fighter &other, direction_t &direction);

  private:
	uint32_t _x;
	uint32_t _y;
	uint32_t _hitpoints;
	uint32_t _attack_power;
	bool _is_elf;
	
	static const uint32_t _hitpoints_init = 200;
	static const uint32_t _attack_power_init = 3;
};

#endif // FIGHTER_HPP