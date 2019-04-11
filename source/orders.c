#include "elev.h"
#include "orders.h"
#include <stdio.h>
#include <stdlib.h>

static int m_up_orders[N_FLOORS - 1];
static int m_down_orders[N_FLOORS - 1];
static int m_elevator_orders[N_FLOORS];
static int m_order_at_floor[N_FLOORS];

void orders_poll_orders() {
	int i;
	for (i = 0; i < N_FLOORS; i++) {
		if (i < (N_FLOORS-1) && elev_get_button_signal(BUTTON_CALL_UP, i)) {
			elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
			m_up_orders[i] = 1;
			m_order_at_floor[i] = 1;
		}
		if (i > 0 && elev_get_button_signal(BUTTON_CALL_DOWN, i)) {
			elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
			m_down_orders[i-1] = 1;
			m_order_at_floor[i] = 1;
		}
		if (elev_get_button_signal(BUTTON_COMMAND, i)) {
			elev_set_button_lamp(BUTTON_COMMAND, i, 1);
			m_elevator_orders[i] = 1;
			m_order_at_floor[i] = 1;
		}
	}
}

int orders_get_up_order(int floor) {
	if (m_up_orders[floor] && floor < (N_FLOORS-1)) {
		return 1;
	}
	return 0;
}


int orders_get_down_order(int floor) {
	if (m_down_orders[floor-1] && floor > 0) {
		return 1;
	}
	return 0;
}

int orders_get_elev_order(int floor) {
	if (m_elevator_orders[floor]){
		return 1;
	}
	return 0;
}

int orders_get_order_at_floor(int floor){
	if (m_order_at_floor[floor] == 1){
		return 1;
	}
	return 0;
}

int orders_are_there_orders_above_me(int floor){
	int i;
	for (i=floor+1; i<N_FLOORS; i++){
		if(orders_get_order_at_floor(i)){
			return 1;
		}
	}
	return 0;
}

int orders_are_there_orders_below_me(int floor){
	int i;
	for (i=0; i<floor; i++){
		if(orders_get_order_at_floor(i)){
			return 1;
		}
	}
	return 0;
}

int orders_out_of_orders(){
	int i;
	for (i = 0; i < N_FLOORS; i++){
		if (orders_get_order_at_floor(i)){
			return 0;
		}
	}
	return 1;
}

void orders_clear_orders_at_floor(int floor) {
	if (floor > 0 && floor < N_FLOORS) {
		m_down_orders[floor-1] = 0;
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
	}
	if (floor < (N_FLOORS - 1)) {
		m_up_orders[floor] = 0;
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
	}
	m_elevator_orders[floor] = 0;
	m_order_at_floor[floor] = 0;
	elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
}

void orders_clear_all_orders() {
	int i;
	for (i = 0; i < N_FLOORS; i++)
		orders_clear_orders_at_floor(i);
}
