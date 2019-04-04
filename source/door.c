#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "door.h"
#include "elev.h"
#include "orders.h"

int time_elapsed = 0;
int trigger = 3000;

void everything_is_open(){
    clock_t time_timer_start = clock();
    elev_set_door_open_lamp(1);
    while (time_elapsed <= trigger){
        poll_orders();
        clock_t difference = clock() - time_timer_start;
        time_elapsed = difference * 1000 / CLOCKS_PER_SEC;
    }
    elev_set_door_open_lamp(0);
    time_elapsed = 0;
}
