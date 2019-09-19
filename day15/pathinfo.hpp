#ifndef PATHINFO_HPP
#define PATHINFO_HPP

#include <stdint.h>
#include <unordered_set>

class PathInfo {
  private:
	uint32_t x_;
	uint32_t y_;
	uint32_t x_1st_;
	uint32_t y_1st_;
	uint32_t steps_;
	std::unordered_set<uint32_t> path_;
	uint32_t get_coord(uint32_t x, uint32_t y);

  public:
	PathInfo();
	PathInfo(uint32_t x, uint32_t y);
	bool was_at(uint32_t x, uint32_t y);
	void move_to(uint32_t x, uint32_t y);

	uint32_t get_x();
	uint32_t get_y();
	uint32_t get_x_1st();
	uint32_t get_y_1st();
	uint32_t get_steps();
};

#endif // PATHINFO_HPP
