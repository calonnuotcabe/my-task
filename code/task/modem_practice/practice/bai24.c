#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t reg;
    volatile uint32_t *ptr = &reg;
    *ptr = 0xA5A50001U;
    uint32_t value = *ptr;
    printf("value = 0x%x\n", (unsigned)value);
    return 0;
}