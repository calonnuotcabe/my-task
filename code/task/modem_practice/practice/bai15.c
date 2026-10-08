#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void decoder(uint32_t *value, uint8_t *wire) {
    *value = (uint32_t)wire[3] << 24;
    *value = *value | ((uint32_t)wire[2] << 16);
    *value = *value | ((uint32_t)wire[1] << 8);
    *value = *value | ((uint32_t)wire[0]);
    printf("0x%x\n", *value);

}

int main() {
    uint32_t value = 0;
    uint8_t wire[4] = {0x78, 0x56, 0x34, 0x12};
    decoder(&value, wire);
    return 0;
}