#include <stdio.h>
#include <stdint.h>
#define POOL_SLOTS 4
#define POOL_BYTES 32
enum State {
    POOL_FREE,
    POOL_CPU_OWNED,
};

struct Slot {
    enum State state;
    size_t length;
    uint8_t data[POOL_BYTES];
};

struct pool {
   struct Slot slot[POOL_SLOTS];
};

void stateShowing(struct Slot slot) {
    switch(slot.state) {
        case POOL_FREE:
            printf("state FREE\n");
            break;
        case POOL_CPU_OWNED:
            printf("state OWNED\n");
            break;
        default:
            printf("UNKNOWN\n");
    }
}

void pool_init(struct pool *modem_pool) {
    size_t count = sizeof modem_pool->slot / sizeof modem_pool->slot[0];
    for (size_t index = 0; index < count; index++) {
        modem_pool->slot[index].state = POOL_FREE;
        modem_pool->slot[index].length = 0U;
        printf("slot %zu:\n", index);
        stateShowing((modem_pool->slot[index]));
        printf("length %zu\n", modem_pool->slot[index].length);
        printf("\n");
    }
    printf("INIT SUCCESSFULLY\n");
    printf("\n");
}

int pool_alloc(struct pool *modem_pool, size_t length, uint8_t *slot_index, uint8_t **data) {
    size_t count = sizeof modem_pool->slot / sizeof modem_pool->slot[0];
    if (length == 0U || length > POOL_BYTES) {
            return -1;
    }
    for (size_t index = 0; index < count; index++) {
        if (modem_pool->slot[index].state != POOL_FREE) {
            continue;
        }
        modem_pool->slot[index].state = POOL_CPU_OWNED;
        modem_pool->slot[index].length = length;
        *slot_index = index;
        *data = &(modem_pool->slot[index].data[0]);
        printf("ptr = %p\n", *data);
        printf("slot index: %zu\n", (long unsigned)*slot_index);
        return 0;
    }
    return -2;
}

int main() {
    struct pool modem_pool = {0};
    pool_init(&modem_pool);
    uint8_t slot_index;
    uint8_t *data;
    size_t length = 8;
    if (pool_alloc(&modem_pool, length, &slot_index, &data) < 0) {
        printf("ALLOC FAILED\n");
    }
    else {printf("ALLOC SUCCESSFULLY\n");}
    if (pool_alloc(&modem_pool, length, &slot_index, &data) < 0) {
        printf("ALLOC FAILED\n");
    }
    else {printf("ALLOC SUCCESSFULLY\n");}
    if (pool_alloc(&modem_pool, length, &slot_index, &data) < 0) {
        printf("ALLOC FAILED\n");
    }
    else {printf("ALLOC SUCCESSFULLY\n");}
    if (pool_alloc(&modem_pool, length, &slot_index, &data) < 0) {
        printf("ALLOC FAILED\n");
    }
    else {printf("ALLOC SUCCESSFULLY\n");}
    if (pool_alloc(&modem_pool, length, &slot_index, &data) < 0) {
        printf("ALLOC FAILED\n");
    }
    else {printf("ALLOC SUCCESSFULLY\n");}
    return 0;
}