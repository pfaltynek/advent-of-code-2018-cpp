#ifndef NODE_HPP
#define NODE_HPP

#include <stdint.h>
#include "helpers.h"
#include "coord.hpp"
#include <vector>

class Node {
  public:
	void init(int32_t x, int32_t y, char type);
	int32_t get_x() const {
		return coord_.x;
	}
	int32_t get_y() const {
		return coord_.y;
	}
	coord_str get_coord() const {
		return coord_;
	}
	char get_type(){
		return type_;
	}
	int32_t get_hit_points() {
		return hitpoints_;
	}
	int32_t get_edges_count() {
		return edges_.size();
	}
	int32_t get_attack_power() {
		return attack_power_;
	}
	bool is_alive() {
		return (hitpoints_ > 0);
	}
	coord_str get_edge(int32_t idx);
	void got_attacked(int32_t by_attack_power);
	void set_attack_power(int32_t new_value);
	void swap(Node& other);

  private:
	coord_str coord_;
	std::vector<coord_str> edges_;
	int32_t hitpoints_;
	int32_t attack_power_;
	char type_;

	static const int32_t hitpoints_init_ = 200;
	static const int32_t attack_power_init_ = 3;
};

#endif // NODE_HPP