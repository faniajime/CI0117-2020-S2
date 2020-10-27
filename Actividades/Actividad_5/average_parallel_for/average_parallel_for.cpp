#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <string> 

using namespace std;


int main(int argc, char *argv[])
{
    if (argc <= 1) {

        cout << "Invalid number of parameters.\n";
        return 1;
    }
    if ((argc-1)%5 != 0) {

        cout << "La cantidad de numeros debe ser multiplo de 5.\n";
        return 1;
    }
    int thread_nums;


    double global_sum = 0.0;
    int global_size = argc-1;
    thread_nums = 5;


    #pragma omp parallel for num_threads(thread_nums) shared(global_sum)
    for( int i=0; i<argc-1; i++)
    {
        printf("Thread %d: Processing number %s \n", omp_get_thread_num(), argv[1+i]);
        #pragma omp critical
        {
            global_sum = global_sum + atof(argv[1+i]);
        }

    }
    cout << "Average: " << (global_sum / global_size) << endl;
        
    return 0;
}