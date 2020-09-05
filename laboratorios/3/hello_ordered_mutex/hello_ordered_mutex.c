 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    size_t next_thread;
} shared_message_t;

typedef struct {
    size_t thread_num;
    shared_message_t* shared_message;

} thread_data_t;


void* helloWorld(void* args) {

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_message_t* shared_message = data->shared_message;

    while (thread_num != shared_message->next_thread);

    printf("Hello world from thread number # %zu.\n", thread_num);
    
    ++shared_message->next_thread;
    
    return NULL;
}


int main(int argc, char* arg[]) {

    struct timespec inicio, final; 
    int dif, menor;
    menor = 100000;


    clock_gettime(CLOCK_REALTIME, &inicio);
    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));

    shared_message_t* shared_message = (shared_message_t*)calloc(1, sizeof(shared_message_t));
    shared_message->next_thread = 0;

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_message = shared_message;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }


    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &final);
    dif = (final.tv_nsec - inicio.tv_nsec);
    printf("Hello from the main thread, this solution took %u nanoseconds to run.\n",dif);

    free(threads);
    free(shared_message);
    free(thread_data_list);

    return 0;
} 
