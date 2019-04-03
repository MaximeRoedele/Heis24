#include "elev.h"
#include <stdio.h>
#include "orders.h"
#include "control.h"

//git-test 2
int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    init_movement();
    printf("Press STOP button to stop elevator and exit program.\n");

    //elev_set_motor_direction(DIRN_UP);

    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }
        poll_orders();
        if (elev_get_floor_sensor_signal() >= 0 && elev_get_floor_sensor_signal() < N_FLOORS){
                stop_if_order_at_floor();
        }
        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}
