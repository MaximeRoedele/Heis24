#pragma once
#include <iostream>
#include "elev.h"
#include "orders.h"

//Kjører ned til første etasje den treffer. Oppdaterer current_floor.
void init_movement();	
//Bestemmer retning av heis for neste Order og utfører neste Order.
void exec_order();
//Sjekker om heisen skal stoppe i passerede etasje.
bool check_if_order();
//Stopper heisen
void stop_at_floor();

