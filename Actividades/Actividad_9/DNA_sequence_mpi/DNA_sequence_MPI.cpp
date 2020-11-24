#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "mpi.h"

using namespace std;

void initArray(int *array, int size)
{
    for (int i = 0; i < size; ++i)
    {
        array[i] = 0;
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int my_id, num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);


    string sequence;
    int sequence_count[26];

    initArray(sequence_count, 26);

    if (argc < 2)
    {
        cout << "Invalid number of params." << endl;
        return 1;
    }

    ifstream file(argv[1]);

    if (file.is_open())
    {
        getline(file, sequence);
        file.close();
    }
    else
    {
        cout << "Unable to open file" << argv[1] << endl;
        return 1;
    }

    int sequence_size = sequence.length();
    string newsequence;
    int newsize, chunksize;
    if (my_id == 0){
        int rest = sequence_size%num_processes;
        if ( rest == 0){
            chunksize = sequence_size/num_processes;
            newsize = chunksize*num_processes;
            newsequence = sequence.substr(0,newsize);
        }else{
            chunksize = sequence_size/num_processes;
            newsize = sequence_size;
            newsequence = sequence;
        }
    }
    MPI_Bcast(&chunksize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    string mysequence;
    MPI_Scatter(newsequence,chunksize, MPI_CHAR, mysequence, chunksize, MPI_CHAR, 0, MPI_COMM_WORLD);

    int index;
    for (int i = 0; i < chunksize; ++i)
    {
        index = sequence[i] - 'a';
        if (index < 26 && index >= 0)
        {
            sequence_count[index]++;
        }
        else
        {
            cout << "Invalid char [" << sequence[i] << "]" << endl;
        }
    }
    MPI_Reduce(&sequence_count, &sequence_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    cout << "Sequence occurrences:";

    if (my_id ==0){

        for (int j = 0; j < 26; ++j)
        {
            if (sequence_count[j] > 0)
            {
                cout << " " << (char)('a' + j) << "=" << sequence_count[j];
            }
        }
        cout << endl;
    }
    MPI_Finalize();
    return 0;
}