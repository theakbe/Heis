#include "elev.h"
typedef int bool;
void add_queue_elm(int floor, int dir);
void initialize_queue();
bool queue_empty();
elev_motor_direction_t get_new_dir(int currentFloor);
bool should_stop(int floor);
void remove_from_queue(int floor);
elev_motor_direction_t get_last_direction();
void reset_last_direction();