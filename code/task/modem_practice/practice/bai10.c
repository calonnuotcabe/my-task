#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

static void or_operate(uint8_t *bits, uint8_t *mask, uint8_t *result) {
    *result = *bits | *mask;
    printf("[*]bits = %" PRIu8 "\n", *bits);
    printf("[*]mask = %" PRIu8 "\n", *mask);
    printf("[*]result = %" PRIu8 "\n", *result);
}

int main() {
    uint8_t result;
    uint8_t bits = 8U;
    uint8_t mask = 3U;
    or_operate(&bits, &mask, &result);
    return 0;
}