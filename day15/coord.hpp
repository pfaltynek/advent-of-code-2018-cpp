#ifndef COORD_HPP
#define COORD_HPP

#include <stdint.h>

typedef struct COORD {
	int32_t x, y;

	inline bool const operator==(const COORD& o) {
		return x == o.x && y == o.y;
	}

	inline bool const operator<(const COORD& l) const {
		return (l.y < y || (l.y == y && l.x < x));
	}

	inline COORD operator+(const COORD& o) {
		return {x + o.x, y + o.y};
	}

	inline COORD(const int32_t x_init, const int32_t y_init) {
		x = x_init;
		y = y_init;
	}

	inline COORD() {
		x = 0;
		y = 0;
	}
} coord_str;

uint64_t coord_pack(uint32_t x, uint32_t y);
void coord_unpack(uint64_t packed, uint32_t& x, uint32_t& y);

#endif // COORD_HPP
