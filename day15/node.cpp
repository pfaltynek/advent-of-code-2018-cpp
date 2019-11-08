#include "node.hpp"

void Node::init(int32_t x, int32_t y, char type) {
	coord_.x = x;
	coord_.y = y;
	type_ = type;
	hitpoints_ = hitpoints_init_;
	attack_power_ = attack_power_init_;
	edges_.clear();
	// {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
	edges_.push_back(coord_str(x, y - 1));
	edges_.push_back(coord_str(x - 1, y));
	edges_.push_back(coord_str(x + 1, y));
	edges_.push_back(coord_str(x, y + 1));
}

void Node::got_attacked(int32_t by_attack_power) {
	hitpoints_ -= by_attack_power;

	if (hitpoints_ < 0) {
		hitpoints_ = 0;
	}

	if (hitpoints_ == 0) {
		type_ = '.';
	}
}

void Node::swap(Node& other) {
	Node tmp(other);

	other.hitpoints_ = this->hitpoints_;
	other.attack_power_ = this->attack_power_;
	other.type_ = this->type_;

	this->hitpoints_ = tmp.hitpoints_;
	this->attack_power_ = tmp.attack_power_;
	this->type_ = tmp.type_;
}

coord_str Node::get_edge(uint32_t idx) {
	if ((idx >=0) && (idx < edges_.size())){
		return edges_[idx];
	} else {
		return coord_;
	}
}

void Node::set_attack_power(int32_t new_value) {
	attack_power_ = new_value;
}
