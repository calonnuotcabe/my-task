#include <stdio.h>
#include <stdint.h>

struct modemBackend {
    void (*search)(void);
    void (*connect)(void);
};

void search(void) {
    printf("[*]searching network\n");
}

void connect(void) {
    printf("[*]connected\n");
}

int main() {
    struct modemBackend modem_run = {search, connect};
    modem_run.search();
    modem_run.connect();
    return 0;
}