#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void and_operate(uint8_t *bits, uint8_t *mask, uint8_t *result) {
    *result = *bits & *mask;
    printf("[*]result = %" PRIu8 "\n", *result); 
    printf("[*]bits = %" PRIu8 "\n", *bits);
    printf("[*]mask = %" PRIu8 "\n", *mask);
}

int main() {
    uint8_t bits = 11U;
    uint8_t mask = 3U;
    uint8_t result;
    and_operate(&bits, &mask, &result);
    return 0;
}