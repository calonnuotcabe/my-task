#include <stdint.h>
#include <stdio.h>

uint32_t count = 0U;
uint32_t limit = 100U;

int main() {
    if (count < limit) {
        count++;
        printf("%d\n", count);
    }
    return 0;
}