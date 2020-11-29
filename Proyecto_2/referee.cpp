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
    MPI_Status status;

    int coins[num_processes];
    int attacking[num_processes];
    int activePlayers[num_processes];
    int goobas[num_processes];
    int printer = 0;
    bool finished = 0;
    int coins;
    int attacking;
    int active;
    int goobas;
    int es;
    int sincronization=0;

    if(my_id ==0){
        Estrategies es;
        cout << "Digite el tipo de estrategia a usar. 1 para aleatorio, 2 para menos coins, 3 para mas coins y 4 para attacker: \n"; 
        cin>>es;
        cout << "Digite el numero de proceso que se encargara de imprimir";
        cin>>printer;
        while(!finished){
            MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST
            MPI_Send(&es, 1 , MPI_INT , printer , 0 , MPI_COMM_WORLD);
            sincronization=1;
            MPI_Bcast(&sincronization, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST
            finished = game_finished(&allActivePlayers, num_processes);
            MPI_Bcast(&finished, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST

        }
    }
    if(my_id != 0 ){
        if (my_id == printer){
            MPI_Recv( &es,1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD, &status);
        }
        while(!sincronization){}

        Mario* mario = new Mario(my_id);


        while (mario.isAlive()){
            MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST

            MPI_Allgather(&coins, 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&attacking, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&activePlayers, 1 , MPI_INT , active , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&goobas, 1 , MPI_INT , goobas , 1 , MPI_INT , MPI_COMM_WORLD);


            /*
                Calcular a quien atacar
                calcular cantos bichitos meter en el mundo

            */
            /*refrescar el mundo*/
            if(mario->world->current_position ==0){
                vector<int> elements = mario->world->getPositon();
            }else{
                vector<int> elements = mario->world.getNext();
            }
                for( auto& element: elements){
                /*
                do something with each element
                */
            }
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

int choose_new_printer(){
    
}