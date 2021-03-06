
#include "queue.h"
#include "elevatorController.h"
#include "elev.h"
#include <assert.h>
#include <stdlib.h>

typedef int bool;
enum {false, true};

int queue[N_FLOORS][N_BUTTONS] = {{0}};
static elev_motor_direction_t last_direction=DIRN_STOP;

void initialize_queue(){
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			queue[i][j]=0;
		}
	}
}

void add_queue_elm(int floor, int dir){
	queue[floor][dir]=1;
}


int check_queue_empty(){
	int number_of_orders=0;
	for (int floor=0; floor<N_FLOORS;floor++){
		for (elev_button_type_t button = BUTTON_CALL_UP; button<=BUTTON_COMMAND;button++){
			if (queue[floor][button]==1){
					number_of_orders++;
			}
		}
	}
	return number_of_orders;
}


bool check_orders_above(int current_floor){
	for (int floor=current_floor;floor<N_FLOORS;floor++){
			if ((queue[floor][BUTTON_CALL_UP]==1)||(queue[floor][BUTTON_CALL_DOWN]==1)||(queue[floor][BUTTON_COMMAND]==1)){
				return true;

			}

	}
	return false;
}

bool check_orders_below(int current_floor){
	for (int floor=0;floor<=current_floor;floor++){
			if ((queue[floor][BUTTON_CALL_UP]==1)||(queue[floor][BUTTON_CALL_DOWN]==1)||(queue[floor][BUTTON_COMMAND]==1)){
				return true;
			}

	}
	return false;
}

elev_motor_direction_t order_placement(int current_floor, int order_floor){ //sjekker om heis skal gå opp eller ned for å komme til ordre
	if (current_floor > order_floor) {
			last_direction=DIRN_DOWN;
			return  DIRN_DOWN;
		}
	else if(current_floor < order_floor){
			last_direction=DIRN_UP;
			return DIRN_UP;
		}
	return DIRN_STOP;
}


elev_motor_direction_t get_new_dir(int current_floor){
	if((get_emergency_stop_pushed()!=0) && (check_if_order_floor(current_floor))){
		if(last_direction==DIRN_DOWN){
			return DIRN_UP;
		}
		if(last_direction==DIRN_UP){
			return DIRN_DOWN;
		}
	}
	
	if((check_queue_empty()!=0)&&(current_floor==N_FLOORS-1)){
		last_direction = DIRN_DOWN;
		return DIRN_DOWN;
	}
	if((check_queue_empty()!=0)&&(current_floor==0)){
		last_direction = DIRN_UP;
		return DIRN_UP;
	}


	if (last_direction == DIRN_UP){
		if (check_orders_above(current_floor)){
			return DIRN_UP;
		}
	}

	if (last_direction == DIRN_DOWN){
		if (check_orders_below(current_floor)){
			return DIRN_DOWN;
		}
	}
	for (int floor=0; floor<N_FLOORS;floor++){
		if (queue[floor][BUTTON_CALL_UP]==1){
			order_placement(current_floor, floor);
		}
		if (queue[floor][BUTTON_CALL_DOWN]==1){
			order_placement(current_floor, floor);
		}

		if (queue[floor][BUTTON_COMMAND]==1){
			order_placement(current_floor, floor);
		}
	}
	return DIRN_STOP;
}



bool should_stop(int floor){
	if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_COMMAND] ==1)){
		return true;
	}
	
	if (check_queue_empty()==1){
		if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_CALL_DOWN] ==1)) {
			return true;
		}
		else if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_CALL_UP] ==1)) {
			return true;
		}
	}

	else if (last_direction==DIRN_UP){
		if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_CALL_UP] ==1)) {
			return true;
		}
		if((elev_get_floor_sensor_signal() == floor) && (!check_orders_above(floor+1)) && ((queue[floor][BUTTON_CALL_UP] ==1)||(queue[floor][BUTTON_CALL_DOWN] ==1))){
			return true;
		}
	}

	else if (last_direction==DIRN_DOWN){
		if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_CALL_DOWN] ==1)) {
			return true;
		}
		if((elev_get_floor_sensor_signal() == floor) && (!check_orders_below(floor-1)) && ((queue[floor][BUTTON_CALL_UP] ==1)||(queue[floor][BUTTON_CALL_DOWN] ==1))){
			return true;
		}
	}
	return false;
}



void remove_from_queue(int floor){
	if (!(floor == N_FLOORS-1) && !(floor == 0)){
		for(elev_button_type_t button= BUTTON_CALL_UP;button<=BUTTON_COMMAND;button++){
			queue[floor][button] = 0;
			elev_set_button_lamp(button, floor, 0);
			}
		}
	if (floor == N_FLOORS-1){
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
		elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
		queue[floor][BUTTON_CALL_DOWN] = 0;
		queue[floor][BUTTON_COMMAND] = 0;
	}
	if (floor == 0){
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
		elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
		queue[floor][BUTTON_CALL_UP] = 0;
		queue[floor][BUTTON_COMMAND] = 0;
	}
}


bool check_if_order_floor(int floor){
	for(elev_button_type_t button=BUTTON_CALL_UP;button<=BUTTON_COMMAND;button++){
		if(queue[floor][button]==1){
			return true;
		}
	}
	return false;
}



elev_motor_direction_t get_last_dir(){
	return last_direction;
}
