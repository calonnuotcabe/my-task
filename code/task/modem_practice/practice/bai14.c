#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void encoder(uint32_t *value, uint8_t *wire) {
    wire[0] = (uint8_t)*value;
    printf("[*]bit 1 = 0x%x\n", wire[0]);
    wire[1] = (uint8_t)(*value >> 8U);
    printf("[*]bit 2 = 0x%x\n", wire[1]);
    wire[2] = (uint8_t)(*value >> 16U);
    printf("[*]bit 3 = 0x%x\n", wire[2]);
    wire[3] = (uint8_t)(*value >> 24U);
    printf("[*]bit 4 = 0x%x\n", wire[3]);
}

int main() {
    uint32_t value = 0x12345678U;
    uint8_t wire[4] = {0};
    encoder(&value, wire);
    return 0;
}