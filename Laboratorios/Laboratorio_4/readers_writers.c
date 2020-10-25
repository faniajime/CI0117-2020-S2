#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

// 2 hilos: Productor y Consumidor
// Buffer de numeros, tamanio como parametro

typedef struct 
{
    size_t quantity;
    pthread_mutex_t mutex;
    sem_t writer_semaphore;
    size_t readers_counter;
    size_t writers_counter;   
} shared_data_t;

typedef struct {

    size_t thread_num;
    shared_data_t* shared_data;

} thread_data_t;

void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds)
{
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}

// Escritor
void* writet(void* args) {
    random_sleep(500, 1000);

    thread_data_t* data = (thread_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;  
    
    sem_wait(&shared_data->writer_semaphore);
    shared_data->quantity++;
    printf("I added 1, quantity is: %zu \n", shared_data->quantity);
    sem_post(&shared_data->writer_semaphore);
    return NULL;
}

// lector
void* readt(void* args) {
    random_sleep(500, 1000);

    thread_data_t* data = (thread_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;
    pthread_mutex_lock(&shared_data->mutex); 
    
    shared_data->readers_counter++;
    if(shared_data->readers_counter >= 1)
    {
        sem_wait(&shared_data->writer_semaphore);
    }
    
    printf("Reading quantity: %zu \n", shared_data->quantity);

    pthread_mutex_unlock(&shared_data->mutex);
    pthread_mutex_lock(&shared_data->mutex);
    shared_data->readers_counter--;
    if(shared_data->readers_counter==0)
    {
        sem_post(&shared_data->writer_semaphore);
    }
    pthread_mutex_unlock(&shared_data->mutex);   
    return NULL;
}

int main(int argc, char* arg[]) {

    srand( time(NULL) );
    size_t readers_num;
    size_t writers_num;
    size_t numthreads;

    if (argc >= 3) {
        writers_num = (size_t)strtoul(arg[1], NULL, 10);
        readers_num = (size_t)strtoul(arg[2], NULL, 10);
    } else {
        printf("Invalid number of params\n");
        return 1;
    }
    
    numthreads = readers_num + writers_num;
    pthread_t* threads = malloc((size_t)(numthreads * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)malloc(sizeof(shared_data_t));
    thread_data_t* thread_data_list = malloc((size_t)(numthreads * sizeof(thread_data_t)));

    shared_data->quantity = 0;
    shared_data->readers_counter = 0;

    sem_init(&shared_data->writer_semaphore, 0, 1);
    pthread_mutex_init(&shared_data->mutex, NULL);

// Thread creation
    
    for (size_t i = 0; i < writers_num; ++i) {
        thread_data_list[i].thread_num= i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, writet, (void*)&thread_data_list[i]);
    }

    for (size_t i = writers_num; i < numthreads; ++i) {
        thread_data_list[i].thread_num= i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, readt, (void*)&thread_data_list[i]);
    }

    //random_sleep(2000, 3000);

    for (size_t i = 0; i < numthreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&shared_data->writer_semaphore);
    pthread_mutex_destroy(&shared_data->mutex);

    free(threads);
    free(shared_data);
    free(thread_data_list);

}
