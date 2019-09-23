#include "node.hpp"

void Node::init(uint32_t x, uint32_t y, char type) {
	x_ = x;
	y_ = y;
	type_ = type;
	hitpoints_ = hitpoints_init_;
	attack_power_ = attack_power_init_;
}


bool Node::got_attacked(uint32_t by_attack_power) {
	if (by_attack_power >= hitpoints_) {
		hitpoints_ = 0;
	} else {
		hitpoints_ -= by_attack_power;
	}

	return is_alive();
}
void Node::move_to(uint32_t x, uint32_t y){
	x_ = x;
	y_ = y;
}
