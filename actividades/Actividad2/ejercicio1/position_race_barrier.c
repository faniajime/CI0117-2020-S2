#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>



typedef struct {
    size_t position;
    pthread_barrier_t barrier; 
    size_t value;
    sem_t semaphore;
} shared_message_t;

typedef struct {
    size_t thread_num;
    shared_message_t* shared_message;
    size_t thread_count;
} thread_data_t;

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds)
{
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}

void* race(void* args) {

    thread_data_t* data = (thread_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_message_t* shared_message = data->shared_message;
    size_t thread_count = data->thread_count;

    random_sleep(1,5);
    printf("Thread %zu/%zu, Im ready!\n", thread_num, thread_count);
    pthread_barrier_wait(&shared_message->barrier);


    sem_wait(&shared_message->semaphore);  
    
    printf("Thread %zu/%zu: I arrived at position %zu\n", thread_num, thread_count, shared_message->position);
    shared_message->position = shared_message->position + 1;

    sem_post(&shared_message->semaphore);

    return NULL;
}


int main(int argc, char* arg[]) {

    srand(time(NULL));
    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);

    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    shared_message_t* shared_message = (shared_message_t*)calloc(1, sizeof(shared_message_t));
    shared_message->position = 0;
    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));
    shared_message->value = thread_count;

    pthread_barrier_init(&shared_message->barrier, NULL, shared_message->value);
    sem_init(&shared_message->semaphore, 1, 1);

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].thread_count = thread_count;
        thread_data_list[i].shared_message = shared_message;
        pthread_create(&threads[i], NULL, race, (void*)&thread_data_list[i]);
    }

    sem_destroy(&shared_message->semaphore);

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }



    free(threads);
    free(shared_message);
    free(thread_data_list);

    return 0;
} 
