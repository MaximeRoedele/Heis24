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

static state m_current_state = INIT;
static int m_current_floor;
static int m_next_floor;
static elev_motor_direction_t m_motor_direction;


void control_init_movement() {
	m_current_state = INIT;
	m_motor_direction = DIRN_DOWN;
	elev_set_motor_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1){

	}
	elev_set_motor_direction(DIRN_STOP);
	m_current_floor = elev_get_floor_sensor_signal();
};

int control_should_i_stop(int floor){
	m_current_floor = floor;
	elev_set_floor_indicator(m_current_floor);
	if (orders_get_elev_order(m_current_floor)){
		return 1;
	}
	if ((m_motor_direction == DIRN_UP && m_current_floor == (N_FLOORS-1))|| (m_motor_direction == DIRN_DOWN && m_current_floor == 0)){
		return 1;
	}
	if ((m_motor_direction == DIRN_UP && orders_get_up_order(m_current_floor)) || (m_motor_direction == DIRN_DOWN && orders_get_down_order(m_current_floor))){
		return 1;
	}
	else if (orders_get_order_at_floor(m_current_floor)){
		if (!orders_are_there_orders_below_me(m_current_floor) && !orders_are_there_orders_above_me(m_current_floor)){
			return 1;
		}
		if (m_motor_direction == DIRN_UP){
			if(orders_are_there_orders_above_me(m_current_floor) && orders_get_down_order(m_current_floor+1)){
				return 0;
			}
			else if (orders_are_there_orders_below_me(m_current_floor) && orders_get_down_order(m_current_floor-1)){
				return 1;
			}
			else if (!orders_get_order_at_floor(m_current_floor+1) && !orders_get_order_at_floor(m_current_floor-1)&&!orders_are_there_orders_above_me(m_current_floor)){
				return 1;
			}
		}
		else if(m_motor_direction == DIRN_DOWN){
			if (orders_are_there_orders_below_me(m_current_floor) && orders_get_up_order(m_current_floor-1)){
				return 0;
			}
			else if(orders_are_there_orders_above_me(m_current_floor) && orders_get_up_order(m_current_floor+1)){
				return 1;
			}
			else if (!orders_get_order_at_floor(m_current_floor+1) && !orders_get_order_at_floor(m_current_floor-1)&&!orders_are_there_orders_below_me(m_current_floor)){
				return 1;
			}
		}
	}

	m_next_floor = m_motor_direction + m_current_floor;
	return 0;
}



void control_stop_at_floor(){
	elev_set_motor_direction(DIRN_STOP);
	door_open_timer();
	orders_clear_orders_at_floor(m_current_floor);
}



void control_run_elevator_fsm(){
	while (1){
		switch (m_current_state) {
			case INIT:
				control_init_movement();
				m_current_state = IDLE;
				break;
			case IDLE:
				if (elev_get_stop_signal()){
					m_current_state = STOP;
					break;
				}
				orders_poll_orders();
				int i;
				for (i=0;i<N_FLOORS;i++){
					if (elev_get_floor_sensor_signal() == -1 && orders_get_order_at_floor(i)){
						float current_pos = (m_current_floor + m_next_floor)/2.0;
						if (i > current_pos){
							m_motor_direction = DIRN_UP;
							elev_set_motor_direction(m_motor_direction);
						}
						else if (i < current_pos) {
							m_motor_direction = DIRN_DOWN;
							elev_set_motor_direction(m_motor_direction);
						}
						m_current_state = MOVING;
						break;
					}
					else if (i == elev_get_floor_sensor_signal() && orders_get_order_at_floor(i)){
						m_current_state = DOOR_OPEN;
						break;
					}
					else if (orders_get_order_at_floor(i) && i != m_current_floor && elev_get_floor_sensor_signal() != -1){
						m_motor_direction = (i-m_current_floor)/(abs(i-m_current_floor));
						elev_set_motor_direction(m_motor_direction);
						m_current_state = MOVING;
						break;
					}
				}
				break;
			case MOVING:
				if (elev_get_stop_signal()){
					m_current_state = STOP;
					break;
				}
				if (m_next_floor == -1){
					m_next_floor = m_current_floor + m_motor_direction;
				}
				orders_poll_orders();
				int floor_sensor = elev_get_floor_sensor_signal();
				if (floor_sensor != -1){
					if (control_should_i_stop(floor_sensor)){
						m_current_state = DOOR_OPEN;
						break;
					}
				}
				break;
			case DOOR_OPEN:
				if (elev_get_stop_signal()){
					m_current_state = STOP;
					break;
				}
				m_next_floor = -1;
				orders_poll_orders();
				control_stop_at_floor();

				if (orders_out_of_orders()){
					m_current_state = IDLE;
					break;
				}
				else if ((m_motor_direction==DIRN_DOWN && orders_are_there_orders_below_me(m_current_floor) )|| (m_motor_direction==DIRN_UP && orders_are_there_orders_above_me(m_current_floor))){
					m_current_state=MOVING;
					elev_set_motor_direction(m_motor_direction);
					break;
				}
				else{
					m_motor_direction = m_motor_direction *(-1);
					elev_set_motor_direction(m_motor_direction);
					m_current_state = MOVING;
					break;
				}
				break;
			case STOP:
				orders_excecute_order_66();
				elev_set_stop_lamp(1);
				elev_set_motor_direction(DIRN_STOP);
				if (elev_get_floor_sensor_signal()!= -1){
					elev_set_door_open_lamp(1);
				}
				while (elev_get_stop_signal()==1) {}
				elev_set_stop_lamp(0);
				if(elev_get_floor_sensor_signal()!= -1){
					door_open_timer();
					if(elev_get_stop_signal()){
						m_current_state = STOP;
					}
				}
				m_current_state=IDLE;
				break;
		}
	}
}
