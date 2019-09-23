#ifndef COORD_HPP
#define COORD_HPP

#include <stdint.h>

uint64_t coord_pack(uint32_t x, uint32_t y);
void coord_unpack(uint64_t packed, uint32_t &x, uint32_t &y);

#endif // COORD_HPP
