#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

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

uint32_t read_u32_le(uint8_t *buffer) {
    uint32_t value;
    value = (uint32_t)buffer[3] << 24;
    value = value | ((uint32_t)buffer[2] << 16);
    value = value | ((uint32_t)buffer[1] << 8);
    value = value | ((uint32_t)buffer[0]);
    printf("value = 0x%x\n", value);
    return value;
}

void dma_descriptor_decode(uint8_t *buffer, struct dma_descriptor *desc) {
    desc->src = read_u32_le(buffer);
    desc->dest = read_u32_le(buffer + 4);
    desc->length = read_u32_le(buffer + 8);
}

void dma_descriptor_encode(uint8_t *buffer, struct dma_descriptor *desc) {
    write_u32_le(buffer, desc->src);
    write_u32_le(buffer + 4, desc->dest);
    write_u32_le(buffer + 8, desc->length);
}

int main() {
    struct dma_descriptor desc = {0x11223344, 0x55667788, 0x00000020};
    uint8_t byte[12] = {0x44, 0x33, 0x22, 0x11, 0x88, 0x77, 0x66, 0x55, 0x20, 0x00, 0x00, 0x00};
    dma_descriptor_decode(byte, &desc);
    printf("desc = 0x%x, 0x%x, 0x%x \n", desc.src, desc.dest, desc.length);

    return 0;
}