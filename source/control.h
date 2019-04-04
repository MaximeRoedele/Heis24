#pragma once
#include <stdlib.h>



//Kj�rer ned til f�rste etasje den treffer. Oppdaterer current_floor.
void init_movement();

int should_i_stop(int floor);

void stop_at_floor();
//Kjører hele heisen med retningslogikk
void run_elevator_fsm();
