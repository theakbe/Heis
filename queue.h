#include "elev.h"
typedef int bool;
void add_queue_elm(int floor, int dir);
void initialize_queue();
bool queue_empty();
bool check_orders_above(int current_floor);
bool check_orders_below(int current_floor);
elev_motor_direction_t order_placement(int current_floor, int floor);
elev_motor_direction_t get_new_dir(int currentFloor);
bool should_stop(int floor);
void remove_from_queue(int floor);
elev_motor_direction_t get_last_direction();
void reset_last_direction();