#include <stdio.h>
#include <stdint.h>

static void print_value(const uint32_t *x) {
    printf("%u\n", (unsigned)*x);
}

int main() {
    const uint32_t x = 25U;
    print_value(&x);
    return 0;
}