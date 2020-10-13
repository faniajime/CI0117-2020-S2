#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "dna_sequences.h"

/*       Esta seccion del codigo toma el tiempo      */

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

/*     A partir de aca esta la implementacion de la solucion      */

typedef struct {

    int threads;
    pthread_mutex_t mutex;
    int* counts1;
    int* counts2;
    int seq;
    int* compares;

} shared_data_t;

typedef struct {

    shared_data_t* shared_data;
    size_t thread_num;

} thread_data_t;

void* countDNA(void* args ){
    thread_data_t* data = (thread_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;
    if (shared_data->seq == 1){
        int nparatodos = len1/shared_data->threads;
        int npersonal = nparatodos;
        if(thread_num == shared_data->threads - 1){
            npersonal = (nparatodos + len1%shared_data->threads);
        }
        int inicio = thread_num*nparatodos;
        int position;
        for (int i = inicio; i < inicio+npersonal; ++i){
            position = sequence1[i] - 97;
            pthread_mutex_lock(&shared_data->mutex);
            shared_data->counts1[position] += 1;
            pthread_mutex_unlock(&shared_data->mutex);
        }  
    }else{
        int nparatodos = len2/shared_data->threads;
        int npersonal = nparatodos;
        if(thread_num == shared_data->threads - 1){
            npersonal = (nparatodos + len2%shared_data->threads);
        }
        int inicio = thread_num*nparatodos;
        int position;
        for (int i = inicio; i < inicio+npersonal; ++i){
            position = sequence2[i] - 97;
            pthread_mutex_lock(&shared_data->mutex);
            shared_data->counts2[position] += 1;
            pthread_mutex_unlock(&shared_data->mutex);
        }
    }
    
}


void* compareSequences(void* args ){
    thread_data_t* data = (thread_data_t*) args;
    size_t thread_num = data->thread_num;
    shared_data_t* shared_data = data->shared_data;
    
    int nparatodos = 26/shared_data->threads;
    int npersonal = nparatodos;
    if(thread_num ==  shared_data->threads - 1){
        npersonal = (nparatodos + 26%shared_data->threads);
    }
    
    int inicio = thread_num*nparatodos;
    int position;
    for (int i = inicio; i < inicio+npersonal; ++i){
        if((shared_data->counts1[i] > 0) && (shared_data->counts2[i]> 0)){
            pthread_mutex_lock(&shared_data->mutex);
            shared_data->compares[i] = 1;
            pthread_mutex_unlock(&shared_data->mutex);
        }
        
    }    
    
}


int main(int argc, char* arg[]) {
    
    walltime_t timer;
    
    size_t thread_count;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        printf("Invalid number of params\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));
    int* counts1 = (int*)calloc(26,sizeof(int));
    int* counts2 = (int*)calloc(26,sizeof(int));
    int* compares = (int*)calloc(26,sizeof(int));
    pthread_mutex_init(&shared_data->mutex, /*attr*/ NULL);


    walltime_start(&timer);
    

    shared_data->counts1 = counts1;
    shared_data->threads = (int)thread_count;
    shared_data->seq = 1;
    if (thread_count>len1){
        shared_data->threads = len1;
    }
    

    for (size_t i = 0; i< shared_data->threads ; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, countDNA, (void*)&thread_data_list[i]);
    }   

    for (size_t i = 0; i < shared_data->threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    shared_data->counts2 = counts2;
    shared_data->seq = 2;

    if (thread_count>len2){
        shared_data->threads = len2;
    }else{
        shared_data->threads = thread_count;
    }

    
    
    for (size_t i = 0; i< shared_data->threads ; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, countDNA, (void*)&thread_data_list[i]);
    }   

    for (size_t i = 0; i < shared_data->threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    printf("La secuencia 1 tiene: ");
    for (int i = 0; i< 26 ; ++i) {
        if(shared_data->counts1[i]>0)
            printf("%c = %d, ", i+97, shared_data->counts1[i]);
    }   
    printf("\n");

    printf("La secuencia 2 tiene: ");
    for (int i = 0; i< 26 ; ++i) {
        if(shared_data->counts2[i]>0)
            printf("%c = %d, ", i+97, shared_data->counts2[i]);
    }   
    printf("\n");
    
    if (thread_count>26){
        shared_data->threads = 26;
    }else{
        shared_data->threads = thread_count;
    }

    shared_data->compares = compares;
    shared_data->threads = thread_count;
    for (size_t i = 0; i< shared_data->threads ; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, compareSequences, (void*)&thread_data_list[i]);
    }


    for (size_t i = 0; i < shared_data->threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Las letras compartidas entre ambas son: ");
    for (int i = 0; i< 26 ; ++i) {
        if(shared_data->compares[i]>0)
            printf("%c, ", i+97);
    }   
    printf("\n");
    printf("===========================================================================\n");
    printf("\n");
    printf("El tiempo de duracion fue %f \n", walltime_elapsed(&timer));

    pthread_mutex_destroy(&shared_data->mutex);
    
    free(counts1);
    free(counts2);
    free(compares);
    free(threads);
    free(thread_data_list);
    free(shared_data);
    
}