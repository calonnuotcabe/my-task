#include <stdio.h>
#include <stdint.h>
#define DMA_DESCRIPTOR_SIZE 64U

int check(uintptr_t valid_addr) {
    if ((unsigned)valid_addr % 64 == 0) {
        printf("[*]valid\n");
        return 0;
    }
    else {
        return -1;
    }
}

int main() {
    _Alignas(64) uint8_t descriptor[DMA_DESCRIPTOR_SIZE];
    uintptr_t valid_addr = (uintptr_t)descriptor;
    if (check(valid_addr) == -1) {
        return -1;
    }
    return 0;
}