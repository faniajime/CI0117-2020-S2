#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../model/runner.h"

runner_t*  runner_create(int id, int preparation_time, int running_time) {

    runner_t* runner = calloc(1, sizeof(runner_t));

    runner->id = id;
    runner->preparation_time = preparation_time;
    runner->running_time - running_time;
    runner->obstacle_time = running_time/OBSTACLES;

    return runner;
}

void runner_destroy(runner_t* runner) {
    free(runner);
}

void fixed_sleep(useconds_t duration)
{
	usleep( 1000 * duration );
}

void* run(void* args) {

    // runner
    // position
    // lane_start
    // lane_finish
    // mutexposition 
    // barrier

    run_data_t* data = (run_data_t*) args;

    fixed_sleep(data->runner->preparation_time);
    *data->lane_start = 1;
    printf("Thread %d: Ready!\n", data->runner->id);
    pthread_barrier_wait(data->barrier_start_line);

    printf("Thread %d: Go!!!\n", data->runner->id);
    //Running!!!!

    int obstacle_num = 0;
    while(obstacle_num<OBSTACLES)
    {

        fixed_sleep((useconds_t)data->runner->obstacle_time);
        pthread_mutex_lock(&data->mutex_position);
        data->matrix_obstacles[data->runner->id][obstacle_num] = 1;
        data->matrix_obstacles[data->runner->id][obstacle_num-1] = 0;
        pthread_mutex_unlock(&data->mutex_position);
        obstacle_num++;        
    }
    


    pthread_mutex_lock(data->mutex_position);

    *data->lane_finish = *data->position;
    printf("Thread %d: Arrived at position %d\n", data->runner->id, *data->position);
    *data->position = *data->position + 1;

    pthread_mutex_unlock(data->mutex_position);

}