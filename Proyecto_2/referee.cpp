#include <iostream>
#include <string>
#include "mpi.h"
#include "Mario.cpp"
#include "Player.cpp"
#include "world.cpp"
#include <vector>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int my_id, num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int coins[num_processes];
    int attacking[num_processes];
    int activePlayers[num_processes];
    int goobas[num_processes];
    int printer = 0;
    bool finished;
    
    int allCoins[num_processes];
    int allAttacking[num_processes];
    int allActivePlayers[num_processes];
    int allGoobas[num_processes];

    if(my_id ==0){
        
        while(!finished){
            MPI_Bcast(, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST

            finished = game_finished(&allActivePlayers, num_processes);
        }
    }
    if(my_id != 0 ){
        Mario* mario = new Mario(my_id);

        while (mario.isAlive()){
            MPI_Bcast(, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST

            MPI_Allgather(&coins, 1 , MPI_INT , &allCoins , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&attacking, 1 , MPI_INT , &allAttacking , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&activePlayers, 1 , MPI_INT , &allActivePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&goobas, 1 , MPI_INT , &allGoobas , 1 , MPI_INT , MPI_COMM_WORLD);


            /*
                Calcular a quien atacar
                calcular cantos bichitos meter en el mundo

            */
            vector<int> elements = mario->world.getNext();
            for( auto& element: elements){
                /*
                do something with each element
                */
            }
            if (my_id == printer){
                //Se imprime
            }

        }
        while(!finished){}
    }
    cout << "El juego ha terminado";
    MPI_Finalize();
    return 0;
}


bool game_finished(int* active, int size){
    int alive = 0;
    for(int i = 1; i<size; i++){
        if(active[i]){
            alive+=1;
        }
    }
    if(active<=1){
        return true;
    }else{
        return false;
    }

}