/**
* @file
* @brief The source-code of all functions in control.h and an enum for the states.
*/


#include <stdlib.h>
#include "control.h"
#include "elev.h"
#include "orders.h"
#include "door.h"
#include <stdio.h>

///Enumerators for the various states of the final state machine.
typedef enum
{
	INIT, ///<The initialization state of the final state machine.
	IDLE, ///<The state of the elevator when it's standing still without orders.
	MOVING, ///<The state of the elevator when it's not standing still.
	DOOR_OPEN, ///<The state of the elevator as it stops in a floor and opens the door.
	STOP ///<The state the elevator enters when the STOP-button is pressed.
}state;

static state current_state = INIT;
static int current_floor;
static int next_floor;
static elev_motor_direction_t motor_direction;


void init_movement() {
	current_state = INIT;
	motor_direction = DIRN_DOWN;
	elev_set_motor_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1){

	}
	elev_set_motor_direction(DIRN_STOP);
	current_floor = elev_get_floor_sensor_signal();
};

int should_i_stop(int floor){
	current_floor = floor;
	elev_set_floor_indicator(current_floor);
	if (get_elev_order(current_floor)){
		return 1;
	}
	if ((motor_direction == DIRN_UP && current_floor == (N_FLOORS-1))|| (motor_direction == DIRN_DOWN && current_floor == 0)){
		return 1;
	}
	if ((motor_direction == DIRN_UP && get_up_order(current_floor)) || (motor_direction == DIRN_DOWN && get_down_order(current_floor))){
		return 1;
	}
	else if (get_order_at_floor(current_floor)){
		if (!are_there_orders_below_me(current_floor) && !are_there_orders_above_me(current_floor)){
			return 1;
		}
		if (motor_direction == DIRN_UP){
			if (are_there_orders_below_me(current_floor) && get_down_order(current_floor-1)){
				return 1;
			}
/*			else if(are_there_orders_above_me(current_floor) && get_down_order(current_floor+1)){
				return 0;
			}*/
		}
		else if(motor_direction == DIRN_DOWN){
/*			if (are_there_orders_below_me(current_floor) && get_up_order(current_floor-1)){
				return 0;
			}*/
			if(are_there_orders_above_me(current_floor) && get_up_order(current_floor+1)){
				return 1;
			}
		}
	}
	next_floor = motor_direction + current_floor;
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
				if (elev_get_stop_signal()){
					current_state = STOP;
					break;
				}
				poll_orders();
				int i;
				for (i=0;i<N_FLOORS;i++){
					if (elev_get_floor_sensor_signal() == -1 && get_order_at_floor(i)){
						float current_pos = (current_floor + next_floor)/2.0;
						if (i > current_pos){
							motor_direction = DIRN_UP;
							elev_set_motor_direction(motor_direction);
						}
						else if (i < current_pos) {
							motor_direction = DIRN_DOWN;
							elev_set_motor_direction(motor_direction);
						}
						current_state = MOVING;
						break;
					}
					else if (i == elev_get_floor_sensor_signal() && get_order_at_floor(i)){
						current_state = DOOR_OPEN;
						break;
					}
					else if (get_order_at_floor(i) && i != current_floor && elev_get_floor_sensor_signal() != -1){
						motor_direction = (i-current_floor)/(abs(i-current_floor));
						elev_set_motor_direction(motor_direction);
						current_state = MOVING;
						break;
					}
				}
				break;
			case MOVING:
				if (elev_get_stop_signal()){
					current_state = STOP;
					break;
				}
				if (next_floor == -1){
					next_floor = current_floor + motor_direction;
				}
				poll_orders();
				int floor_sensor = elev_get_floor_sensor_signal();
				if (floor_sensor != -1){
					if (should_i_stop(floor_sensor)){
						current_state = DOOR_OPEN;
						break;
					}
				}
				break;
			case DOOR_OPEN:
				if (elev_get_stop_signal()){
					current_state = STOP;
					break;
				}
				next_floor = -1;
				poll_orders();
				stop_at_floor();

				if (out_of_orders()){
					current_state = IDLE;
					break;
				}
				else if ((motor_direction==DIRN_DOWN && are_there_orders_below_me(current_floor) )|| (motor_direction==DIRN_UP && are_there_orders_above_me(current_floor))){
					current_state=MOVING;
					elev_set_motor_direction(motor_direction);
					break;
				}
				else{
					motor_direction = motor_direction *(-1);
					elev_set_motor_direction(motor_direction);
					current_state = MOVING;
					break;
				}
				break;
			case STOP:
				excecute_order_66();
				elev_set_stop_lamp(1);
				elev_set_motor_direction(DIRN_STOP);
				if (elev_get_floor_sensor_signal()!= -1){
					elev_set_door_open_lamp(1);
				}
				while (elev_get_stop_signal()==1) {}
				elev_set_stop_lamp(0);
				if(elev_get_floor_sensor_signal()!= -1){
					everything_is_open();
					if(elev_get_stop_signal()){
						current_state = STOP;
					}
				}
				current_state=IDLE;
				break;
		}
	}
}
