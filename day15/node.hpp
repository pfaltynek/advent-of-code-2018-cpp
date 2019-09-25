#ifndef NODE_HPP
#define NODE_HPP

#include <stdint.h>
#include "helpers.h"
#include "coord.hpp"

class Node {
  public:
	void init(uint32_t x, uint32_t y, char type);
	uint32_t get_x() const {
		return coord_.x;
	}
	uint32_t get_y() const {
		return coord_.y;
	}
	coord_str get_coord() const {
		return coord_;
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
	void got_attacked(uint32_t by_attack_power);
	void swap(Node& other);

  private:
	coord_str coord_;
	uint32_t hitpoints_;
	uint32_t attack_power_;
	char type_;

	static const uint32_t hitpoints_init_ = 200;
	static const uint32_t attack_power_init_ = 3;
};

#endif // NODE_HPP