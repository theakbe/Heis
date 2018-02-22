#include "elevator.h"
#include "elev.h"
#include "queue.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void add_to_queue(){
		for (int i = 0; i < sizeof(button_channel_matrix); ++i)
		{
			for (int j = 0; j < sizeof(button_channel_matrix[0]); ++j)
			{
				if (button_channel_matrix[i][j]==1){
					add_queue_elm(i, j)
					if(j==0){
						elev_set_button_lamp(BUTTON_CALL_UP, i, 1)
					}
					else if (j==1){
						elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1)
					}
					else if (j==2){
						elev_set_button_lamp(BUTTON_COMMAND, i, 1)
					}
				}
			}
		}
} // kanskje while løkke rundt hele funksjonen


void open_door(){
	int sec = 0, trigger = 3000;
	clock_t before = clock();
do {
  elev_set_motor_direction(DIRN_STOP);
	elev_set_door_open_lamp(1);
  clock_t difference = clock() - before;
  sec = difference * 1000 / CLOCKS_PER_SEC;
  iterations++;
} while ( sec < trigger );
	elev_set_door_open_lamp(0);
}

void drive(){
	while (1) {
		int dir=0;
		for(int i=0; i< sizeof(queue);++i){
			for (int j=0; j< sizeof(queue[i]);++i){
				if(queue[i][j]==1){
					if(j=0){
						  elev_set_motor_direction(DIRN_UP);
							dir=0;
							queue[i][j]=0;
							break;
					}
					if(j=1){
							elev_set_motor_direction(DIRN_DOWN);
							dir=1;
							queue[i][j]=0;
							break;
					}
				}
			}
		}
	}
	for(int i=0; i< sizeof(queue);++i){
		if ((elev_get_floor_sensor_signal() == i) && (queue[i][2]==1)){
			open_door();
			elev_set_button_lamp(BUTTON_COMMAND, i, 0);
			queue[i][2]=0;
		}
		else if ((elev_get_floor_sensor_signal() == i) && (queue[i][dir]==1)){
			open_door();
		}
		if ((elev_get_floor_sensor_signal() == i) && (queue[i][dir]==1)){
			queue[i][dir]=0;
			if (dir==0){
				elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
			}
			else if (dir==1){
				elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
			}
		}
	}
}

void floor_light(){
		elev_set_floor_indicator(elev_get_floor_sensor_signal())
}
// sjekk verdier på lampe og floor

void initialize(){
	initialize_queue();
	while (1) {
			if (elev_get_floor_sensor_signal() ==-1) {
					elev_set_motor_direction(DIRN_DOWN);
					if (elev_get_floor_sensor_signal() !=-1){
						elev_set_motor_direction(DIRN_STOP);
						break;
					}
			}
			else{
					elev_set_motor_direction(DIRN_STOP);
					break;
			}

	}
}

void stop(){
	while(elev_get_stop_signal()==1){
		elev_set_motor_direction(DIRN_STOP);
		if (elev_get_floor_sensor_signal() !=-1){
			elev_set_door_open_lamp(1);
		}
		elev_set_stop_lamp(1);
		initialize_queue();
	}
	if (elev_get_floor_sensor_signal() !=-1){
		open_door();
	}
}
