#ifndef FIGHTER_HPP
#define FIGHTER_HPP

#include <stdint.h>

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

	bool equals(Fighter &other);
	bool is_adjacent(Fighter &other);

  private:
	uint32_t _x;
	uint32_t _y;
	uint32_t _hitpoints;
	uint32_t _attack_power;
	bool _is_elf;
};

#endif // FIGHTER_HPP