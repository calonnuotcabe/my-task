#include <stdio.h>
#include <stdint.h>
#define MODEM_PAYLOAD_BYTES 32U
#define MODEM_HEADER_BYTES 4U
#define MODEM_FRAME_BYTES (MODEM_HEADER_BYTES + MODEM_PAYLOAD_BYTES)

int check(uint8_t *frame, size_t payload_length) {
    size_t valid_frame_bytes = MODEM_FRAME_BYTES;
    printf("[*] frame bytes = %u\n", (unsigned)valid_frame_bytes);
    if (payload_length > valid_frame_bytes) {
        printf("[*] payload rejected\n");
        return -1;
    }
    printf("[*] payload accepted\n");
    return 0;
}

int main() {
    uint8_t frame[MODEM_FRAME_BYTES];
    size_t payload_length = 30U;
    size_t count = sizeof frame / sizeof frame[0];
    check(frame, payload_length);
    return 0;
}