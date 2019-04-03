#include <iostream>
#include "control.h"
#include "elev.h"
#include "orders.h"

static int current_floor;
static int next_floor;
static elev_motor_direction_t motor_direction;

void init_movement() {
	elev_set_motor_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1){					//Så lenge ingen sensorer reagerer skal heisen bevege seg nedover

	}
	elev_set_motor_direction(DIRN_STOP);						//Når en sensor reagerer skal heisen stoppe
	current_floor = elev_get_floor_sensor_signal();
	
};

//exec_order skal bestemme retningen til heisen og sette igang motoren etter et stopp utført av stop_if_order_at_floor eller stoppknappen.
void exec_order() {
	//X hvis retning opp og ordre over skal den opp
	//X hvis retning opp og ordre under og ikke ordre over skal den ned
	//X hvis retning opp og ordre over og under skal den opp
	//X hvis retning ned og ordre under skal den ned
	//X hvis retning ned og ordre over og ikke under skal den opp
	//X hvis retning ned og ordre over og under skal den ned
		//trenger ikke ta hensyn til endetilfellene (etasje 0 og 3), for retningen der skiftes i gitt kode. Se main.c
		// + - 1 tar hensyn til ordre i etasjen rett over/under og ikke generelt alle etasjen over/under, men bør gå her siden det kun er 4 etasjer. PS funker ikke. Gjør noe bedre
		if ((current_floor > 0) && (current_floor < N_FLOORS-1)) {
			if ((get_elev_order(/* alle etasjer over */) && get_elev_order(/* alle etasjer under */)) ||
				(motor_direction==DIRN_DOWN && get_elev_order(/* alle etasjer under */)) || (motor_direction==DIRN_UP && get_elev_order(/* alle etasjer over */))) {
				motor_direction = elev_set_motor_direction(motor_direction);
			}
			else if ((motor_direction == DIRN_DOWN && !get_elev_order(/* alle etasjer under */) && gel_elev_order(/* alle etasjer over */)) ||
				(motor_direction == DIRN_UP && !get_elev_order(/* alle etasjer over */) && get_elev_order(/* alle etasjer under */))) {
				motordirection = -elev_set_motor_direction(motor_direction);  //usikker på om - funker her.
			}
			//start motor her
		}																				


}

//Stop_if_order_at_floor stopper heisen på en forbipasserende etasje dersom krav for stopp er tilfredsstillt. 
void stop_if_order_at_floor() {
	current_floor = elev_get_floor_sensor_signal();
	elev_set_floor_indicator(current_floor);
	if ((get_elev_order(current_floor)) {
		if ((motor_direction == DIRN_UP && get_up_order(current_floor)) ||
			(motor_direction == DIRN_DOWN && get_down_order(current_floor))) {
			//stop og åpne dør
			clear_orders_at_floor(current_floor);
		}
	}
	else() {
		if (motor_direction = DIRN_UP) {													//Bør dobbeltsjekke at next_floor ikke går ut av scope
			next_floor = current_floor + 1;
		}
		else if (motor_direction = DIRN_DOWN) {
			next_floor = current_floor - 1;
		}
	}
}