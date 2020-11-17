#include <iostream>
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>     
#include <time.h>  

using namespace std;


int main(int argc, char *argv[]) {

    int my_id, num_processes;
    int message_received, message_sent;
    MPI_Status status;
    srand(time(NULL) * my_id * 1000); /* Initialize seed */

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    int size;

    if (my_id == 0) {
        cout << "Please enter the size of the array: " << endl;
        cin >> size;
        int array[size];
        int min;
        int max;
        int sum;
    }
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_id == 0) {
        for(int i =0; i<size; i++){
            cout << "Insert the  " << i<< " number of the array: " << endl;
            cin >> array[i];
        }   

    }

    MPI_Bcast(&array, size, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_id != 0) {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
        int buffer_array[message_received];
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
        buffer_array = message_received;
    }

    int index = rand() % size;
    int my_value = array[index];

    MPI_Reduce(&my_value, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_value, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (my_id != 0) {
        MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }

    cout << "Hello. I am process: " << my_id << "I chose number  "  << my_value << endl;
    if (my_id ==0){
        cout << "Min is " << min << endl;
        cout << "Max is " << max << endl;
        cout << "Sum is " << sum << endl;
    }

	if ( my_id < num_processes - 1 ) {
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;
}