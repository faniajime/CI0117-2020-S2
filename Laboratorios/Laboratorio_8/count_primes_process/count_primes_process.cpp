#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<omp.h>
#include <math.h>
#include "mpi.h"
#include <unistd.h>
#include <time.h>  

using namespace std;

bool isPrime(int number){
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

int main(int argc, char *argv[]) {
    int finalCount;

    int my_id, num_processes;
    int message_received, message_sent;

    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    double starttime, endtime;
    starttime = MPI_Wtime();
    int total = 0;

    if (argc >= 2) {
        finalCount = (int)strtoul(argv[1], NULL, 10);

    } else {
        printf("Invalid number of params");
        return 1;
    }
    bool prime = false;
    int count = 0;
    if (finalCount < num_processes){
        num_processes = finalCount;
    }
    int size = (finalCount)/num_processes;
    for (int i = my_id*size; i<(my_id*size+size); i++){
        prime = isPrime(i);
        //printf(" The number %i prime value is %d\n", i, prime);
        if (prime == true){
            total+=1;
        }
        prime = false;
    }
    prime = false;
    if(my_id == 0 && finalCount%num_processes != 0){
        for (int i = (finalCount-finalCount%num_processes); i< finalCount; i++ ){
            prime = isPrime(i);
            //printf(" The number %i prime value is %d\n", i, prime);
            if (prime == true){
                total+=1;
            }
            prime = false;
        }

    }
    
    if(my_id != 0){
        message_sent = total;
        printf("total %d from thread %d\n", total, my_id);
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, 0 /*dest*/, my_id /*message id*/, MPI_COMM_WORLD);
    }
    if (my_id == 0){
        printf("total %d from thread %d\n", total, my_id);
        for(int i = 1; i<num_processes; i++){
            MPI_Recv(&message_received, 1 /* count*/, MPI_INT, i /*source*/, i /*message id*/, MPI_COMM_WORLD, &status);
            total+= message_received;
        }
        endtime = MPI_Wtime();
        printf("La cantidad de primos encontrados fue %d, entre 1 y %d. En un tiempo de %f\n", total, finalCount, endtime-starttime);
    }
    MPI_Finalize();

    return 0;
}