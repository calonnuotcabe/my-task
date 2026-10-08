#include <stdio.h>
#include <stdint.h>

static void change_value(uint32_t *value) {
    *value = 99U;
}

int main() {
    uint32_t x = 20U;
    uint32_t y = 10U;
    uint32_t * const p = &x;
    change_value(p);
    printf("[*]value y = %u\n", (unsigned)y);
    printf("[*]value x = %u\n", (unsigned)x);
    return 0;
}