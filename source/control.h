#pragma once
#include <iostream>
#include "elev.h"
#include "orders.h"

int current_floor;
int next_floor;
elev_motor_direction_t motor_direction;

//Kjører ned til første etasje den treffer. Oppdaterer current_floor.
void init_movement(int *curret_floor);	
//Bestemmer retning av heis for neste Order og utfører neste Order.
void exec_order(elev_motor_direction_t motor_direction, int current_floor, int next_floor, bool order_at_floor);
//Sjekker om heisen skal stoppe i passerede etasje.
bool check_if_order(elev_motor_direction_t motor_direction, bool order_at_floor);
//Stopper heisen
void stop_at_floor();

