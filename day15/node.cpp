#include "node.hpp"

void Node::init(uint32_t x, uint32_t y, char type) {
	coord_.x = x;
	coord_.y = y;
	type_ = type;
	hitpoints_ = hitpoints_init_;
	attack_power_ = attack_power_init_;
}

void Node::got_attacked(uint32_t by_attack_power) {
	if (by_attack_power >= hitpoints_) {
		hitpoints_ = 0;
		type_ = '.';
	} else {
		hitpoints_ -= by_attack_power;
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
