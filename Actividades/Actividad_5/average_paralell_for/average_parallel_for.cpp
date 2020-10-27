#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <string> 

using namespace std;

static void calculate_sum(char **array, int array_size, double* global_sum){

    int num_thread = omp_get_thread_num();
    double sum = 0;

    for( int i=0; i<array_size; i++)
    {
        printf("Thread %d: Processing number %s \n", num_thread, array[i]);
        sum = sum + stod(array[i]);
    }
    
    #pragma omp critical
    {
        *global_sum+= sum;
    }
}

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
    int to_add = global_size/thread_nums;


    #pragma omp parallel num_threads(thread_nums) shared(global_sum)
    {
        int num_thread = omp_get_thread_num();
        calculate_sum(argv+1+num_thread*to_add, to_add, &global_sum);
    }            

    cout << "Average: " << (global_sum / global_size) << endl;
        
    return 0;
}