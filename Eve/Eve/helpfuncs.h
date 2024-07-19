#pragma once
#include <inttypes.h>
uint64_t mod_pow(uint32_t a, uint64_t b, uint64_t mod);

uint64_t discreteLogarithm(uint32_t a, uint64_t mod, uint64_t res);

uint8_t* convertToUint8Array(uint64_t num);