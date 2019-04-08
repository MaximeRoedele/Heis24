/**
* @file
* @brief A simple library handling orders from the outside and inside of
* the elevator, insering them into, deleting them and reading from respective
* lists.
*/
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "elev.h"


/**
* @brief Sets an order in a floor, according to the elev_button_type_t at that
* instant.
*
* @param[in] floor The desired floor
*/
void set_order(int floor);


/**
* @brief Polls all 4 floors of the elevator with set_order() and places orders
* accordingly.
*
*/
void poll_orders();

/**
* @brief Checks if up_orders[floor] is a non-zero value.
*
* @param[in] floor The desired floor
*
* @return 1 if up_orders[floor] != 0. 0 if otherwise.
*/
int get_up_order(int floor);

/**
* @brief Checks if down_orders[floor] is a non-zero value.
*
* @param[in] floor The desired floor
*
* @return 1 if down_orders[floor] != 0. 0 if otherwise.
*/
int get_down_order(int floor);

/**
* @brief Checks if elevator_orders[floor] is a non-zero value.
*
* @param[in] floor The desired floor
*
* @return 1 if elevator_orders[floor] != 0. 0 if otherwise.
*/
int get_elev_order(int floor);

/**
* @brief Checks if order_at_floor[floor] is a non-zero value.
*
* @param[in] floor The desired floor
*
* @return 1 if order_at_floor[floor] != 0. 0 if otherwise.
*/
int get_order_at_floor(int floor);

/**
* @brief Checks if there are any orders above the desired floor.
*
* @param[in] floor The desired floor
*
* @return 1 if there are any orders above "floor" in order_at_floor. 0
* otherwise.
*/
int are_there_orders_above_me(int floor);

/**
* @brief Checks if there are any orders below the desired floor.
*
* @param[in] floor The desired floor
*
* @return 1 if there are any orders below "floor" in order_at_floor. 0
* otherwise.
*/
int are_there_orders_below_me(int floor);

/**
* @brief Checks if there are any unhandled orders left.
*
* @return 1 there are no more orders in order_at_floor. 0 if there are unexpended
* orders.
*/
int out_of_orders();


/**
* @brief Clears all orders at the desired floor.
*
* @param[in] floor The desired floor
*/
void clear_orders_at_floor(int floor);

/**
* @brief Clears all orders at all floors.
*/
void excecute_order_66();
