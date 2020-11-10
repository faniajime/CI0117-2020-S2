#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    int message_sent = 0;
    int message_recieved = 0;
    
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    if(my_id ==0 ){
        for (int i = 1; i<num_processes; i++){
            message_sent = i;
            MPI_Send(&message_sent, 1 /*count*/, MPI_INT, i /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
        }
    }else{
        MPI_Recv(&message_recieved, 1 /* count*/, MPI_INT, 0 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }
    cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << ". I recieved message " << message_recieved << endl;

    MPI_Finalize();

    return 0;
}