#include <iostream>
#include <string>
#include "mpi.h"
#include "Mario.cpp"
#include "Player.cpp"
#include "world.cpp"
#include <vector>


int main(int argc, char *argv[])
{

    //agregar logica de jugador

    MPI_Init(&argc, &argv);
    int my_id, num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Status status;

    int coins[num_processes];
    int attacking[num_processes];
    int activePlayers[num_processes];
    int goombas[num_processes]; 
    int koopatroopas[num_processes];
    int printer = 0;
    bool finished = 0;
    int mycoins= 0;
    int iattacking=0;
    int imactive=1;
    int mygoombas=0;
    int es;
    int sincronization=0;
    int maxCoins;
    int minCoins;

    if(my_id ==0){
        int es;
        cout<< "Digite el tipo de estrategia a usar. 1 para aleatorio, 2 para menos coins, 3 para mas coins y 4 para attacker: \n"; 
        cin>>es;
        cout << "Digite el numero de Mario que quiere observar";
        cin>>printer;
        while(!finished){
            MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Send(&es, 1 , MPI_INT , printer , 0 , MPI_COMM_WORLD);
            sincronization=1;
            MPI_Bcast(&sincronization, 1, MPI_INT, 0, MPI_COMM_WORLD);
            finished = game_finished(&activePlayers, num_processes);
            MPI_Bcast(&finished, 1, MPI_INT, 0, MPI_COMM_WORLD);
            //agregar logica de sincronizacion en caso de que faltes
        }
    }
    if(my_id != 0 ){
        if (my_id == printer){
            MPI_Recv( &es,1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD, &status);
        }
        while(!sincronization){}

        Player* player(my_id);// = new Player(my_id);
        player->setEstrategy(es);

        while (player->mario().isAlive())// and no
        {
            MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST

            MPI_Allgather(&player->mario.getCoins(), 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&attacking, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD); //pensar esta mejor
            MPI_Allgather(&player->mario.isAlive(), 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD);

            int action = 0;
            vector<int> elements;
            if(mario->world->current_position ==0)
            {
                elements = mario->world->getPositon();
            }
            else
            {
                elements = mario->world.getNext();
            }

            for( auto& element: elements && player->mario.isAlive())// cerciorarse que Mario siga vivo
            {   
                int enemy = 0;
                action = player->mario.chooseAction(element);
                if(action==1)//coin
                {
                    player->mario.addCoins();
                    MPI_Allgather(&player->mario.getCoins(), 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
                }
                if else(action==2)//element is an enemy: goomba or koopatroopa
                {
                    if(es==R)
                    {
                        enemy = activePlayers[rand()%num_processes];
                        if(element==Goomba)  {
                            goombas[enemy]++;
                        }else {
                            koopatroopas[enemy]+;
                        }
                        
                    }
                    else if(es==L)
                    {
                        //en caso de no funcionar, quitar Allreduce
                        MPI_Allreduce(&my_coins, &minCoins, 1, MPI_INT , MPI_MIN , MPI_COMM_WORLD);
                        for(int i = 0; i<num_processes;i++)
                        {
                            if(coins[i]==minCoins)
                            {
                                enemy = i;                                
                            }

                            if(element==Goomba){
                                goombas[enemy]++;
                            } else {
                                koopatroopas[enemy]+;
                            }
                        }
                    }
                    else if (es==M)
                    {
                        MPI_Allreduce(&my_coins, &maxCoins, 1, MPI_INT , MPI_MAX , MPI_COMM_WORLD);
                        for(int i = 0; i<num_processes;i++)
                        {
                            if(coins[i]==maxCoins)
                            {
                                enemy = i;                                
                            }

                            if(element==Goomba){
                                goombas[enemy]++;
                            } else {
                                koopatroopas[enemy]+;
                            }
                        }
                    }else{
                        for(int i = 0;i<num_processes;++i)
                        {
                            if(attacking[i]==1)
                            {
                                enemy = i;
                            }

                            if(element==Goomba){
                                goombas[enemy]++;
                            } else {
                                koopatroopas[enemy]+;
                            }
                        }
                    }

                }
                else// jump and pass or die
                {
                    if(player->mario.isAlive())
                    {
                        player->mario.getNext();
                    }
                }                  
            }
        }
        // marcar la casilla cuando muere
        activePlayers[my_id] = 0;

            if (my_id == printer){
                //Se imprime
            }

            //SETEAR LOS ARREGLOS QUE SEAN NECESARIOS EN CERO

            MPI_Allgather(&mario->getCoins(), 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&attacking, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD); //pensar esta mejor
            MPI_Allgather(&mario->isAlive(), 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
            MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD); //PENSAR COMO CAMBIARLO
            MPI_Allgather(&KoopaTroopa, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD); //ANADIR LOGICA DE KOOPATROPAS

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