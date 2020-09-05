#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void* emptyfunct(void* args){
    return NULL;
}

int main(int argc, char *arg[]){

    int T = 0;
    if (argc >= 1){
         T = atoi(arg[1]);
    }else{
        int T = 1;
    }

    struct timespec inicio, final; 
    int dif, menor;
    menor = 100000;
    for (int i = 0; i<=T; ++i){
        clock_gettime(CLOCK_REALTIME, &inicio);
        pthread_t thread;
        pthread_create(&thread, NULL, emptyfunct, NULL);
        clock_gettime(CLOCK_REALTIME, &final);
        dif = (final.tv_nsec - inicio.tv_nsec);
        if(menor > dif){
            menor = dif;
        }
    }
    
    
    printf("El tiempo minimo de creacion y destruccion de un hilo fue: %u nanosegundos, de un total de %u pruebas \n", menor, T);
    return 0;

}