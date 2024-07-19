#include <inttypes.h>
#include "helpfuncs.h"
#include <limits.h>
uint64_t mod_pow(uint32_t a, uint64_t b, uint64_t mod) {
    int64_t result = 1;
    int64_t buffer = a;
    if (b & 1) {
        result = a;
        result %= mod;
        b >>= 1;
    }
    while (b != 0) {
        buffer *= buffer;
        buffer %= mod;
        if (b & 1 == 1) {
            result *= buffer;
        }
        b >>= 1;
        result %= mod;
    }
    return result;
}

uint64_t discreteLogarithm(uint32_t a, uint64_t mod, uint64_t res) {
    for (uint64_t i = 0; i < UINT64_MAX; i++){
        if (mod_pow(a, i, mod) == res) {
            return i;
        }
    }
}

uint8_t* convertToUint8Array(uint64_t num) {
    uint8_t arr[8];

    for (int i = 0; i < 8; i++) {
        arr[i] = (num >> (i * 8)) & 0xFF;
    }

    return arr;
}