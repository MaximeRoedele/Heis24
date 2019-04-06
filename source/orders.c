#include "elev.h"
#include "orders.h"
#include <stdio.h>
#include <stdlib.h>

static int up_orders[N_FLOORS - 1];		//Oversikt over bestillinger oppover
static int down_orders[N_FLOORS - 1];		//Oversikt over bestillinger nedover
static int elevator_orders[N_FLOORS];		//Oversikt over bestillinger p� panelet i heisen
static int order_at_floor[N_FLOORS];		//Oversikt over bestillinger i etasjer


void set_order(int floor) {										//bytt ut order
	if (floor < (N_FLOORS-1) && elev_get_button_signal(BUTTON_CALL_UP, floor)) {		//Sjekker om knapp er trykket inn
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);			//Generelt for de tre:
		up_orders[floor] = 1;									//Setter lampen p� bestillingsknappen for den gitte etasjen
		order_at_floor[floor] = 1;								//og oppdaterer registrene
	}
	if (floor > 0 && elev_get_button_signal(BUTTON_CALL_DOWN, floor)) {
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
		down_orders[floor-1] = 1;
		order_at_floor[floor] = 1;
	}
	if (elev_get_button_signal(BUTTON_COMMAND, floor)) {
		elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
		elevator_orders[floor] = 1;
		order_at_floor[floor] = 1;
	}
}

void poll_orders() {
	int i;
	for (i = 0; i < N_FLOORS; i++) {								//G�r over alle 4 etasjene og sjekker om vi f�r igjen et signal fra en av
		set_order(i);
	}
}

int get_up_order(int floor) {
	if (up_orders[floor] && floor < (N_FLOORS-1)) {
		return 1;
	}
	return 0;
}


int get_down_order(int floor) {
	if (down_orders[floor] && floor > 0) {
		return 1;
	}
	return 0;
}

int get_elev_order(int floor) {
	if (elevator_orders[floor]){
		return 1;
	}
	return 0;
}

int get_order_at_floor(int floor){
	if (order_at_floor[floor] == 1){
		return 1;
	}
	return 0;
}

int are_there_orders_above_me(int floor){
	int i;
	for (i=floor+1; i<N_FLOORS; i++){
		if(get_order_at_floor(i)){
			return 1;
		}
	}
	return 0;
}

int are_there_orders_below_me(int floor){
	int i;
	for (i=0; i<floor; i++){
		if(get_order_at_floor(i)){
			return 1;
		}
	}
	return 0;
}

int out_of_orders(){
	int i;
	for (i = 0; i < N_FLOORS; i++){
		if (get_order_at_floor(i)){
			return 0;
		}
	}
	return 1;
}

void clear_orders_at_floor(int floor) {
	if (floor > 0 && floor < N_FLOORS) {
		down_orders[floor-1] = 0;
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
	}
	if (floor < (N_FLOORS - 1)) {
		up_orders[floor] = 0;
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
	}
	elevator_orders[floor] = 0;
	order_at_floor[floor] = 0;
	elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
}

void excecute_order_66() {
	int i;
	for (i = 0; i < N_FLOORS; i++)
		clear_orders_at_floor(i);
}
