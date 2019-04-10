/**
* @file
* @brief A library including all the elements to control the elevator. This includes
* initialization, stop conditions, turning conditions and the final state machine for
* the complete system. To run the elevator with the worked functionality, call
* control_run_elevator_fsm in the main.c file and compile the project.
*/

#pragma once
#include <stdlib.h>

/**
* @brief Initializez the elevator. Drives it from an undefined position, meaning one
* between two floors, to the first one below while not accepting any orders. If it
* is in a defined position this function does nothing.
*/
void control_init_movement();

/**
* @brief Checks if the elevator, on the desired floor,
* fulfills any stop-conditions described in the functional requirement.
*
* @param[in] floor The desired floor.
*
* @return 0 if stop conditions are not met on the spesific floor. 1 if any of
* them are met and the elevator is meant to stop.
*/
int control_should_i_stop(int floor);

/**
* @brief Initializez a stop at a floor. Stops the motor, opens the door and clears all
* orders at the floor. Uses m_current_floor set in control_should_i_stop.
*/
void control_stop_at_floor();

/**
* @brief Runs the elevator FSM with the 5 declared states. This is the only function
* one needs to call in main.c.
*/
void control_run_elevator_fsm();
