#include <stdio.h>
#include <stdint.h>
#define DMA_FLAG_OWN (1U << 0)
enum dma_state {
    DMA_FREE,
    DMA_CPU_OWNED,
    DMA_DEVICE_OWNED,
    DMA_DONE
};

struct dma_descriptor {
    uint32_t source;
    uint32_t destination;
    uint32_t length;
    uint32_t flags;
    uint32_t cookie;
    uint32_t generation;
    uint32_t next;
    uint32_t reserved;
};

struct dma_slot {
    enum dma_state state;
    uint32_t generation;
    struct dma_descriptor desc;
    _Alignas(64) uint8_t raw_descriptor[32];
};
void write_u32_le(uint8_t *buffer, uint32_t value) {
    buffer[0] = (uint8_t)value;
    buffer[1] = (uint8_t)(value >> 8);
    buffer[2] = (uint8_t)(value >> 16);
    buffer[3] = (uint8_t)(value >> 24);
}
void dma_descriptor_encode(struct dma_slot *slot) {
    write_u32_le(slot->raw_descriptor, slot->desc.source);
    write_u32_le(slot->raw_descriptor + 4, slot->desc.destination);
    write_u32_le(slot->raw_descriptor + 8, slot->desc.length);
    write_u32_le(slot->raw_descriptor + 12, slot->desc.flags);
    write_u32_le(slot->raw_descriptor + 16, slot->desc.cookie);
    write_u32_le(slot->raw_descriptor + 20, slot->desc.generation);
    write_u32_le(slot->raw_descriptor + 24, slot->desc.next);
    write_u32_le(slot->raw_descriptor + 28, slot->desc.reserved);
}
int dma_prepare(struct dma_slot *slot, uint32_t source, uint32_t destination, uint32_t length, uint32_t cookie) {
    if (slot->state != DMA_FREE) {
        return -1;
    }
    if (length == 0) {
        return -2;
    }
    slot->generation = slot->generation + 1;
    slot->desc.source = source;
    slot->desc.destination = destination;
    slot->desc.cookie = cookie;
    slot->desc.generation = slot->generation;
    slot->desc.length = length;
    slot->desc.next = 0U;
    slot->desc.reserved = 0U;
    slot->desc.flags = 0U;
    dma_descriptor_encode(slot);
    slot->state = DMA_CPU_OWNED;
    return 0;
}

void dma_publish_barrier() {

}

int dma_publish(struct dma_slot *slot) {
    if (slot->state != DMA_CPU_OWNED) {
        return -1;
    }
    dma_publish_barrier();
    slot->desc.flags |= DMA_FLAG_OWN;
    dma_descriptor_encode(slot);
    dma_publish_barrier();
    slot->state = DMA_DEVICE_OWNED;
    return 0;
}

int dma_complete(struct dma_slot *slot) {
    if (slot->state != DMA_DEVICE_OWNED) {
        return -1;
    }
    slot->desc.flags &= ~DMA_FLAG_OWN;
    slot->state = DMA_DONE;
    dma_descriptor_encode(slot);
    return 0;
}

int dma_reclaim(struct dma_slot *slot) {
    if (slot->state != DMA_DONE) {
        return -1;
    }
    slot->desc.length = 0U;
    slot->desc.flags = 0U;
    dma_descriptor_encode(slot);
    slot->state = DMA_FREE;

    return 0;
}

int main() {
    struct dma_slot slot = {0};
    dma_prepare(&slot, 0x11223344, 0x55667788, 0x20, 0xdeadbeef);
    dma_publish(&slot);
    dma_complete(&slot);
    dma_reclaim(&slot);
    return 0;
}