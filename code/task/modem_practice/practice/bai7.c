#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


static void change_cast(uint32_t *large, uint16_t *small) {
    *small = (uint16_t)*large;
    printf("small = %" PRIu16 "\n", *small);
}

int main() {
    uint32_t large = 500U;
    uint16_t small = 100U;

    change_cast(&large, &small);
    printf("large = %u\n", (unsigned)large);
    return 0;
}