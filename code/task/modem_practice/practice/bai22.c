#include <stdio.h>
#include <stdint.h>
#define MODEM_RING_SIZE 8U

_Static_assert((MODEM_RING_SIZE & (MODEM_RING_SIZE - 1U)) == 0, "event ring modem must be power of two");

int main() {
    size_t valid_ring = MODEM_RING_SIZE;
    printf("[*] ring size = %u\n", (unsigned)valid_ring);
    printf("[*] ring configuration valid\n");
    return 0;
}