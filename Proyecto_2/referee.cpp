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
    int prints[num_processes]  = {0};
    int printer;
    int finished = 0;
    int mycoins= 0;
    int iattacking=0;
    int imactive=1;
    int mygoombas=0;
    int myKoopas = 0;
    int estrategy;
    int sincronization=0;
    int maxCoins;
    int minCoins;
    int my_coins; 
    int amIalive;

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
        if(printer<=0 || printer >= num_processes-1){
            cout << "El numero de proceso ingresado no es valido \n" << endl;
            return 1;
        }else{
            prints[printer] = 1;
        }
    }
    MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&estrategy, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Player* player = new Player(my_id);
    if(my_id ==printer)
    {
        player->setEstrategy((Estrategies) estrategy);
    }

    while(player->mario->isAlive())
    {
        
        //esta logica no la revise
        my_coins = player->mario->getCoins();
        MPI_Allgather(&my_coins, 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&attacking, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD); //como se quien ataca?***
        amIalive = player->mario->isAlive();
        MPI_Allgather(&amIalive, 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&myKoopas, 1 , MPI_INT , koopatroopas , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allreduce(&my_coins, &minCoins, 1, MPI_INT , MPI_MIN , MPI_COMM_WORLD);
        MPI_Allreduce(&my_coins, &maxCoins, 1, MPI_INT , MPI_MAX , MPI_COMM_WORLD);

        if(my_id!=0)
        {
            if(player->mario->world->getCurrentPosition() > 99){ //si ya pasamos por la casilla final entonces refresquese
                player->mario->world->refreshWorld();
            }

            int action = 0;
            vector<int> elements;
            if(player->mario->world->getCurrentPosition() ==0)
            {
                elements = player->mario->world->getPosition();
            }
            else
            {
                elements = player->mario->world->getNext();
            }
            for( auto& element: elements)
            {   
                if(player->mario->isAlive())
                {
                    int enemy = 0;
                    action = player->mario->chooseAction((Elements)element);
                    if(action==1)//coin
                    {
                        player->mario->addCoins();
                        my_coins=player->mario->getCoins();
                    }
                    else if(action==2)//element is an enemy: goomba or koopatroopa
                    {
                        if(estrategy==R){
                            enemy = activePlayers[rand()%1+(num_processes-1)];// va de 1 a numero de procesos-1
                            
                            if(element==Goomba){
                                goombas[enemy]++;
                            }else{
                                koopatroopas[enemy]++;
                            }
                        }
                        else if (estrategy == L)
                        {
                            
                            for(int i = 1; i<num_processes;i++){
                                if(coins[i]==minCoins){
                                    enemy = i;
                                }
                                if(element==Goomba){
                                    goombas[enemy]++;
                                }
                                else{
                                    koopatroopas[enemy]++;
                                }
                            }
                        }
                        else if (estrategy == M)// ataca al que tiene mas monedas
                        {                        
                            for(int i = 1; i<num_processes; i++){
                                if(coins[i]==maxCoins && activePlayers[i]==1)
                                {
                                    enemy = i; 
                                    if(element==Goomba){
                                        goombas[enemy]++;
                                    } else {
                                        koopatroopas[enemy]++;    
                                    }    
                                    break;                           
                                }
                                
                            }
                        }else{// estrategia: atacar a mi atacante (ley del Talion)
                            for(int i = 1;i<num_processes;++i)
                            {
                                if(attacking[i]==my_id && activePlayers[i]==1) //si un Mario me ataca y esta vivo, se convierte en mi enemigo 
                                {
                                    enemy = i;
                                    
                                    if(element==Goomba){
                                        goombas[enemy]++;
                                    } else {
                                        koopatroopas[enemy]++;
                                    }
                                    break;// termina ciclo para que no haga mas de un ataque
                                }                                
                            }
                        }
                    }else{
                        if(player->mario->isAlive())
                        {
                            player->mario->world->getNext();
                        }
                    }   
                    attacking[my_id] = enemy;
                }
            }
            if(my_id==0)
            {
                finished = game_finished(activePlayers, num_processes))               
            }
        }
    }

    if (my_id == printer && !finished){
        cout << "Oh no, su mario murio, escoja un nuevo jugador al que observar, estas son sus opciones:";
        for(int i =1; i<num_processes,i++)
        {
            if(activePlayers[i]==1)
            {
                cout<<i<<" "<<endl;
            }
        }
        cin>>printer;
        if(printer<=0 || printer >= num_processes-1){
            cout << "El numero de proceso ingresado no es valido \n" << endl;
            return 1;
        }
        if(!activePlayers[printer]){
            cout << "El mario que escogio no esta vivo \n" << endl;
            return 1; 
        }
        MPI_Bcast(&printer, 1, MPI_INT, my_id, MPI_COMM_WORLD);
    }

    
    cout << "El juego ha terminado"<< endl;
    MPI_Finalize();
    return 0;
}


