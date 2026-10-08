#include <stdio.h>
#include <stdint.h>
#define POOL_SLOT 4
#define POOL_SLOT_BYTES 32

enum State {
    POOL_CPU_OWNED,
    POOL_FREE,
};

struct handle {
    uint8_t slot;
    size_t length;
    uint8_t generation;
};

struct Slot {
    enum State state;
    size_t length;
    uint8_t data[POOL_SLOT_BYTES];
    uint8_t generation;
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

void SlotShowing(struct pool *modem_pool, struct handle *modem_handle) {
    printf("slot %zu\n", (long unsigned)modem_handle->slot);
    StateShowing(modem_pool->slot[modem_handle->slot]);
    printf("length: %zu\n", (long unsigned)modem_pool->slot[modem_handle->slot].length);
    printf("data: ");
    for (size_t index = 0; index < POOL_SLOT_BYTES; index++) {
        printf("%c", modem_pool->slot[modem_handle->slot].data[index]);
    }
    printf("\n");
}
void pool_init(struct pool *modem_pool) {
    for (size_t index = 0U; index < POOL_SLOT; index++) {
        modem_pool->slot[index].generation = 0U;
        modem_pool->slot[index].state = POOL_FREE;
        modem_pool->slot[index].length = 0;
        printf("slot %zu:\n", (long unsigned)index);
        StateShowing(modem_pool->slot[index]);
        printf("length: %zu\n", (long unsigned)modem_pool->slot[index].length);
        printf("\n");
    }
}

int pool_alloc(struct pool *modem_pool, size_t length, struct handle *modem_handle) {
    if (length == 0U || length > POOL_SLOT_BYTES) {
        return -1;
    }
    for (size_t index = 0U; index < POOL_SLOT; index++) {
        if (modem_pool->slot[index].state != POOL_FREE) {
            continue;
        }
        modem_pool->slot[index].state = POOL_CPU_OWNED;
        modem_pool->slot[index].length = length;
        modem_pool->slot[index].generation = modem_pool->slot[index].generation + 1;
        modem_handle->length = length;
        modem_handle->slot = index;
        modem_handle->generation = modem_pool->slot[index].generation;
        return 0;
    }
    return -2;
}

int pool_get(struct pool *modem_pool, struct handle *modem_handle, uint8_t **data) {
    if (modem_handle->slot >= POOL_SLOT) {
        return -1;
    }
    if (modem_pool->slot[modem_handle->slot].state != POOL_CPU_OWNED) {
        return -2;
    }
    if (modem_pool->slot[modem_handle->slot].length != modem_handle->length) {
        return -3;
    }
    if (modem_pool->slot[modem_handle->slot].generation != modem_handle->generation) {
        return -4;
    }
    *data = &(modem_pool->slot[modem_handle->slot].data[0]);
    return 0;
}

int pool_free(struct pool *modem_pool, struct handle *modem_handle) {
    if (modem_handle->slot >= POOL_SLOT) {
        return -1;
    }
    if (modem_handle->generation != modem_pool->slot[modem_handle->slot].generation) {
        return -3;
    }
    if (modem_pool->slot[modem_handle->slot].state == POOL_CPU_OWNED) {
        modem_pool->slot[modem_handle->slot].state = POOL_FREE;
        modem_pool->slot[modem_handle->slot].length = 0U;
        return 0;
    }
    printf("double free\n");
    return -2;
}

int main() {
    struct pool modem_pool = {0};
    struct handle modem_handle_A = {0};
    struct handle modem_handle_B = {0};
    uint8_t *data;
    size_t length = 8U;
    pool_init(&modem_pool);
    pool_alloc(&modem_pool, length, &modem_handle_A);
    pool_alloc(&modem_pool, length, &modem_handle_B);
    pool_get(&modem_pool, &modem_handle_A, &data);
    pool_get(&modem_pool, &modem_handle_B, &data);
    pool_free(&modem_pool, &modem_handle_A);
    SlotShowing(&modem_pool, &modem_handle_A);
    SlotShowing(&modem_pool, &modem_handle_B);
    return 0;
}