#include <iostream>
#include "mpi.h"
#include <cstdio>
#include <cstring>

#define MAX_MESSAGE_SIZE 1024

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    char message[MAX_MESSAGE_SIZE];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    if (my_id == 0) {
        for (int i = 1; i < num_processes; ++i) {
            MPI_Recv(&message, MAX_MESSAGE_SIZE /* count*/, MPI_CHAR, MPI_ANY_SOURCE /*source*/, MPI_ANY_TAG /*message id*/, MPI_COMM_WORLD, &status);
            cout << "Process " << status.MPI_SOURCE << " arrived at position " << i << ". Message: " << message << endl;
        }
    } else {
        sprintf(message, "Hello, I am process %d of %d!\n", my_id, num_processes);
        MPI_Send(&message, MAX_MESSAGE_SIZE /*count*/, MPI_CHAR, 0 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}