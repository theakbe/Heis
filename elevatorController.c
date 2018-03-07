#include "elevatorController.h"
#include "elev.h"
#include "queue.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "controlPanel.h"


static int last_floor=0;

void initialize() {
    initialize_queue();
    while (1) {
        if (elev_get_floor_sensor_signal() == -1) {
            elev_set_motor_direction(DIRN_DOWN);
            if (elev_get_floor_sensor_signal() != -1) {
                elev_set_motor_direction(DIRN_STOP);
                break;
            }
        } else {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }
}

int get_last_floor(){
	int floor=elev_get_floor_sensor_signal();
	if (floor!=-1){
		last_floor=floor;
	}
	return last_floor;
}


void open_door() {
    time_t startTime = time(NULL); // return current time in seconds
    while (time(NULL) - startTime < 3) {
        check_buttons();
        elev_set_motor_direction(DIRN_STOP);
        elev_set_door_open_lamp(1);
    }
    elev_set_door_open_lamp(0);
}

//mulig legge check_buttons() inni drive() for å kunne enten stoppe uten å reagere eller drive og reagere på knapper
void drive(){
	int floor = get_last_floor();
	int lastDIr = get_last_direction();
	int newDir = get_new_dir(floor);
	if ((floor != -1)&&(should_stop(floor))){
		printf("DRIVE: i want to stop in floor: %d \n", floor);
		remove_from_queue(floor);
		open_door();
	}
//	printf("new dir: %d last dir: %d last floor%d \n", newDir, lastDIr, get_last_floor());
	if (lastDIr != newDir){
		printf("diffenret new %d old %d\n",newDir, lastDIr);
	}
	elev_set_motor_direction(get_new_dir(get_last_floor()));
}


void emergancy_stop(){
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
