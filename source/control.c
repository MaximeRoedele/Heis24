#include <stdlib.h>
#include "control.h"
#include "elev.h"
#include "orders.h"
#include "door.h"
#include <stdio.h>

typedef enum
{
	INIT,
	IDLE,
	MOVING,
	DOOR_OPEN
	//STOP
}state;

state current_state = INIT;
static int current_floor;
//static int next_floor;
static elev_motor_direction_t motor_direction;


void init_movement() {
	printf("Hei\n");
	current_state = INIT;
	elev_set_motor_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1){					//S� lenge ingen sensorer reagerer skal heisen bevege seg nedover

	}
	elev_set_motor_direction(DIRN_STOP);						//N�r en sensor reagerer skal heisen stoppe
	current_floor = elev_get_floor_sensor_signal();
	printf("%d\n", current_floor);
};

int should_i_stop(int floor){
	current_floor = floor;
	elev_set_floor_indicator(current_floor);
	if (get_elev_order(current_floor) || (motor_direction == DIRN_UP && get_up_order(current_floor)) || (motor_direction == DIRN_DOWN && get_down_order(current_floor))
	|| (get_order_at_floor(current_floor ) && !are_there_orders_below_me(current_floor) && !are_there_orders_above_me(current_floor))) {
		printf("Nå stopper jeg\n");
		return 1;
	}
	return 0;
}



void stop_at_floor(){
	elev_set_motor_direction(DIRN_STOP);
	everything_is_open();
	clear_orders_at_floor(current_floor);
}

void run_elevator_fsm(){
	while (1){
		switch (current_state) {
			case INIT:
				init_movement();
				current_state = IDLE;
				break;
			case IDLE:
				poll_orders();
				int i;
				for (i=0;i<N_FLOORS;i++){
					if (get_order_at_floor(i)){
						motor_direction = (i-current_floor)/(abs(i-current_floor));
						elev_set_motor_direction(motor_direction);
						current_state = MOVING;
						printf("motor direction %d", motor_direction);
						break;
					}
				}
				break;
			case MOVING:
				poll_orders();
				if (should_i_stop(elev_get_floor_sensor_signal())){
					current_state = DOOR_OPEN;
					break;
				}
				break;
			case DOOR_OPEN:
				printf("Døra di er åpen hihi\n");
				poll_orders();
				stop_at_floor();
				if (out_of_orders()){
					printf("Ingen ordre\n");
					current_state = IDLE;
					break;
				}
				if ((motor_direction==DIRN_DOWN && are_there_orders_below_me(current_floor) )|| (motor_direction==DIRN_UP && are_there_orders_above_me(current_floor))){
					current_state=MOVING;
					printf("Samme retning WOWE\n");
					elev_set_motor_direction(motor_direction);
					break;
				}
				else{
					printf("Ikke samme sadface\n");
					motor_direction = motor_direction *(-1);
					elev_set_motor_direction(motor_direction);
					current_state = MOVING;
					break;
				}
				break;
		}
	}

}
