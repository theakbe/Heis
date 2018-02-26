#include "elev.h"

void add_queue_elm(int floor, int dir);
void initialize_queue();
void check_queue();
int get_queue(int floor, elev_motor_direction_t dir);
void remove_from_queue(int floor, int dir);
int get_dir();