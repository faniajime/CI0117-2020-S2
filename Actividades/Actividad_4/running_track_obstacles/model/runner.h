#ifndef RUNNER_H
#define RUNNER_H

#include "running_track.h"

typedef struct runner
{
    int id;
    int preparation_time;
    int running_time;
} runner_t;


typedef struct run_data
{
    runner_t* runner;
    int * lane_start;
    int * lane_finish;
    int * position;
    pthread_mutex_t * mutex_position;
    pthread_barrier_t * barrier_start_line;
} run_data_t;


runner_t* runner_create(int id, int preparation_time, int running_time);
void runner_destroy(runner_t* runner);
void* run(void* args);

#endif