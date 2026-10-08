#include <stdio.h>
#include <stdint.h>
#define DMA_FLAG_OWN (1U << 0)
#define MEM_BASE 0x100000
#define MEM_SIZE 0x1000
enum dma_state {
    DMA_FREE,
    DMA_CPU_OWNED,
    DMA_DEVICE_OWNED,
    DMA_DONE
};

struct virtual_memory {
    uint8_t bytes[MEM_SIZE];
};

enum reg_status {
    IDLE,
    BUSY,
    DONE,
    ERROR,
};

struct dma_registers {
    uint32_t descriptor_addr;
    uint32_t doorbell;
    enum reg_status status;
    uint32_t completion_cookie;
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

struct dma_engine {
    struct dma_registers regs;
    struct dma_slot *active_slot;
};

int range_valid(uint32_t addr, uint32_t length) {
    if (addr < MEM_BASE) {
        return -1;
    }
    uint32_t offset = addr - MEM_BASE;
    if (offset >= MEM_SIZE) {
        return -1;
    }
    if (length > MEM_SIZE - offset) {
        return -1;
    }
    return 0;
}

int phys_to_ptr(struct virtual_memory *memory, uint32_t physical_addr, uint32_t length, uint8_t **ptr) {
    if (length == 0) {
        return -1;
    }
    if ((range_valid(physical_addr, length)) < 0) {
        return -2;
    }
    *ptr = &memory->bytes[physical_addr - MEM_BASE];
    return 0;
}

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

int dma_engine_init(struct dma_engine *engine) {
    engine->regs.descriptor_addr = 0;
    engine->regs.doorbell = 0;
    engine->regs.status = IDLE;
    engine->regs.completion_cookie = 0U;
    engine->active_slot = NULL;
    return 0;
}

int dma_submit(struct dma_engine *engine, struct dma_slot *slot, uint32_t descriptor_addr) {
    if (slot->state !=  DMA_DEVICE_OWNED || engine->regs.status != IDLE) {
        return -1;
    }
    engine->regs.descriptor_addr = descriptor_addr;
    dma_descriptor_encode(slot);
    engine->active_slot = slot;
    engine->regs.doorbell = 1U;
    engine->regs.status = BUSY;
    return 0;
}

int dma_engine_step(struct dma_engine *engine) {
    if ((engine->regs.doorbell != 1U) || (engine->regs.status != BUSY) || (engine->active_slot == NULL)) {
        return -1;
    }
    if (dma_complete(engine->active_slot) != 0 ) {
        return -2;
    }
    engine->regs.completion_cookie = engine->active_slot->desc.cookie;
    engine->regs.status = DONE;
    engine->regs.doorbell = 0;
    return 0;
}

int dma_check_completion(struct dma_engine *engine) {
    if (engine->regs.status != DONE) {
        return -1;
    }
    uint32_t cookie = engine->regs.completion_cookie;
    printf("I complete your cookie, here is it: 0x%x\n", (unsigned)cookie);
    dma_reclaim(engine->active_slot);
    engine->regs.status = IDLE;
    engine->active_slot = NULL;
    return 0;
}


int main() {
    struct dma_slot slot = {0};
    struct dma_engine engine = {0};
    dma_engine_init(&engine);
    dma_prepare(&slot, 0x11223344, 0x55667788, 0x20, 0xdeadbeef);
    dma_publish(&slot);
    dma_submit(&engine, &slot, 0x00100000);
    dma_engine_step(&engine);
    dma_check_completion(&engine);
    return 0;
}