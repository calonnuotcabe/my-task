#include <stdio.h>
#include <stdint.h>

struct dma_descriptor {
    uint32_t src;
    uint32_t dest;
    uint32_t length;
};

void write_u32_le(uint8_t *buffer, uint32_t value) {
    buffer[0] = (uint8_t)value;
    buffer[1] = (uint8_t)(value >> 8);
    buffer[2] = (uint8_t)(value >> 16);
    buffer[3] = (uint8_t)(value >> 24);
}

void dma_descriptor_encode(uint8_t *buffer, struct dma_descriptor *desc) {
    write_u32_le(buffer, desc->src);
    write_u32_le(buffer + 4, desc->dest);
    write_u32_le(buffer + 8, desc->length);
}

int main() {
    struct dma_descriptor desc = {0x11223344, 0x55667788, 0x00000020};
    uint8_t byte[12];
    dma_descriptor_encode(byte, &desc);
    size_t count = sizeof byte / sizeof byte[0];
    for (size_t index = 0; index < count; index++) {
        printf("byte %zu: 0x%x\n", (long unsigned)index, byte[index]);
    }
    return 0;
}