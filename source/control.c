#include <stdlib.h>
#include "control.h"
#include "elev.h"
#include "orders.h"

static int current_floor;
static int next_floor;
static elev_motor_direction_t motor_direction;

void init_movement() {
	elev_set_motor_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1){					//S� lenge ingen sensorer reagerer skal heisen bevege seg nedover

	}
	elev_set_motor_direction(DIRN_STOP);						//N�r en sensor reagerer skal heisen stoppe
	current_floor = elev_get_floor_sensor_signal();

};

int next_direction(){
	if (motor_direction )
}

//exec_order skal bestemme retningen til heisen og sette igang motoren etter et stopp utf�rt av stop_if_order_at_floor eller stoppknappen.
void exec_order() {
	if (elev_get_floor_sensor_signal >= 0 && elev_get_floor_sensor_signal < N_FLOORS){
		switch (current_floor) {
			case 0:
				int i;
				for (i = 1; i < N_FLOORS; i++){
					if (get_order_at_floor(i) == 1){
						elev_set_motor_direction(DIRN_UP);
						motor_direction = DIRN_UP;
						break;
					}
				}
				break;
			case 1:
				if ()
		}
	}

	else {
		//Stopp var trykka og vi er mellom to etasjer
	}

	//X hvis retning opp og ordre over skal den opp
	//X hvis retning opp og ordre under og ikke ordre over skal den ned
	//X hvis retning opp og ordre over og under skal den opp
	//X hvis retning ned og ordre under skal den ned
	//X hvis retning ned og ordre over og ikke under skal den opp
	//X hvis retning ned og ordre over og under skal den ned
		//trenger ikke ta hensyn til endetilfellene (etasje 0 og 3), for retningen der skiftes i gitt kode. Se main.c
		// + - 1 tar hensyn til ordre i etasjen rett over/under og ikke generelt alle etasjen over/under, men b�r g� her siden det kun er 4 etasjer. PS funker ikke. Gj�r noe bedre
		if ((current_floor > 0) && (current_floor < N_FLOORS-1)) {
			if ((get_elev_order() && get_elev_order()) ||
				(motor_direction==DIRN_DOWN && get_elev_order()) || (motor_direction==DIRN_UP && get_elev_order())) {
				motor_direction = elev_set_motor_direction(motor_direction);
			}
			else if ((motor_direction == DIRN_DOWN && !get_elev_order() && gel_elev_order()) ||
				(motor_direction == DIRN_UP && !get_elev_order() && get_elev_order())) {
				motordirection = elev_set_motor_direction((-1) * motor_direction);  //usikker p� om - funker her.
			}
			//start motor her
		}


}

//Stop_if_order_at_floor stopper heisen p� en forbipasserende etasje dersom krav for stopp er tilfredsstillt.
void stop_if_order_at_floor() {
	current_floor = elev_get_floor_sensor_signal();
	elev_set_floor_indicator(current_floor);
	clear_orders_at_floor(current_floor);
	if (get_elev_order(current_floor)) {
		if ((motor_direction == DIRN_UP && get_up_order(current_floor)) ||
			(motor_direction == DIRN_DOWN && get_down_order(current_floor))) {
			//stop og �pne d�r
			clear_orders_at_floor(current_floor);
		}
	}
	else {
		if (motor_direction == DIRN_UP) {													//B�r dobbeltsjekke at next_floor ikke g�r ut av scope
			next_floor = current_floor + 1;
		}
		else if (motor_direction == DIRN_DOWN) {
			next_floor = current_floor - 1;
		}
	}
}
