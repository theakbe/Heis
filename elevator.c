#include "elevator.h"
#include "elev.h"
#include "queue.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int last_floor=0;

int get_last_floor(){
	int floor=elev_get_floor_sensor_signal();
	if (floor!=-1){
		last_floor=floor;
	}
	return last_floor;
}

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


//mulig legge check_buttons() inni drive() for å kunne enten stoppe uten å reagere eller drive og reagere på knapper
void drive(){
	int floor=elev_get_floor_sensor_signal();
	if ((floor != -1)&&(should_stop(floor))){
		printf("DRIVE: i want to stop in floor: %d \n", floor);
		remove_from_queue(floor);
		open_door();
	}
	
	printf("%d %d %d \n", get_new_dir(floor), get_last_direction(), get_last_floor());
	elev_set_motor_direction(get_new_dir(get_last_floor()));
}




void floor_light(){
	int floor = elev_get_floor_sensor_signal();
	if(floor!=-1){

		elev_set_floor_indicator(floor);
	}
}


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

