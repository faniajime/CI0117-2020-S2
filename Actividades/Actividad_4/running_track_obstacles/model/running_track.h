#ifndef RUNNING_TRACK_H
#define RUNNING_TRACK_H

#define NUM_LANES 8

#include <pthread.h>

typedef struct running_track
{
    int* lanes_start_line;
    int* lanes_finish_line;

    int position;
    pthread_mutex_t mutex_position;
    pthread_barrier_t barrier_start_line;

} running_track_t;

running_track_t* running_track_create();
void running_track_destroy(running_track_t* running_track);

#endif