#include <iostream>
#include <string>
#include "mpi.h"
#include "Mario.h"
#include "Player.h"
#include "world.h"
#include <vector>



bool game_finished(int* active, int size){
    int alive = 0;
    bool gameFinished = false;
    for(int i = 1; i<size; i++){
        if(active[i]){
            alive+=1;
        }
    }
    if(alive<=1){
        gameFinished = true;
    }
    return gameFinished;
}

int choose_new_printer()
{
    return 0;
}


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
    int finished = 0;
    int mycoins= 0;
    int iattacking=0;
    int imactive=1;
    int mygoombas=0;
    int myKoopas = 0;
    int estrategy = 0;
    int sincronization=0;
    int maxCoins;
    int minCoins;
    int my_coins; 
    int amIalive;
    int es;

    if(my_id ==0){
        
        //Ingreso de datos y validaciones
        cout<< "Digite el tipo de estrategia a usar. 1 para aleatorio, 2 para menos coins, 3 para mas coins y 4 para attacker: "; 
        cin>>estrategy;
        if(estrategy>4 || estrategy < 1){
            cout << "El numero de estrategia ingresado no es valido \n" << endl;
            return 1;
        }
        cout << "Digite el numero de Mario que quiere observar: ";
        cin>>printer;
        if(printer<=0 || printer >= num_processes){
            cout << "El numero de proceso ingresado no es valido \n" << endl;
            return 1;
        }
    }
    MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&estrategy, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Player* player;
    if (my_id != 0){
        player = new Player(my_id);
        if (my_id != printer){
            estrategy = (rand() % 4) +1;
        }
        player->setEstrategy((Estrategies) estrategy);
    }

    while(!finished){
        if(!player->mario->isAlive()){
            break;
        }
        
    }
    // while (player->mario->isAlive())// and no
    // {
    //     MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);// HAY QUE DETERMINAR LA FUNCION DE ESTE BCAST
    //     my_coins = player->mario->getCoins();
    //     MPI_Allgather(&my_coins, 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
    //     MPI_Allgather(&attacking, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD); //pensar esta mejor
    //     amIalive = player->mario->isAlive();
    //     MPI_Allgather(&amIalive, 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
    //     MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD);
    //     MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD);

    //     int action = 0;
    //     vector<int> elements;
    //     if(player->mario->world->getCurrentPosition() ==0)//***revisar, esto o getPosition****
    //     {
    //         elements = player->mario->world->getPosition();
    //     }
    //     else
    //     {
    //         elements = player->mario->world->getNext();
    //     }

    //     for( auto& element: elements)
    //     {   
    //         if(player->mario->isAlive())
    //         {
    //             int enemy = 0;
    //             action = player->mario->chooseAction((Elements)element);
    //             if(action==1)//coin
    //             {
    //                 player->mario->addCoins();
    //                 my_coins=player->mario->getCoins();
    //                 MPI_Allgather(&my_coins, 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
    //             }
    //             else if(action==2)//element is an enemy: goomba or koopatroopa
    //             {
    //                 if(es==R)
    //                 {
    //                     enemy = activePlayers[rand()%num_processes];
    //                     if(element==Goomba)  {
    //                         goombas[enemy]++;
    //                     }else {
    //                         koopatroopas[enemy]++;
    //                     }
                        
    //                 }
    //                 else if(es==L)
    //                 {
    //                     //en caso de no funcionar, quitar Allreduce
    //                     MPI_Allreduce(&my_coins, &minCoins, 1, MPI_INT , MPI_MIN , MPI_COMM_WORLD);
    //                     for(int i = 0; i<num_processes;i++)
    //                     {
    //                         if(coins[i]==minCoins)
    //                         {
    //                             enemy = i;                                
    //                         }

    //                         if(element==Goomba){
    //                             goombas[enemy]++;
    //                         } else {
    //                             koopatroopas[enemy]++;
    //                         }
    //                     }
    //                 }
    //                 else if (es==M)
    //                 {
    //                     MPI_Allreduce(&my_coins, &maxCoins, 1, MPI_INT , MPI_MAX , MPI_COMM_WORLD);
    //                     for(int i = 0; i<num_processes;i++)
    //                     {
    //                         if(coins[i]==maxCoins)
    //                         {
    //                             enemy = i;                                
    //                         }

    //                         if(element==Goomba){
    //                             goombas[enemy]++;
    //                         } else {
    //                             koopatroopas[enemy]++;
    //                         }
    //                     }
    //                 }else{
    //                     for(int i = 0;i<num_processes;++i)
    //                     {
    //                         if(attacking[i]==1)
    //                         {
    //                             enemy = i;
    //                         }

    //                         if(element==Goomba){
    //                             goombas[enemy]++;
    //                         } else {
    //                             koopatroopas[enemy]++;
    //                         }
    //                     }
    //                 }

    //             }
    //             else// jump and pass or die
    //             {
    //                 if(player->mario->isAlive())
    //                 {
    //                     player->mario->world->getNext();
    //                 }
    //             }     
    //     }             
    // }
    
    // // marcar la casilla cuando muere
    // activePlayers[my_id] = 0;

    // if (my_id == printer){
    //     //Se imprime
    // }

    //SETEAR LOS ARREGLOS QUE SEAN NECESARIOS EN CERO
    my_coins = player->mario->getCoins();
    amIalive = player->mario->isAlive();
    MPI_Allgather(&my_coins, 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
    MPI_Allgather(&attacking, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD); //pensar esta mejor
    MPI_Allgather(&amIalive, 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
    MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD); //PENSAR COMO CAMBIARLO
    MPI_Allgather(&myKoopas, 1 , MPI_INT , koopatroopas , 1 , MPI_INT , MPI_COMM_WORLD); //ANADIR LOGICA DE KOOPATROPAS

    while(!finished)
    cout << "El juego ha terminado"<< endl;
    MPI_Finalize();
    return 0;
}


