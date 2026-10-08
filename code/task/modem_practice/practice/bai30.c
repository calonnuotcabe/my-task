#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdatomic.h>

struct EVENT_RING {
    _Atomic uint32_t write_index;
    _Atomic uint32_t read_index;
    uint32_t capacity;
    uint32_t entries[8];
};

int push_ring(struct EVENT_RING *event_ring, uint32_t event) {
    printf("Pushing...\n");
    uint32_t write_index;
    uint32_t read_index;
    uint32_t used;
    uint32_t slot;
    write_index = atomic_load_explicit(&(event_ring->write_index), memory_order_relaxed);
    read_index = atomic_load_explicit(&(event_ring->read_index), memory_order_acquire);
    used = write_index - read_index;
    if (used >= event_ring->capacity) {
        return -1;
    }
    else {
        slot = write_index & (event_ring->capacity - 1U);
        (event_ring->entries[slot]) = event;
        write_index = write_index + 1;
        atomic_store_explicit(&(event_ring->write_index), write_index, memory_order_release);
    }
    used = write_index - read_index;
    printf("[*] pushed event = %" PRIu32 "\n", event);
    printf("[*] write index = %" PRIu32 "\n", write_index);
    printf("[*] read index = %" PRIu32 "\n", read_index);
    printf("[*] used = %" PRIu32 "\n", used);
    return 0;
}

int pop_ring(struct EVENT_RING *event_ring, uint32_t *event) {
    printf("Poping...\n");
    uint32_t write_index;
    uint32_t read_index;
    uint32_t used;
    uint32_t slot;
    write_index = atomic_load_explicit(&(event_ring->write_index), memory_order_acquire);
    read_index = atomic_load_explicit(&(event_ring->read_index), memory_order_relaxed);
    used = write_index - read_index;
    if (write_index == read_index) {
        return -1;
    }
    else {
        slot = read_index & (event_ring->capacity - 1U);
        *event = event_ring->entries[slot];
        read_index = read_index + 1;
        atomic_store_explicit(&(event_ring->read_index), read_index, memory_order_release);
    }
    used = write_index - read_index;
    printf("[*] popped event = %" PRIu32 "\n", *event);
    printf("[*] write index = %" PRIu32 "\n", write_index);
    printf("[*] read index = %" PRIu32 "\n", read_index);
    printf("[*] used = %" PRIu32 "\n", used);
    return 0;
}

int main() {
    struct EVENT_RING event_ring = {0};
    uint32_t event = 42U;
    uint32_t write_index = 0U;
    uint32_t read_index = 0U;
    atomic_store_explicit(&(event_ring.write_index), write_index, memory_order_relaxed);
    atomic_store_explicit(&(event_ring.read_index), read_index, memory_order_relaxed);
    printf("Current state:\n");
    printf("[*] write index: %" PRIu32 "\n", write_index);
    printf("[*] read index: %" PRIu32 "\n", read_index);
    event_ring.capacity = 8U;
    if (push_ring(&event_ring, event) == -1) {
        return -1;
    }
    if (pop_ring(&event_ring, &event) == -1) {
        return -1;
    }
    return 0;
}