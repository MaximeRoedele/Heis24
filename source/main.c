#include "elev.h"
#include <stdio.h>
#include "orders.h"
#include "control.h"
#include "door.h"

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    control_run_elevator_fsm();

    return 0;
}
