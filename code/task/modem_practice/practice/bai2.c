#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint32_t double_value(uint32_t value) {
    return value * 2;
} 

int main() {
    uint32_t result;
    uint32_t x = 10U;
    result = double_value(x);
    printf("[*]result=%d\n", result);
    return 0;
}