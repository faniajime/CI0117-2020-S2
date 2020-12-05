#include <stdio.h>
#include <stdlib.h>
#include "../Model/player.h"

player_t* team_create()
{
    player_t* player = calloc(1,sizeof(player_t));
    return player;
}

void destroy_player(player_t* player)
{
    free(player->pokemon);
    free(player);
}

int* chooseTeam(player_t* player){
    int* pokemons_id;
    int id;
    printf("Bienvenido jugador %d, escoja sus pokemones en el orden en el que pelearan \n", player->id);
    for( int i =0; i<3; i++){
        int created = 0;
        while(!created){
            scanf("%d", &id);
            if( id>=0 && id <=50){
                created = 1;
                pokemons_id[i] = id;
            }
            else{
                printf("El valor insertado es incorrecto, por favor escribalo de nuevo \n");
            }
        }        
    }
    return pokemons_id;
}



int is_team_weak(pokemon_t** pokemon)// all 3 pokemon are weak
{
    int weak = 0;
    //if(isWeak(pokemon[0]) && isWeak(pokemon[1]) && isWeak(pokemon[2]) )
    if(pokemon[0]->hp<=0 && pokemon[1]->hp<=0 && pokemon[2]->hp<=0 )
    {
        weak = 1;
    }
    return weak;
}