#ifndef PLAYER_H
#define PLAYER_H
#include "../Model/pokemon.h"

typedef struct player
{
    pokemon_t** pokemon; 
    int id;
    
}player_t;

player_t* team_create();
void destroy_player();
int* chooseTeam(player_t* player); //devuelve un arreglo con los ids de los pokemones
int is_team_weak(pokemon_t** pokemon);// all 3 pokemon are weak

#endif