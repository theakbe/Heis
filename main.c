#include "elev.h"
#include <stdio.h>
#include "elevatorController.h"
#include "queue.h"
#include "controlPanel.h"



int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    //Go to initial position
    initialize();
    while(1){
        drive();
        check_buttons();
        current_floor_light();
        emergency_stop();
    }
    return 0;
}
