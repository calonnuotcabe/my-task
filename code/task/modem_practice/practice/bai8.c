#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void print_result(uint8_t *bit, uint8_t *result) {
    *result = *bit >> 2;
    printf("[*]result = %u\n", (unsigned)*result);
    printf("[*]result = %" PRIu8 "\n", *result);
}
int main() {
    uint8_t bits = 12U;
    uint8_t result = 0;
    print_result(&bits, &result);
    return 0;
}