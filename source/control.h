#pragma once
#include <stdlib.h>
#include "elev.h"
#include "orders.h"

//Kj�rer ned til f�rste etasje den treffer. Oppdaterer current_floor.
void init_movement();


int next_direction();
//Bestemmer retning av heis for neste Order og utf�rer neste Order.
void exec_order();
//Sjekker om heisen skal stoppe i passerede etasje.
void stop_if_order_at_floor();
//Stopper heisen
void stop_at_floor();
