#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>
#include <string> 

using namespace std;

int calculate_sum(char **array, int array_size, double global_sum){

    int num_thread = omp_get_thread_num();
    double sum = 0;

    for( int i=0; i<array_size; i++)
    {
        printf("Thread %d: Processing number %s", num_thread, array[num_thread+i]);
        sum = sum + stoi(array[1+num_thread+i]);
    }
    
    #pragma omp critical
    {
        global_sum+= sum;
    }
    return global_sum;
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        cout << "Invalid number of parameters.\n";
        return 1;
    }
    int thread_nums;


    double global_sum = 0.0;
    int global_size = argc-1;
    thread_nums = global_size;


    #pragma omp parallel num_threads(thread_nums) shared(global_sum)
    {
        int num_thread = omp_get_thread_num();
        calculate_sum(&argv[1+num_thread], global_size, global_sum);
    }            

    cout << "Average: " << (global_sum / global_size) << endl;
        
    return 0;
}