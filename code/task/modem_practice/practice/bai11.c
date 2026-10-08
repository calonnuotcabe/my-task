#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void xor_op(uint8_t *bits, uint8_t *mask, uint8_t *result) {
    *result = *bits ^ *mask; 
    printf("[*]bits = %" PRIu8 "\n", *bits);
    printf("[*]mask = %" PRIu8 "\n", *mask);
    printf("[*]result = %" PRIu8 "\n", *result);

}

int main() {
    uint8_t bits = 8; 
    uint8_t mask = 3;
    uint8_t result;
    xor_op(&bits, &mask, &result);
    return 0;
}