
#include "queue.h"
#include "elevator.h"
#include "elev.h"
#include <assert.h>
#include <stdlib.h>


int queue[4][3] = {0};
int dir = -1;

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



elev_motor_direction_t getNewDir(int currentFloor, elev_motor_direction_t lastDirection){
	if (lastDirection == DIRN_UP){
		for (int floor=currentFloor;floor<N_FLOORS;floor++){
			if ((queue[floor][BUTTON_CALL_UP]==1)||(queue[floor][BUTTON_CALL_DOWN]==1)){
				return  DIRN_UP;
			}

		}
		//sjekk om ordre over?
	}

	if (lastDirection == DIRN_DOWN){
		for (int floor=0;floor<currentFloor;floor++){
			if ((queue[floor][BUTTON_CALL_UP]==1)||(queue[floor][BUTTON_CALL_DOWN]==1)){
				return  DIRN_DOWN;
			}

		}
	}
	for (int floor=0; floor<N_FLOORS;floor++){
		if (queue[floor][BUTTON_CALL_UP]==1){
				return  DIRN_UP;
		}
		if (queue[floor][BUTTON_CALL_DOWN]==1){
				return  DIRN_DOWN;
		}
// sjekker ordre under
	}
	//sjekk denne funksjonen

	//sjekk om ordre over

	// sjekk om ordre under

	return DIRN_STOP;
}

bool shouldStop(int floor){
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

int get_queue(int floor, int dir){
	return queue[floor][dir];
}

void remove_from_queue(int floor, int dir){
	queue[floor][dir] = 0;
}

int get_dir(){
	return dir;

}


// nb! kan iterere gjennom enum og lage for løkke for button her
