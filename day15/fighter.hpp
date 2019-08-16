#ifndef FIGHTER_HPP
#define FIGHTER_HPP

#include <stdint.h>
#include "helpers.h"

class Fighter {
  public:
	Fighter();
	void init(uint32_t x, uint32_t y, bool is_elf);
	uint32_t get_x() const {
		return x_;
	}
	uint32_t get_y() const {
		return y_;
	}
	uint32_t get_hit_points() {
		return hitpoints_;
	}
	uint32_t get_attack_power() {
		return attack_power_;
	}
	bool get_is_elf() const {
		return is_elf_;
	}
	bool is_alive() {
		return (hitpoints_ > 0);
	}
	bool got_attacked(uint32_t by_attack_power);

	bool equals(Fighter &other);
	
  private:
	uint32_t x_;
	uint32_t y_;
	uint32_t hitpoints_;
	uint32_t attack_power_;
	bool is_elf_;
	
	static const uint32_t hitpoints_init_ = 200;
	static const uint32_t attack_power_init_ = 3;
};

#endif // FIGHTER_HPP