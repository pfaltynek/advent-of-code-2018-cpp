#ifndef NODE_HPP
#define NODE_HPP

#include <stdint.h>
#include "helpers.h"

class Node {
  public:
	void init(uint32_t x, uint32_t y, char type);
	uint32_t get_x() const {
		return x_;
	}
	uint32_t get_y() const {
		return y_;
	}
	char get_type(){
		return type_;
	}
	uint32_t get_hit_points() {
		return hitpoints_;
	}
	uint32_t get_attack_power() {
		return attack_power_;
	}
	bool is_alive() {
		return (hitpoints_ > 0);
	}
	bool got_attacked(uint32_t by_attack_power);

	void move_to(uint32_t x, uint32_t y);

  private:
	uint32_t x_;
	uint32_t y_;
	uint32_t hitpoints_;
	uint32_t attack_power_;
	char type_;

	static const uint32_t hitpoints_init_ = 200;
	static const uint32_t attack_power_init_ = 3;
};

#endif // NODE_HPP