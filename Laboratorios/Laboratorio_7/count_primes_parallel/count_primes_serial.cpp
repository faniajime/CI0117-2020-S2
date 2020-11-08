#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

bool isPrime(int number){
    if(number<2){c;
        return false;
    }
    if (number == 2){
        return true;
    }
    if (number % 2 == 0){
        return false;
    }
    int last = (int)sqrt(number);
    for(int i = 3; i<=last; i+=2){
        if (number % i == 0){
            return false;
        }
    }
    return true;
}

int main(int argc, char* arg[]) {
    int finalCount;

    if (argc >= 2) {
        finalCount = (int)strtoul(arg[1], NULL, 10);

    } else {
        printf("Invalid number of params");
        return 1;
    }

    bool prime = false;
    int count =0;
    for (int i = 2; i<= finalCount; i++){
        prime = isPrime(i);
        //printf(" The number %i prime value is %d\n", i, prime);
        if (prime == true){
            count+=1;
        }
        prime = false;
    }
    printf("La cantidad de primos encontrados fue %d, entre 2 y %d\n", count, finalCount);
}