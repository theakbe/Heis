#include "elev.h"
typedef int bool;
void add_queue_elm(int floor, int dir);
void initialize_queue();
elev_motor_direction_t get_new_dir(int currentFloor);
bool should_stop(int floor);
int get_queue(int floor, elev_motor_direction_t dir);
void remove_from_queue(int floor);
bool queue_empty();
elev_motor_direction_t get_last_direction();
void reset_last_direction();