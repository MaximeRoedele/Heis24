#pragma once
#include <stdlib.h>

#include "elev.h"



void set_order(int floor);

void poll_orders();

//Get funksjoner for alle listene
int get_up_order(int floor);
int get_down_order(int floor);
int get_elev_order(int floor);

void clear_orders_at_floor(int floor);

//Sletter alle bestillinger i order_type arrayene
void excecute_order_66();