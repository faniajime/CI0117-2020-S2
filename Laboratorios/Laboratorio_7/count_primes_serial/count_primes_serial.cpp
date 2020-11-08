#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <omp.h>

using namespace std;

typedef struct timespec walltime_t;

void walltime_start(walltime_t* start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);
	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;
	return elapsed;
}

double funcion(double x)
{
    double resultado = 0;
    resultado = (x*x)+1;
    //printf("resultado= %f\n",resultado);
    return resultado;
}

double riemann(double a, double b, int n, int thread_num)
{
    double area = 0;
    double delta = (b-a)/n;
    if(thread_num > n){
        thread_num = n;
    }
    
    #pragma omp parallel for num_threads(thread_num) default(none) shared(delta, n, a) reduction(+:area)
    for (int i= 0; i<n; i++){
        area += funcion(a+(i*delta))*delta;
    }
    return area;
}

int main(int argc, char* arg[]) {
    
    walltime_t timer;

    double a;
    double b;
    size_t n;
    size_t thread_count = 0;

    if (argc >= 5) {
        a = (size_t)strtoul(arg[1], NULL, 10);
        b = (size_t)strtoul(arg[2], NULL, 10);
        n = (size_t)strtoul(arg[3], NULL, 10);
        thread_count = (size_t)strtoul(arg[4], NULL, 10);

    } else {
        printf("Invalid number of params");
        return 1;
    }

    walltime_start(&timer);

    double area = riemann(a,b,n,thread_count);

    printf("El area es: %.2f\n", area);
    printf("El tiempo de duracion fue %f \n", walltime_elapsed(&timer));

}