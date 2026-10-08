#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void calculate(uint8_t *bits) {
    uint8_t lsb = *bits & 1U;
    int8_t result = lsb == 1U ? -100 : 100;
    printf("[*]result = %d\n", result);
}

int main() {
    uint8_t bits = 5U;
    calculate(&bits);
    return 0;
}