#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void calculate(uint8_t *bits, uint8_t *result) {
    *result = (*bits >> 2U) & 3U;
    printf("[*]bits = %" PRIu8 "\n", *bits);
    printf("[*]result = %" PRIu8 "\n", *result);
}

int main() {
    uint8_t bits = 11U;
    uint8_t result;
    calculate(&bits, &result);
    return 0;
}