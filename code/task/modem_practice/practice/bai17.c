#include <stdio.h>
#include <stdint.h>

enum state {
    OFF,
    SEARCHING,
    CAMPED,
    CONNECTING,
    CONNECTED 
};

static void change_state(enum state *stateModem) {
    switch (*stateModem) {
        case OFF:
            *stateModem = *stateModem + 1;
            printf("OFF -> SEARCHING\n");
            break;
        case SEARCHING:
            *stateModem = *stateModem + 1;
            printf("SEARCHING -> CAMPED\n");
            break;
        case CAMPED:
            *stateModem = *stateModem + 1;
            printf("CAMPED -> CONNECTING\n");
            break;
        case CONNECTING:
            *stateModem = *stateModem + 1;
            printf("CONNECTING -> CONNECTED\n");
            break;
        case CONNECTED:
            printf("already CONNECTED\n");
            break;
        default:
            printf("UNKNOWN\n");
    }

}


int main() {
    enum state stateModem = OFF;
    change_state(&stateModem);
    change_state(&stateModem);
    change_state(&stateModem);
    change_state(&stateModem);
    change_state(&stateModem);
    return 0;
}