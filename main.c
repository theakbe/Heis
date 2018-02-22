#include "elev.h"
#include <stdio.h>
#include "elevator.h"
#include "queue.h"



int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    //Go to initial position
    initialize();

    while(1){
        // sjekk knapper
            // evt. gjør noe
        
        // sjekk floor
            // sejkke om ny motor dir

        // sjekke Emergency

        // evt mer?
        //floor_light();
        check_buttons();
        check_queue();
        check_floor();
        drive();
    }
    /*
    while(1){
      if(elev_get_stop_signal()==1){
        stop();
        continue;
      }
      else{
        while(1){
          drive();
          floor_light();
          add_to_queue();
        }
      }
    }
*/
    //queue? lage dobbel array, med for hver etasje: opp, ned, inni heis, "clean" ifølge studass




    //elev_set_motor_direction(DIRN_UP);

/*
    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }*/

    return 0;
}
