#include <iostream>
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>     
#include <time.h>  

using namespace std;


int main(int argc, char *argv[]) {
    int my_value;

    int my_id, num_processes;
    int message_received, message_sent;
    MPI_Status status;
    srand(time(NULL) * my_id * 1000); /* Initialize seed */

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    int size;

    int min;
    int max;
    int sum;


    if (my_id == 0) {
        cout << "Please enter the size of the array: " << endl;
        cin >> size;
    }

    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int array[size];
    if (my_id == 0) {
        for(int i =0; i<size; i++){
            cout << "Insert the  " << i<< " number of the array: " << endl;
            cin >> array[i];
        }   
    }
    int index = rand() % size;
    MPI_Bcast(array, size, MPI_INT, 0, MPI_COMM_WORLD);
    my_value = array[index];  

    MPI_Allreduce(&my_value, &max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&my_value, &min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&my_value, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    int average = sum/num_processes;
    if (my_id != 0) {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }
    if (my_value  == min){
        cout << "Hello. I am process: " << my_id << ". I chose: " << my_value << ". My value is the minimum. " << endl;
    }
    else if(my_value == max){
        cout << "Hello. I am process: " << my_id << ". I chose: " << my_value << ". My value is the maximum. " << endl;
    }
    else if (my_value > average){
        cout << "Hello. I am process: " << my_id << ". I chose: " << my_value << ". My value is above the average (" << average << ")." << endl;

    }
    else if (my_value < average){
        cout << "Hello. I am process: " << my_id << ". I chose: " << my_value << ". My value is below the average (" << average << ")." << endl;
    }
    else
    {
        cout << "Hello. I am process: " << my_id << ". I chose: " << my_value << ". My value is equal to the average (" << average << ")." << endl;
    }

	if ( my_id < num_processes - 1 ) {
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;
}