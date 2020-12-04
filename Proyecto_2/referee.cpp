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

int choose_new_printer(int num_processes, int* activePlayers)
{
    int printer;
    cout << "Oh no, su mario murio, escoja un nuevo jugador al que observar, estas son sus opciones:";
    for(int i =1; i<num_processes;++i)
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
        cout << "El mario que escogio no esta vivo, el programa terminara.\n" << endl;
        return 1; 
    }
    return printer;
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
    int enemy = 0;

    if(my_id ==0){
        
        //Ingreso de datos y validaciones
        cout<<endl;
        cout<<"--------------------------------------------------------------------------\n";
        cout<<"|    ***  ***  ***   **  ** ***   **  **  ****  ****    ***  ****   *    |\n";
        cout<<"|     *    *   *     * ** * *     * ** *  *  *  *  *     *   *  *   *    |\n";
        cout<<"|     *    *   ***   * *  * **    * *  *  ****  ****     *   *  *   *    |\n";
        cout<<"|     *    *     *   *    * *     *    *  *  *  *  *     *   *  *        |\n";
        cout<<"|    ***   *   ***   *    * ***   *    *  *  *  *    *  ***  ****   *    |\n";
        cout<<"--------------------------------------------------------------------------\n";

        cout<<"Bienvenid@ a Super Mario\n";
        cout<< "Digite el tipo de estrategia a usar:\n1.Atacar a un Mario aleatoriamente\n2.Atacar al Mario con menos monedas\n3.Atacar al Mario con mas monedas\n4.Atacar a un Mario que me esta atacando\n"; 
        cin>>estrategy;
        if(estrategy>4 || estrategy < 1){
            cout << "El numero de estrategia ingresado no es valido \n" << endl;
            return 1;
        }
        cout << "Digite el numero de Mario que quiere observar:\n ";
        cin>>printer;
        if(printer<=0 || printer >= num_processes-1){
            cout << "El numero de proceso ingresado no es valido \n" << endl;
            return 1;
        }else{
            prints[printer] = 1;
        }
    }
    MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD); //Le comunico a todos quien imprimira
    MPI_Bcast(&estrategy, 1, MPI_INT, 0, MPI_COMM_WORLD); //Le comunico a todos la estrategia del impresor
    Player* player = new Player(my_id);
    if(my_id ==printer) //si soy el impresor mi estrategia sera la seleccionada por el usuario
    {
        player->mario->printer = 1;
        player->setEstrategy((Estrategies) estrategy);
    }

    while(!finished) //mientras que el juego no haya terminado
    {
        
        //esta logica no la revise
        my_coins = player->mario->getCoins();
        MPI_Allgather(&my_coins, 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&enemy, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD); 
        amIalive = player->mario->isAlive();
        MPI_Allgather(&amIalive, 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&myKoopas, 1 , MPI_INT , koopatroopas , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allreduce(&my_coins, &minCoins, 1, MPI_INT , MPI_MIN , MPI_COMM_WORLD);
        MPI_Allreduce(&my_coins, &maxCoins, 1, MPI_INT , MPI_MAX , MPI_COMM_WORLD);
        if(my_id == printer){ //checkeo que sigo siendo el impresor
            player->mario->printer = 1;
        }else{
            player->mario->printer = 0;
        }

        if(my_id!=0)
        {
            if(player->mario->world->getIndexPosition() > 99){ //si ya pasamos por la casilla final entonces refresquese
                player->mario->world->refreshWorld();
            }

            int action = 0;
            vector<int> elements;
            if(player->mario->world->getIndexPosition() ==0)
            {
                elements = player->mario->world->getPosition();
                cout<<"getPOsition\n";
            }
            else
            {
                cout<<"getNext"<<endl;
                elements = player->mario->world->getNext();
            }
            if(my_id == printer){
                cout << "Posicion en el mundo: " <<  player->mario->getLocation() << "." << "Mario esta caminando. Monedas: " << my_coins <<endl;
            }
            for( auto& element: elements)
            {   
                if(player->mario->isAlive())
                {
                    if(my_id == printer){
                        cout << "Posicion en el mundo: " <<  player->mario->getLocation() << ".";
                    }
                    action = player->mario->chooseAction((Elements)element);
                    
                    if(action==1) //significa que mario brinco y agarro la moneda
                    {
                        player->mario->addCoins();
                        my_coins=player->mario->getCoins();
                        if(my_id == printer){
                            //cout << "Posicion en el mundo: " <<  player->mario->getLocation() << "." << "Mario ha brincado y ha agarrado una moneda! Monedas: " << my_coins << endl;
                        }
                    }
                    else if(action==2)//significa que mario brinco y mato a un enemigo
                    {
                        if(estrategy==R){
                            enemy = activePlayers[rand()%1+(num_processes-1)];// va de 1 a numero de procesos-1
                            if(element==Goomba){
                                goombas[enemy]++;
                                if(my_id == printer){
                                    cout<< ". He enviado un goomba al proceso " << enemy << ".";
                                }
                            }else{
                                koopatroopas[enemy]++;
                                if(my_id == printer){
                                    cout<< ". He enviado un koopatroopa al proceso " << enemy << ".";
                                }
                            }
                            if(my_id == printer){
                                //cout << "Posicion en el mundo: " <<  player->mario->getLocation() << "." << "Mario ha brincado y ha agarrado una moneda! Monedas: " << my_coins << endl;
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
                                    if(my_id == printer){
                                        cout<< ". He enviado un goomba al proceso " << enemy << ".";
                                    }
                                }
                                else{
                                    koopatroopas[enemy]++;
                                    if(my_id == printer){
                                        cout<< ". He enviado un Koopatroopa al proceso " << enemy << ".";
                                    }
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
                                        if(my_id == printer){
                                            cout<< ". He enviado un Goomba al proceso " << enemy << ".";
                                        }
                                    } else {
                                        koopatroopas[enemy]++;  
                                        if(my_id == printer){
                                            cout<< ". He enviado un Koopatroopa al proceso " << enemy << ".";
                                        }  
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
                                        if(my_id == printer){
                                            cout<< ". He enviado un Koopatroopa al proceso " << enemy << ".";
                                        }
                                    } else {
                                        koopatroopas[enemy]++;
                                        if(my_id == printer){
                                            cout<< ". He enviado un Koopatroopa al proceso " << enemy << ".";
                                        }
                                    }
                                    break;// termina ciclo para que no haga mas de un ataque
                                }                                
                            }
                        }
                    }
                    if(my_id == printer){
                        cout << "Coins: " << my_coins << endl;
                    }
                }
            }
        }
        MPI_Allgather(&amIalive, 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
        if(my_id==0)
        {
            if(activePlayers[printer] == 0){
                
                printer = choose_new_printer(num_processes, activePlayers);
            }
            finished = game_finished(activePlayers, num_processes);               
        }
        MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&finished, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
        
    cout << "El juego ha terminado"<< endl;
    MPI_Finalize();
    return 0;
}

