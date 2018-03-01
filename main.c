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
        drive();
        check_buttons();
        floor_light();
        stop();
        
    }
    
    return 0;
}
