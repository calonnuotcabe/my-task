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

int main() {
    uint32_t used;
    uint32_t write_index = 0U;
    uint32_t read_index = 0U;
    struct EVENT_RING event_ring = {0};
    event_ring.capacity = 8U;
    atomic_store_explicit(&(event_ring.write_index), 3U, memory_order_relaxed);
    atomic_store_explicit(&(event_ring.read_index), 1U, memory_order_relaxed);
    write_index = atomic_load_explicit(&(event_ring.write_index), memory_order_relaxed);
    read_index = atomic_load_explicit(&(event_ring.read_index), memory_order_relaxed);
    used = write_index - read_index;
    printf("[*] result = %" PRIu32 "\n", used);
    return 0;
}