#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    (void)signum;
}

void initi() {
    signal(14, handler);
    alarm(0x78);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
}

int main() {
    initi();
    return 0;
}