#ifndef RUNNER_H
#define RUNNER_H
#define OBSTACLES 5

#include <pthread.h>

typedef struct runner
{
    int id;
    int preparation_time;
    int running_time;
    int obstacle_time;
} runner_t;

typedef struct run_data
{
    runner_t* runner;
    int *position;

    int * lane_start;
    int * lane_finish;
    int * * matrix_obstacles;

    pthread_mutex_t * mutex_position;
    pthread_barrier_t * barrier_start_line;

} run_data_t;


runner_t* runner_create(int id, int preparation_time, int running_time);
void runner_destroy(runner_t* runner);

void* run(void* args);

#endif