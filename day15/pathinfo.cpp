#include "pathinfo.hpp"

uint32_t PathInfo::get_x() {
	return x_;
}

uint32_t PathInfo::get_y() {
	return y_;
}

uint32_t PathInfo::get_x_1st() {
	return x_1st_;
}

uint32_t PathInfo::get_y_1st() {
	return y_1st_;
}

uint32_t PathInfo::get_steps() {
	return steps_;
}

PathInfo::PathInfo() {
}

PathInfo::PathInfo(uint32_t x, uint32_t y) {
	path_.emplace(get_coord(x, y));
	x_ = x;
	y_ = y;
	steps_ = 0;
}

uint32_t PathInfo::get_coord(uint32_t x, uint32_t y) {
	uint32_t result = x;

	result &= 0x0000FFFF;
	result = result << 16;
	result |= y & 0x0000FFFF;
	return result;
}

void PathInfo::move_to(uint32_t x, uint32_t y) {
	x_ = x;
	y_ = y;
	if (!steps_) {
		x_1st_ = x;
		y_1st_ = y;
	}
	steps_++;
	path_.emplace(get_coord(x, y));
}

bool PathInfo::was_at(uint32_t x, uint32_t y) {
	if (path_.find(get_coord(x, y)) == path_.end()) {
		return false;
	} else {
		return true;
	}
}