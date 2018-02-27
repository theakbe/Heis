#include "elevator.h"
#include "elev.h"
#include "queue.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//elev_motor_direction_t lastDirection= DIRN_STOP;

/*
void open_door(){
	int sec = 0, trigger = 3;
	clock_t before = clock();
do {
  elev_set_motor_direction(DIRN_STOP);
  elev_set_door_open_lamp(1);
  clock_t difference = clock() - before;
  sec = difference * 1 / CLOCKS_PER_SEC;
} while ( sec < trigger );
	elev_set_door_open_lamp(0);
}


*/
void open_door(){
	time_t startTime = time(NULL); // return current time in seconds
	while (time(NULL) - startTime < 3)
	{
		check_buttons();
    	elev_set_motor_direction(DIRN_STOP);
  		elev_set_door_open_lamp(1);
	}
  	elev_set_door_open_lamp(0);
	}

/*
void open_door(){
	time_t start, end;
	double elapsed;
	time(&start);

	do {
		time(&end);
		elapsed = difftime(end, start);

		elev_set_motor_direction(DIRN_STOP);
  		elev_set_door_open_lamp(1);

	} while(elapsed < 3);
	elev_set_door_open_lamp(0);
}
*/

void drive(){
	int floor=elev_get_floor_sensor_signal();
	if ((floor != -1)&&(should_stop(floor))){
		/*
		for (elev_button_type_t dir = BUTTON_CALL_UP; dir<=BUTTON_COMMAND;dir++){
			if (get_queue(floor,dir)==1){
				lastDirection=dir;
			}
		}*/
		remove_from_queue(floor);
		open_door();
	}
	
	printf("%d %d\n", get_new_dir(floor), get_last_direction());

	elev_set_motor_direction(get_new_dir(floor)); //usikker på denne
}
/*
	for(int floor=0; floor< N_FLOORS;floor++){

		if ((elev_get_floor_sensor_signal() == floor) && (get_queue(floor,2) ==1)) {

			elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
			remove_from_queue(floor, 2);
			open_door();

		}
		else if ((elev_get_floor_sensor_signal() == floor) && (get_queue(floor,get_dir())==1)){
			open_door();
		}
		if ((elev_get_floor_sensor_signal() == floor) && (get_queue(floor,get_dir())==1)){
			remove_from_queue(floor, get_dir());


			if ((get_dir()==0) && (floor !=N_FLOORS-1)){
				elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
			}
			else if ((get_dir()==1) && (floor != 0)){
				elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
			}
		}
	}*/





void floor_light(){
	int floor = elev_get_floor_sensor_signal();
	if(floor!=-1){

		elev_set_floor_indicator(floor);
	}
}
// spør studass om elev_set_floor_indicator funksjonen!!!!

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
	if(elev_get_stop_signal()==1){
		reset_last_direction();
		while(elev_get_stop_signal()==1){
			elev_set_motor_direction(DIRN_STOP);
			if (elev_get_floor_sensor_signal() !=-1){
				elev_set_door_open_lamp(1);
			}
			elev_set_stop_lamp(1);
			for(int floor = 0; floor < N_FLOORS; floor++){
				remove_from_queue(floor);
			}

			
		}
		elev_set_stop_lamp(0);
	
		if (elev_get_floor_sensor_signal() !=-1){
			open_door();
		}
	}
}

void check_buttons(){ //sjekker gjennom, og legger bestillinger til kø
	for (int floor=0;floor<N_FLOORS;floor++){
		if (floor!=N_FLOORS-1){ //hvis vi ikke er i 4 etasje kan vi trykke på oppo-knapp
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
	}
}
// nb! kan iterere gjennom enum og lage for løkke for button her, fiks når vi får til resten



/*
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
*/



/*void change_direction(){
	if (elev_get_floor_sensor_signal() == N_FLOORS-1){
		elev_set_motor_direction(DIRN_DOWN);
	}
	if (elev_get_floor_sensor_signal() == 0){
		elev_set_motor_direction(DIRN_UP);
	}
}
*/

/*void drive(){
	int dir=0;
	while (1) {
		for(int floor=0; floor< N_FLOORS;++floor){
			for (int button=0; button< 3;++button){
				if(queue[floor][button]==1){
					if(button==0){
						  elev_set_motor_direction(DIRN_UP);
							dir=0;
							queue[floor][button]=0;
							break;
					}
					if(button==1){
							elev_set_motor_direction(DIRN_DOWN);
							dir=1;
							queue[floor][button]=0;
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
}*/
