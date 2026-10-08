#include <stdio.h>
#include <stdint.h>
#define POOL_SLOT 4
#define POOL_SLOT_BYTES 32

enum State {
    POOL_CPU_OWNED,
    POOL_FREE,
};

struct Slot {
    enum State state;
    size_t length;
    uint8_t data[POOL_SLOT_BYTES];
};

struct pool {
    struct Slot slot[POOL_SLOT];
};

void StateShowing(struct Slot slot) {
    switch(slot.state) {
        case POOL_CPU_OWNED:
            printf("STATE CPU_OWNED\n");
            break;
        case POOL_FREE:
            printf("STATE FREE\n");
            break;
        default:
            printf("UNKNOWN\n");
    }
}

void SlotShowing(struct pool *modem_pool, size_t slot_index) {
    printf("slot %zu\n", (long unsigned)slot_index);
    StateShowing(modem_pool->slot[slot_index]);
    printf("length: %zu\n", (long unsigned)modem_pool->slot[slot_index].length);
    printf("data: ");
    for (size_t index = 0; index < POOL_SLOT_BYTES; index++) {
        printf("%c", modem_pool->slot[slot_index].data[index]);
    }
    printf("\n");
}
void pool_init(struct pool *modem_pool) {
    for (size_t index = 0U; index < POOL_SLOT; index++) {
        modem_pool->slot[index].state = POOL_FREE;
        modem_pool->slot[index].length = 0;
        printf("slot %zu:\n", (long unsigned)index);
        StateShowing(modem_pool->slot[index]);
        printf("length: %zu\n", (long unsigned)modem_pool->slot[index].length);
        printf("\n");
    }
}

int pool_alloc(struct pool *modem_pool, size_t length, uint8_t *slot_index, uint8_t **data) {
    if (length == 0U || length > POOL_SLOT_BYTES) {
        return -1;
    }
    for (size_t index = 0U; index < POOL_SLOT; index++) {
        if (modem_pool->slot[index].state != POOL_FREE) {
            continue;
        }
        modem_pool->slot[index].state = POOL_CPU_OWNED;
        modem_pool->slot[index].length = length;
        *slot_index = index;
        *data = &(modem_pool->slot[index].data[0]);
        return 0;
    }
    return -2;
}

int pool_free(struct pool *modem_pool, uint8_t slot_index) {
    if (slot_index >= POOL_SLOT) {
        return -1;
    }
    if (modem_pool->slot[slot_index].state == POOL_CPU_OWNED) {
        modem_pool->slot[slot_index].state = POOL_FREE;
        modem_pool->slot[slot_index].length = 0U;
        return 0;
    }
    printf("double free\n");
    return -2;
}

int main() {
    struct pool modem_pool = {0};
    uint8_t slot_index;
    uint8_t *data;
    size_t length = 8U;
    pool_init(&modem_pool);
    pool_alloc(&modem_pool, length, &slot_index, &data);
    pool_alloc(&modem_pool, length, &slot_index, &data);
    pool_free(&modem_pool, 0);
    SlotShowing(&modem_pool, 0);

    return 0;
}