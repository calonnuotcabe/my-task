#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>

int main() {
    _Atomic uint32_t write_index = 0U;
    memory_order order = memory_order_relaxed;
    uint32_t value = 5U;
    uint32_t result;
    atomic_store_explicit(&write_index, value, order);
    result = atomic_load_explicit(&write_index, order);
    printf("[*] write_index = %zu\n", (long unsigned)result);
    return 0;
}