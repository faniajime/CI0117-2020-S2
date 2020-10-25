#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    return resultado;
}

double riemann(double a, double b, double n)
{
    double area = 0;
    double delta = (b-a)/n;
    double x = a;
    
    while(x<=b-delta)
    {
        area += funcion(x)*delta;        
        x+=delta;           
    }
    return area;
}

int main(int argc, char* arg[]) {
    
    walltime_t timer;

    walltime_start(&timer);

    size_t a;
    size_t b;
    size_t n;

    if (argc >= 4) {
        a = (size_t)strtoul(arg[1], NULL, 10);
        b = (size_t)strtoul(arg[2], NULL, 10);
        n = (size_t)strtoul(arg[3], NULL, 10);
    } else {
        printf("Invalid number of params");
        return 1;
    }
    printf("El area es: %.2f\n", riemann(a,b,n));
    printf("El tiempo de duracion fue %f \n", walltime_elapsed(&timer));

}