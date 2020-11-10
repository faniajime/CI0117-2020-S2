#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<omp.h>
#include <math.h>

using namespace std;

bool isPrime(int number)
{
    if(number<2){
        return false;
    }
    if (number == 2){
        return true;
    }
    if (number % 2 == 0){
        return false;
    }
    int last = (int)sqrt(number);
    for(int i = 3; i<=last; i+=2){
        if (number % i == 0){
            return false;
        }
    }
    return true;
}

int main(int argc, char* arg[]) {
    int finalCount;

    if (argc >= 2) {
        finalCount = (int)strtoul(arg[1], NULL, 10);

    } else {
        printf("Invalid number of params");
        return 1;
    }

    bool prime = false;
    int count =0;
    
    #pragma omp parallel for default(none) shared(finalCount) private(prime) reduction(+:count) schedule(runtime)
    for (int i = 2; i<= finalCount; i++){
        prime = isPrime(i);
        //printf(" The number %i prime value is %d\n", i, prime);
        if (prime == true){
            count+=1;
        }
        prime = false;
        
        //printf("Hilo %i Cantidad de hilos utilizados %d\n", omp_get_thread_num(), omp_get_num_threads());
        
    }
    printf("La cantidad de primos encontrados fue %d, entre 2 y %d\n", count, finalCount);
    
}