 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

typedef struct {
    size_t position;
    size_t num_threads;
    sem_t* semaphores;

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

    sem_wait(&shared_data->semaphores[thread_num]);
    printf("Im the thread %zu and I arrived %zu\n", thread_num, shared_data->position);
    shared_data->position = shared_data->position+1;
    sem_post(&shared_data->semaphores[shared_data->position]);
    sem_post(&shared_data->semaphores[thread_num]);
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

    struct timespec inicio, final; 
    int dif;
    clock_gettime(CLOCK_REALTIME, &inicio);

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));

    shared_data->semaphores = (sem_t*)calloc(thread_count, sizeof(sem_t));

    shared_data->num_threads = thread_count;
    shared_data->position = 0;

    for (size_t i = 0; i < thread_count; ++i) {
        sem_init(&shared_data->semaphores[i], 1 , 1);
    }
    
    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i = 1; i < shared_data->num_threads; ++i) {
        sem_wait(&shared_data->semaphores[i]);
    }


    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }
    
    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        sem_destroy(&shared_data->semaphores[i]);
    }
    clock_gettime(CLOCK_REALTIME, &final);
    dif = (final.tv_nsec - inicio.tv_nsec);
    printf("This solution took %u nanoseconds to run \n", dif);

    free(threads);
    free(shared_data->semaphores);
    free(shared_data);

    return 0;
}