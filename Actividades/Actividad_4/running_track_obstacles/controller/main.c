#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../model/main.h"
#include "../model/runner.h"

int start_race() {

	srand( time(NULL) );

    running_track = running_track_create();
    
    run_data_t* run_data_list = malloc(NUM_LANES * sizeof(run_data_t));
    
    pthread_t* runner_threads = malloc(NUM_LANES * sizeof(pthread_t));

    for (int i = 0; i < NUM_LANES; ++i) {
        run_data_list[i].barrier_start_line = &running_track->barrier_start_line;
        run_data_list[i].mutex_position = &running_track->mutex_position;
        run_data_list[i].runner = runner_create(i, 1000 + (rand() % 4000), 5000 + (rand() % 11000));
        run_data_list[i].lane_start = &running_track->lanes_start_line[i];
        run_data_list[i].lane_finish = &running_track->lanes_finish_line[i];
        run_data_list[i].position = &running_track->position;

        pthread_create(&runner_threads[i], NULL, run, (void*)&run_data_list[i]);
    }

    for (int i = 0; i < NUM_LANES; ++i) {
        pthread_join(runner_threads[i], NULL);
    }

    for (int i = 0; i < NUM_LANES; ++i) {
        printf("lane %d: %d\n", i, running_track->lanes_finish_line[i]); 
    }

    running_track_destroy(running_track);
    for (int i = 0 ; i < NUM_LANES; ++i) {
        runner_destroy(run_data_list[i].runner);
    }

    free(run_data_list);
    free(runner_threads);

    running_track = 0;

    return 0;
}