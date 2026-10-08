#include <stdio.h>
#include <stdint.h>

struct rfBackend {
    int (*txSubmit)(uint32_t);
};

int txSubmit(uint32_t frame_id) {
    printf("[*]Frame ID = %zu\n", (long unsigned)frame_id);
    return 0;
}
int modem_transmit(uint32_t frame_id, struct rfBackend *backend) {
    if (backend->txSubmit(frame_id) != 0) {
        printf("[*]transmit failed\n");
        return -1;
    }
    printf("[*]transmit finished\n");
    return 0;
}

int main() {
    uint32_t frame_id = 42U;
    struct rfBackend backend = {txSubmit};
    if (modem_transmit(frame_id, &backend) != 0) {
        return -1;
    }
    return 0;
}