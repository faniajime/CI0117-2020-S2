#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>


typedef struct timespec walltime_t;

void walltime_start(walltime_t* start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);

	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;

	return elapsed;
}

typedef struct {

    double area;
    int rectangles_thread;
    pthread_mutex_t mutex_area;
    double delta;
    size_t n;
    double a;
    double b;
    size_t thread_count;
    pthread_barrier_t barrier;

} shared_data_t;

typedef struct {
    
    size_t thread_num;
    shared_data_t* shared_data;
    double a;
    double b;
    double n;

} thread_data_t;

double funcion(double x)
{
    double resultado = 0;
    resultado = (x*x)+1;
    //printf("resultado= %f\n",resultado);
    return resultado;
}

double riemann(double a, double b, double n)
{
    double area = 0;
    double delta = (b-a)/n;
    double x = a;
    
    while(x<=b-delta)
    {
        area += funcion(x)*delta;        
        x+=delta;           
        
    }
    return area;
}

void* distribute(void* args ){
    thread_data_t* data = (thread_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;
    data->n += shared_data->rectangles_thread;


    data->a = shared_data->a + thread_num*data->n*shared_data->delta;
    data->b = data->a + data->n*shared_data->delta;


    double area = riemann(data->a,data->b,data->n);

    pthread_mutex_lock(&shared_data->mutex_area);
    shared_data->area += area;
    pthread_mutex_unlock(&shared_data->mutex_area);

    pthread_barrier_wait(&shared_data->barrier);

}


int main(int argc, char* arg[]) {
    
    walltime_t timer;

    double a;
    double b;
    size_t n;

    
    size_t thread_count = 0;

    if (argc >= 5) {
        a = (size_t)strtoul(arg[1], NULL, 10);
        b = (size_t)strtoul(arg[2], NULL, 10);
        n = (size_t)strtoul(arg[3], NULL, 10);
        thread_count = (size_t)strtoul(arg[4], NULL, 10);

    } else {
        printf("Invalid number of params");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    pthread_mutex_init(&shared_data->mutex_area, /*attr*/ NULL);

    shared_data->n = n;
    shared_data->area = 0;
    shared_data->delta = (b-a)/n;
    //printf("Delta is %f \n", shared_data->delta);
    shared_data->a = a;
    shared_data->b = b;

    walltime_start(&timer);

    if (thread_count > n){
        thread_count = n;
    }
    
    pthread_barrier_init (&shared_data->barrier, NULL, thread_count);
        
    shared_data->rectangles_thread = n/thread_count;
    shared_data->thread_count = thread_count;

    for (size_t i = 0; i< thread_count ; ++i) {
        thread_data_list[i].n = 0;
        if(i==thread_count){
            thread_data_list[i].n = shared_data->n%shared_data->thread_count;
        }
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, distribute, (void*)&thread_data_list[i]);
    }   


    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }
    printf("El area es: %.2f\n", shared_data->area);
    printf("El tiempo de duracion fue %f \n", walltime_elapsed(&timer));

    pthread_mutex_destroy(&shared_data->mutex_area);
    
    free(threads);
    free(shared_data);
}