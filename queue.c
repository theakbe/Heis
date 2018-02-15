
#include <queue.h>
#include <assert.h>
#include <stdlib.h>

int queue[4][3];


void add_queue_elm(int floor, int dir){
	queue[floor-1][dir]=1;
}

void initialize_queue(){
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			queue[i][j]=0;
		}
	}
}