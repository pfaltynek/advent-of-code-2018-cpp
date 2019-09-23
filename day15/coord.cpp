#include "coord.hpp"

uint64_t coord_pack(uint32_t x, uint32_t y) {
	uint64_t result = (y & 0x00000000FFFFFFFF);
	result = result << 32;
	result |= (x & 0x00000000FFFFFFFF);
	return result;
}

void coord_unpack(uint64_t packed, uint32_t &x, uint32_t &y) {
	x = (packed & 0x00000000FFFFFFFF);
	y = packed >> 32;
}