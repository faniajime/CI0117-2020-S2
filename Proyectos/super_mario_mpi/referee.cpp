#include <iostream>
#include <string>
#include "mpi.h"
#include "Mario.h"
#include "Player.h"
#include "world.h"
#include <vector>
#include <unistd.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <cstring>
using namespace std;



int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    int my_id, num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Status status;
    srand((unsigned) time(NULL)*my_id*1000);

    //Se inicializan los buffers
    int coins[num_processes], attacking[num_processes], activePlayers[num_processes], goombas[num_processes], koopatroopas[num_processes], prints[num_processes]  = {0};

    //Se inicializan las variables necesarias
    int printer, finished, alive,my_coins, mygoombas, myKoopas, estrategy = 0;
    int recgoombas, reckoopas = 0;
    int maxCoins =  -1;
    int minCoins = 10000;
    int amIalive = 1; //ESTOY VIVO?????? 
    int enemy = rand()%num_processes+1; //Se inicializa en random y luego se cambia

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
        if(printer<=0 || printer > num_processes){
            cout << "El numero de proceso ingresado no es valido \n" << endl;
            return 1;
        }else{
            prints[printer] = 1;
        }
    }
    //Comunicamos a todos quien imprime y su estrategia
    MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD); //Le comunico a todos quien imprimira
    MPI_Bcast(&estrategy, 1, MPI_INT, 0, MPI_COMM_WORLD); //Le comunico a todos la estrategia del impresor
    
    //Creamos un jugador por proceso
    Player* player = new Player(my_id);
    if(my_id ==printer) //si soy el impresor mi estrategia sera la seleccionada por el usuario
    {
        player->setEstrategy((Estrategies) estrategy);
    }


    while(!finished) //Este ciclo se ejecuta mientras que el juego no haya terminado
    {

        //Hacemos las comunicaciones entre todos los procesos para que sepan el estado de todas las variables importantes de cada proceso
        my_coins = player->mario->getCoins();
        MPI_Allgather(&my_coins, 1 , MPI_INT , coins , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&enemy, 1 , MPI_INT , attacking , 1 , MPI_INT , MPI_COMM_WORLD); 
        amIalive = player->mario->isAlive();
        MPI_Allgather(&amIalive, 1 , MPI_INT , activePlayers , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&mygoombas, 1 , MPI_INT , goombas , 1 , MPI_INT , MPI_COMM_WORLD);
        MPI_Allgather(&myKoopas, 1 , MPI_INT , koopatroopas , 1 , MPI_INT , MPI_COMM_WORLD);

        
        //Se agregan al mundo todos los goombas y koopas que le enviaron
        if(player != 0){
            for(int i =0 ; i < num_processes;i++){
                if(attacking[i] == my_id){
                    recgoombas += goombas[i];
                    reckoopas += koopatroopas[i];
                } 
            }
            player->mario->world->addElement(recgoombas, 2);
            player->mario->world->addElement(reckoopas, 1);
            mygoombas, myKoopas, reckoopas, recgoombas=0;
        }
    
        
        //El hilo cero comprueba que el juego no haya terminado
        if(my_id == 0){ 
            alive = 0;
            for(int i = 1; i<num_processes; i++){
                if(activePlayers[i]){
                    alive+=1;
                }
            }
            if(alive<2){
                finished = 1;
                amIalive = 0;
            }
            else{
                finished = 0;
            }
        }
        MPI_Bcast(&finished, 1, MPI_INT, 0, MPI_COMM_WORLD);

        //En caso de que el impresor se haya muerto, se selecciona uno nuevo
        if(my_id==0 && !finished){
            if(!activePlayers[printer])
            {
                cout << "Oh no, su mario murio, escoja un nuevo jugador al que observar, estas son sus opciones:";
                for(int i =1; i<num_processes;++i)
                {

                    if(activePlayers[i])
                    {
                        cout<<i<<", ";
                    }
                }
                cout <<endl;
                if(!finished){
                    cin>>printer;
                }
                
                if(printer<=0 || printer >= num_processes-1){
                    cout << "El numero de proceso ingresado no es valido \n" << endl;
                    return 1;
                }
                if(!activePlayers[printer]){
                    cout << "El mario que escogio no esta vivo, el programa terminara.\n" << endl;
                    return 1; 
                }
            }
        }             
        MPI_Bcast(&printer, 1, MPI_INT, 0, MPI_COMM_WORLD);
        player->mario->printer = printer;

        //Si soy el comunicador, imprimo todas mis variables en pantalla
        if(my_id == printer){
            cout << "Posicion en el mundo: " <<  player->mario->getLocation() << "." << "Mario # " << my_id<< ". ";
        }

        //Aca se toman acciones y se camina por el mundo
        if(my_id != 0 && !finished)
        {
            //Si ya llegamos al final del mundo entonces volvemos a empezar de 0
            if(player->mario->world->getIndexPosition() > 99){ //si ya pasamos por la casilla final entonces refresquese
                player->mario->world->refreshWorld();
            }
            
            int action = 0;
            vector<int> elements;
            elements = player->mario->world->getNext();

            //Se toman acciones para cada elemento del mundo en determinada posicion
            for(auto& element: elements)
            {   
                if(amIalive)
                {
                    //Se decide la accion de acuerdo al elemento
                    action = player->mario->chooseAction((Elements) element);
                    
                    if(action==1) //significa que mario brinco y agarro la moneda
                    {
                        player->mario->addCoins();
                        my_coins=player->mario->getCoins();
                    }
                    else if(action==2)//significa que mario brinco y mato a un enemigo
                    {
                        if(estrategy==R){
                            int foundEnemy = 0;
                            while(!foundEnemy){
                                enemy = rand()%num_processes+1;// va de 1 a numero de procesos-1
                                if(activePlayers[enemy]){
                                    foundEnemy = 1;
                                    if(element==Goomba){
                                        mygoombas++;
                                        if(my_id == printer){
                                            cout<< "He enviado un goomba al proceso " << enemy << ". ";
                                        }
                                    }else{
                                        myKoopas++;
                                        if(my_id == printer){
                                            cout<< "He enviado un koopatroopa al proceso " << enemy << ". ";
                                        }
                                    }
                                }
                            }
                            
                        }
                        else if (estrategy == L) //Si la estrategia es atacar al de menos coins
                        {
                            int minimum = 0;
                            minCoins = 1000000;
                            for(int i = 1; i<num_processes;i++){
                                if (coins[i]<minCoins && activePlayers[i] && i!=my_id){
                                    minCoins = coins[i];
                                    minimum = i;
                                }
                            }
                            if(minimum){
                                enemy = minimum;
                                if(element==Goomba){
                                    mygoombas++;
                                    if(my_id == printer){
                                        cout<< " He enviado un goomba al proceso " << enemy << ". ";
                                    }
                                }
                                else{
                                    myKoopas++;
                                    if(my_id == printer){
                                        cout<< " He enviado un Koopatroopa al proceso " << enemy << ".  ";
                                    }
                                }
                            }
                        }
                        else if (estrategy == M)// ataca al que tiene mas monedas
                        {                        
                            maxCoins = -1;
                            int maximum =0;
                            for(int i = 0; i>num_processes; i++){ //se lo manda al de mayor coins empezando a buscar por el ultimo
                                if (coins[i]> maxCoins && activePlayers[i] && i!=my_id){
                                    maxCoins = coins[i];
                                    maximum = i;
                                }                            }
                            if(maximum)
                            {
                                enemy = maximum; 
                                if(element==Goomba){
                                    mygoombas++;
                                    if(my_id == printer){
                                        cout<< "He enviado un Goomba al proceso " << enemy << ". ";
                                    }
                                } else {
                                    myKoopas++;  
                                    if(my_id == printer){
                                        cout<< " He enviado un Koopatroopa al proceso " << enemy << ".";
                                    }  
                                }           
                                
                            }
                        }else{// estrategia: atacar a mi atacante (ley del Talion)
                            int foundattacker = 0;
                            int foundenemy =0;
                            for(int i = 1;i<num_processes;++i)
                            {
                                if(attacking[i]==my_id && activePlayers[i]==1) //si un Mario me ataca y esta vivo, se convierte en mi enemigo 
                                {
                                    enemy = i;
                                    foundattacker = 1;
                                    if(element==Goomba){
                                        mygoombas++;
                                        if(my_id == printer){
                                            cout<< " He enviado un Koopatroopa al proceso " << enemy << ".";
                                        }
                                    } else {
                                        myKoopas++;
                                        if(my_id == printer){
                                            cout<< " He enviado un Koopatroopa al proceso " << enemy << ".";
                                        }
                                    }
                                    break;// termina ciclo para que no haga mas de un ataque
                                }                                
                            }
                            //Si ningun mario me esta atacando selecciono a uno de los vivos aleatoriamente
                            if(!foundattacker){
                                while(!foundenemy){
                                    enemy = rand()%num_processes+1;// va de 1 a numero de procesos-1
                                    if(activePlayers[enemy]){
                                        foundenemy =1;
                                        if(element==Goomba){
                                            mygoombas++;
                                            if(my_id == printer){
                                                cout<< "He enviado un goomba al proceso " << enemy << ". ";
                                            }
                                        }else{
                                            myKoopas++;
                                            if(my_id == printer){
                                                cout<< "He enviado un koopatroopa al proceso " << enemy << ". ";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                
                }
            }
        }
        //Colectamos informacion de si todos estan vivos, para imprimir las estadisticas del hilo
        MPI_Gather( &amIalive , 1 , MPI_INT , activePlayers , 1 , MPI_INT , printer , MPI_COMM_WORLD);
        
        //Aqui se imprimen todos los datos
        if(my_id == printer){
            alive = 0;
            int beingAttacked = 0;
            for(int i =1; i< num_processes; i++){
                if(attacking[i] == my_id){
                    beingAttacked = i;
                }                 
                if (activePlayers[i] == 1){
                    alive++;
                }
            }
            cout << "Monedas: " << my_coins << "| Atacando a: " << enemy << "| Siendo atacado por: "<< beingAttacked << "| Estrategia: " << estrategy << "| Total jugando: " << alive << "."<< endl;
        }
        player->mario->world->current_position++;
        sleep(1);
    }
    if(player->mario->isAlive() && !(my_id == 0)){
        cout << "El juego ha terminado. El ganador es el proceso: " << my_id <<endl;
    }
    //El hilo 0 comunica a los demas que ya termino el juego e imprime game over
    MPI_Bcast( &finished , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
    if(my_id == 0){
        cout<<"------------------------------------------------------------\n";
        cout<<"|    **** ***  ** **  ****   **** *    *  ****  ****       |\n";
        cout<<"|    *    * *  * * *  *      *  *  *  *   *     *  *       |\n";
        cout<<"|    * ** ***  *   *  ***    *  *  *  *   ***   ****       |\n";
        cout<<"|    *  * * *  *   *  *      *  *  *  *   *     *   *      |\n";
        cout<<"|    **** * *  *   *  ****   ****    *    ****  *    *     |\n";
        cout<<"-----------------------------------------------------------\n";

    }
    MPI_Finalize();
    return 0;
}
