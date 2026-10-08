#include <stdio.h>
#include <stdint.h>

enum modemEvent {
    MODEM_SEARCH,
    MODEM_ATTACH,
    MODEM_CONNECTED,
};

void trace_event(enum modemEvent *event) {
    switch(*event) {
        case MODEM_SEARCH:
            printf("MODEM_SEARCH\n");
            break;
        case MODEM_ATTACH:
            printf("MODEM_ATTACH\n");
            break;
        case MODEM_CONNECTED:
            printf("MODEM_CONNECTED\n");
            break;
        default:
            printf("UNKNOWN\n");
    }
}

static void run_modem(void (*callback)(enum modemEvent *)) {
    enum modemEvent event1 = MODEM_SEARCH;
    enum modemEvent event2 = MODEM_ATTACH;
    enum modemEvent event3 = MODEM_CONNECTED;
    callback(&event1);
    callback(&event2);
    callback(&event3);

}

int main() {
    run_modem(trace_event);
    return 0;
}