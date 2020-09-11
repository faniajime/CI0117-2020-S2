#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t position;
    size_t num_threads;
    pthread_mutex_t* mutexes;

} shared_data_t;

typedef struct {
    
    size_t thread_num;
    shared_data_t* shared_data;

} thread_data_t;


void* helloWorld(void* args) {

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;
    size_t num_threads = shared_data->num_threads;


    if(thread_num == 0){
        for (size_t i = 1; i < num_threads; ++i) {
            pthread_mutex_lock(&shared_data->mutexes[i]);
        }
    }
    printf("Im the thread %zu and I arrived %zu\n", thread_num, shared_data->position);
    ++shared_data->position;
    pthread_mutex_unlock(&shared_data->mutexes[shared_data->position]);
      
    return NULL;
}

int main(int argc, char* arg[]) {

    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);

    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));

    shared_data->mutexes = (pthread_mutex_t*)calloc(thread_count, sizeof(pthread_mutex_t));

    shared_data->num_threads = thread_count;
    shared_data->position = 0;

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_mutex_init(&shared_data->mutexes[i], /*attr*/ NULL);
    }
    
    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_mutex_destroy(&shared_data->mutexes[i]);
    }
    
    free(threads);
    free(shared_data->mutexes);
    free(shared_data);
    free(thread_data_list);

    return 0;
}
