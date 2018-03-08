#include "elevatorController.h"
#include "elev.h"
#include "queue.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "controlPanel.h"


static int last_floor=0;
static int emergency_stop_pushed=0;


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
	printf("FLOOR %d\n", floor );
	int last_dir = get_last_direction();
	elev_motor_direction_t new_dir = get_new_dir(floor);
	printf("NEWDIR %d\n", new_dir );
	printf("STOP %d , ORDER %d\n",emergency_stop_pushed,check_if_order_floor(floor) );
	if ((emergency_stop_pushed!=0)&& (check_if_order_floor(floor))){
		new_dir=DIRN_UP;
		
	}
	if ((emergency_stop_pushed==1)&&(new_dir!=DIRN_STOP)){
		emergency_stop_pushed=0;

	}
	if ((floor != -1)&&(should_stop(floor))){
		printf("STOP %d \n", floor);
		remove_from_queue(floor);
		open_door();
	}
	
	

	elev_set_motor_direction(new_dir);
	
}


void emergency_stop(){
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
		emergency_stop_pushed=1;

	}
}
