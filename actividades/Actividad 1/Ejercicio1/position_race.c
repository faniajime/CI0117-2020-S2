#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t message;
} shared_message_t;

typedef struct {
    size_t thread_num;
    shared_message_t* shared_message;
    size_t thread_count;
    pthread_mutex_t mutex_example;

} thread_data_t;


void* helloWorld(void* args) {

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_message_t* shared_message = data->shared_message;
    size_t thread_count = data->thread_count;
    pthread_mutex_t mutex_example = data->mutex_example;

    pthread_mutex_lock(&mutex_example);
   
    printf("Thread %zu/%zu: I arrived at position %zu\n", thread_num, thread_count, shared_message->message);
    
    ++shared_message->message;

    pthread_mutex_unlock(&mutex_example);
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

    shared_message_t* shared_message = (shared_message_t*)calloc(1, sizeof(shared_message_t));

    shared_message->message = 0;

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    pthread_mutex_t mutex_example;
    pthread_mutex_init(&mutex_example, NULL);

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].thread_count = thread_count;
        thread_data_list[i].shared_message = shared_message;
        thread_data_list[i].mutex_example = mutex_example;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }

    pthread_mutex_destroy(&mutex_example);

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }


    free(threads);
    free(shared_message);
    free(thread_data_list);

    return 0;
} 
