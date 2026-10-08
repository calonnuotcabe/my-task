#include <stdio.h>
#include <stdint.h>

struct Sample {
    int16_t i;
    int16_t q;
};

static void change_sample(struct Sample *sample) {
    sample->i = 100;
    sample->q = 200;
}

int main() {
    struct Sample sample = {10, -20};
    change_sample(&sample);
    printf("sample = (%d, %d)\n", (int)sample.i, (int)sample.q);
    return 0;
}