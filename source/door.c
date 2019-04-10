#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "door.h"
#include "elev.h"
#include "orders.h"

int m_time_elapsed = 0;
int m_trigger = 3000;

void door_open_timer(){
    clock_t time_timer_start = clock();
    elev_set_door_open_lamp(1);
    while (m_time_elapsed <= m_trigger){
        orders_poll_orders();
        clock_t difference = clock() - time_timer_start;
        m_time_elapsed = difference * 1000 / CLOCKS_PER_SEC;
        if (elev_get_stop_signal()){
            break;
        }
    }
    elev_set_door_open_lamp(0);
    m_time_elapsed = 0;
}
