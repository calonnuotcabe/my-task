#include <stdio.h>
#include <stdint.h>

static int get_u16(uint8_t *packet, size_t count, size_t offset, uint16_t *value) {
    if ((offset+1) >= count) {
        return -1;
    }
    *value = (uint16_t)(packet[offset+1] << 8);
    *value = *value | (uint16_t)(packet[offset]);
    printf("0x%04x\n", *value);
    printf("%zu\n",(long unsigned)offset);
    return 0;
}

int main() {
    uint16_t value = 0U;
    uint8_t packet[6] = {0xAAU, 0xBBU, 0x34U, 0x12U, 0xCCU, 0xDDU};
    const size_t count = sizeof packet / sizeof packet[0];
    if (get_u16(packet, count, 4, &value) != 0) {
        printf("0xdeadbeef\n");
        return -1;
    }
    return 0;
}