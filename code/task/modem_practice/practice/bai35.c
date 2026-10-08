#include <stdio.h>
#include <stdint.h>
#define MEM_BASE 0x00100000
#define MEM_SIZE 0x1000
struct dma_descriptor {
    uint32_t source;
    uint32_t destination;
    uint32_t length;
};

int range_valid(uint32_t addr, uint32_t length) {
    if (addr < MEM_BASE) {
        return 0;
    }
    uint32_t offset = addr - MEM_BASE;
    if (offset >= MEM_SIZE) {
        return 0;
    }
    if (length > MEM_SIZE - offset) {
        return 0;
    }
    return 1;
}

int dma_descriptor_valid(struct dma_descriptor *desc) {
    if (desc->length == 0) {
        return -1;
    } 
     if (range_valid(desc->source, desc->length) == 0) {
        return -2;
    }
    if (range_valid(desc->destination, desc->length) == 0) {
        return -3;
    }
    return 0;
}


int main() {
    struct dma_descriptor desc = {0x100000U,0x100FFFU,32U};
    if (range_valid(desc.source, desc.length) == 0) {
        return -3;
    };
    if (range_valid(desc.destination, desc.length) == 0) {
        return -3;
    };
    dma_descriptor_valid(&desc);
    return 0;
}