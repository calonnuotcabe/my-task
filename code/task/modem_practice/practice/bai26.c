#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>

#define MODEM_PAYLOAD_BYTES 32U
#define MODEM_HEADER_BYTES 4U
#define MODEM_FRAME_BYTES (MODEM_PAYLOAD_BYTES+MODEM_HEADER_BYTES)
#define MODEM_RING_SIZE 8U
#define DMA_DESCRIPTOR_SIZE 64U

_Static_assert(((MODEM_RING_SIZE & (MODEM_RING_SIZE - 1U)) == 0), "event ring must be a power of two\n");

enum MODEM_EVENT {
    MODEM_EVENT_INIT,
    MODEM_EVENT_TX,
    MODEM_EVENT_DONE,
};

struct runtime {
    _Atomic uint32_t write_index;
    void (*callback)(enum MODEM_EVENT *enum_event);
    volatile uint32_t *reg_pointer;
};

void modem_event(enum MODEM_EVENT *modem_event) {
    switch(*modem_event) {
        case MODEM_EVENT_INIT:
            printf("[*] event: INIT\n");
            break;
        case MODEM_EVENT_TX:
            printf("[*] event: TX\n");
            break;
        case MODEM_EVENT_DONE:
            printf("[*] event: DONE\n");
            break;
        default:
            printf("UNKNOWN\n");
    }
}

void modem_run(void(*callback)(enum MODEM_EVENT *modem_event), enum MODEM_EVENT *modem_event) {
    callback(modem_event);
}

int modem_init(struct runtime *modem_runtime) {
    uint32_t value = 0U;
    memory_order order = memory_order_relaxed;
    enum MODEM_EVENT modem_event = MODEM_EVENT_INIT;
    atomic_store_explicit(&(modem_runtime->write_index), value, order);
    *(modem_runtime->reg_pointer) = 1U;
    modem_runtime->callback(&modem_event);
    return 0;
}

int modem_send(struct runtime *modem_runtime, uint32_t frame_id, size_t payload_length) {
    enum MODEM_EVENT modem_event = MODEM_EVENT_TX;
    memory_order order = memory_order_relaxed;
    uint32_t increased;
    if (payload_length > MODEM_PAYLOAD_BYTES) {
        return -1;
    }
    modem_runtime->callback(&modem_event);
    *(modem_runtime->reg_pointer) = frame_id;
    printf("[*] frame id = %zu\n", (long unsigned)*(modem_runtime->reg_pointer));
    printf("[*] payload length = %zu\n", (long unsigned)payload_length);

    increased = atomic_load_explicit(&(modem_runtime->write_index), order);
    increased = increased + 1;
    atomic_store_explicit(&(modem_runtime->write_index), increased, order);
    modem_event = modem_event + 1;
    modem_runtime->callback(&modem_event);
    return 0;
}

int main() {
    _Alignas(64) uint8_t DMA_descriptor[DMA_DESCRIPTOR_SIZE];
    uintptr_t p = (uintptr_t)DMA_descriptor;
    uint32_t reg;
    volatile uint32_t *reg_pointer = &reg;
    uint32_t write_index = 5U;
    uint32_t frame_id = 42U;
    size_t payload_length = 30U;
    struct runtime modem_runtime = {write_index,modem_event,reg_pointer};
    memory_order order = memory_order_relaxed;
    uint32_t value;
    if (p % 64 == 0) {
        printf("[*] DMA alignment\n");
    }
    else {
        printf("[*] DMA alignment error\n");
    }
    modem_init(&modem_runtime);
    if (modem_send(&modem_runtime, frame_id, payload_length) < 0) {
        return -1;
    }
    value = atomic_load_explicit(&(modem_runtime.write_index), order);
    printf("[*] write index = %zu\n", (long unsigned)value);
    printf("[*] modem run successfully\n");
    return 0;
}