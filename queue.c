
#include "queue.h"
#include "elevator.h"
#include "elev.h"
#include <assert.h>
#include <stdlib.h>

typedef int bool;
enum {false, true};

int queue[N_FLOORS][N_BUTTONS] = {0};
int dir = -1;
static elev_motor_direction_t lastDirection=DIRN_STOP;

void add_queue_elm(int floor, int dir){
	queue[floor][dir]=1;
}

void initialize_queue(){
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			queue[i][j]=0;
		}
	}
}

//returnerer true hvis køen er tom
bool queue_empty(){
	for (int floor=0; floor<N_FLOORS;floor++){
		if (queue[floor][BUTTON_CALL_UP]==1){
				return  false;
		}
		if (queue[floor][BUTTON_CALL_DOWN]==1){
				return  false;
		}
		if (queue[floor][BUTTON_COMMAND]==1){
				return  false;
		}
// legg til for-løkke istedenfor if her
	}
	return true;
}


//finne ut om prioriteringen, skal den stoppe i 3. for ned hvis den skal opp til 4. først for ned?
//fikse på torsdag
elev_motor_direction_t get_new_dir(int currentFloor){
	if (currentFloor==-1){	//problem med stop(), lastDirection blir alltid stop
		return lastDirection;
	}
	if((!queue_empty())&&(currentFloor==N_FLOORS-1)){
		lastDirection = DIRN_DOWN;
		return DIRN_DOWN;
	}
	if((!queue_empty())&&(currentFloor==0)){
		lastDirection = DIRN_UP;
		return DIRN_UP;
	}
	if (lastDirection == DIRN_UP){
		for (int floor=currentFloor;floor<N_FLOORS;floor++){ //floor = 3
			if ((queue[floor][BUTTON_CALL_UP]==1)||(queue[floor][BUTTON_CALL_DOWN]==1)||(queue[floor][BUTTON_COMMAND]==1)){
				return DIRN_UP;
			
			}

		}
		//sjekk om ordre over?
	}

	if (lastDirection == DIRN_DOWN){
		for (int floor=0;floor<currentFloor;floor++){
			if ((queue[floor][BUTTON_CALL_UP]==1)||(queue[floor][BUTTON_CALL_DOWN]==1)||(queue[floor][BUTTON_COMMAND]==1)){
				return DIRN_DOWN;
			}

		}
	}
	for (int floor=0; floor<N_FLOORS;floor++){
		if (queue[floor][BUTTON_CALL_UP]==1){
			if (currentFloor > floor) {
				lastDirection=DIRN_DOWN;
				return  DIRN_DOWN;
			}
			else {
				lastDirection=DIRN_UP;
				return DIRN_UP;
			}
			
		}
		if (queue[floor][BUTTON_CALL_DOWN]==1){
			if (currentFloor > floor) {
				lastDirection=DIRN_DOWN;
				return  DIRN_DOWN;
			}
			else {
				lastDirection=DIRN_UP;
				return DIRN_UP;
			}
		}

		if (queue[floor][BUTTON_COMMAND]==1){
			if (currentFloor > floor) {
				lastDirection=DIRN_DOWN;
				return  DIRN_DOWN;
			}
			else {
				lastDirection=DIRN_UP;
				return DIRN_UP;
			}
		}

// sjekker ordre under
	}
	//sjekk denne funksjonen

	//sjekk om ordre over

	// sjekk om ordre under

	return DIRN_STOP;
}

bool should_stop(int floor){
	if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_CALL_UP] ==1)) {
			return true;
	}
	else if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_CALL_DOWN] ==1)){
			return true;
	}
	else if ((elev_get_floor_sensor_signal() == floor) && (queue[floor][BUTTON_COMMAND] ==1)){
			return true;
	}
	return false;
}

/*
void check_queue(){
	for(int floor=0; floor < N_FLOORS; floor++){
			for (int button=0; button< 3;button++){
				if(queue[floor][button]==1){
					// har bestilling...
					if(button==0){
							dir=0;
							//queue[floor][button]=0;
					}
					if(button==1){
							dir=1;
							//queue[floor][button]=0;
					}
				}
			}
		}
}
*/

int get_queue(int floor, elev_motor_direction_t dir){
	return queue[floor][dir];
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


/*if (floor!=N_FLOORS-1){ //hvis vi ikke er i 4 etasje kan vi trykke på oppo-knapp
			if (elev_get_button_signal(BUTTON_CALL_UP, floor)==1){
				printf("up pushed\n");
				add_queue_elm(floor,BUTTON_CALL_UP);
				elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
			}
		}
		if(floor!=0){ //hvis vi ikke er i første etasje kan vi trykke på nedknapp
			if (elev_get_button_signal(BUTTON_CALL_DOWN, floor)==1){
				printf("up pushed\n");
				add_queue_elm(floor,1);
				elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
			}
		}
		if (elev_get_button_signal(BUTTON_COMMAND, floor)==1){
			printf("up pushed\n");
			add_queue_elm(floor,2);
			elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
		}
*/


elev_motor_direction_t get_last_direction(){
	return lastDirection;
}

void reset_last_direction(){
	lastDirection = DIRN_STOP;
}


// nb! kan iterere gjennom enum og lage for løkke for button her
