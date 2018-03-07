#include "controlPanel.h"
#include "elev.h"
#include "queue.h"

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
				printf("down pushed\n");
				add_queue_elm(floor,1);
				elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);

			}
		}
		if (elev_get_button_signal(BUTTON_COMMAND, floor)==1){
			printf("command pushed\n");
			add_queue_elm(floor,2);
			elev_set_button_lamp(BUTTON_COMMAND, floor, 1);

		}
	}
}
// nb! kan iterere gjennom enum og lage for løkke for button her, fiks når vi får til resten


void current_floor_light(){
	int floor = elev_get_floor_sensor_signal();
	if(floor!=-1){

		elev_set_floor_indicator(floor);
	}
}
