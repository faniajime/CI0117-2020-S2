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
    int total_global;

    if (my_id==0)
    {
        cout<<"Escriba el numero hasta el que desea contar los numeros primos"<<endl;
        cin>> finalCount;
    }

    MPI_Bcast(&finalCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    bool prime = false;
    int count = 0;
    if (finalCount < num_processes){
        num_processes = finalCount;
    }
    int size = (finalCount)/num_processes;
   
    for (int i = my_id*size; i<(my_id*size+size); i++)
    {
       if (isPrime(i))
        {
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


    MPI_Allreduce(&total, &total_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    
   if (my_id != 0) 
    {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }
    
    if ( my_id < num_processes - 1 ) 
        {        
            MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
        }

    if (my_id == 0){
        endtime = MPI_Wtime();
        printf("La cantidad de primos encontrados fue %d, entre 2 y %d. En un tiempo de %f\n", total_global, finalCount, endtime-starttime);
    }
    MPI_Finalize();

    return 0;
}