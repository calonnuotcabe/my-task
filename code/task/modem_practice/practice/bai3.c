#include <stdio.h>
#include <stdint.h>

static void calculate_sum(uint32_t *number, size_t count, uint32_t *result) {
    for (size_t i = 0; i < count; i++) {
        *result = *result + number[i];
    }
}

int main() {
    uint32_t result = 0U;
    uint32_t number[4] = {10U, 20U, 30U, 40U};
    const size_t count = sizeof number / sizeof number[0];
    calculate_sum(number, count, &result);
    printf("[*]result=%u\n", result);
    return 0;
}