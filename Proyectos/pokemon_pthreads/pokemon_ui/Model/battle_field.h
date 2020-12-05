#ifndef BATTLE_FIELD_H
#define BATTLE_FIELD_H
#include "../Model/player.h"
#include "../Model/pokemon.h"


typedef struct side
{
    pokemon_t* pokemon;
    player_t* player;
    pthread_mutex_t* mutex_array;
    
}side_t;

typedef struct  battleinfo{
    double left_attack;
    double right_attack;
    double left_health;
    double right_health;
    int left_id;
    int right_id;
    
}battle_info_t;

typedef struct battle_field
{
    battle_info_t* battle_info;
    side_t* left_side;
    side_t* right_side;
    pthread_barrier_t barrier;
    pthread_barrier_t barrier_control;
    pthread_mutex_t* mutex;
    int equipoRocket;
}battle_field_t;


battle_field_t* battle_field_create();
void battle_field_destroy(battle_field_t* battle_field);

#endif