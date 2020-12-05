#ifndef POKEMON_H
#define POKEMON_H
#include "pthread.h"
#include "../Model/mapper.h"

typedef struct charged_move
{
    int id; // move id
    int cooldown;
    char* moveName;
    int energy;    
    int power;
    int typeId;
} charged_move_t;

typedef struct fast_move
{
    int id; // move id
    int cooldown;
    char* moveName;
    int energyGain;
    int power;
    int typeId;

} fast_move_t;

typedef struct type
{
    int id;
    char *typeName;
} type_t;

typedef struct pokemon
{
    int id;
    char* species_name;
    double hp; // life points
    fast_move_t* fast_move;//int fast_move;
    charged_move_t* charged_move;//int charged_move;
    type_t* type;
    int order;
    int energy;
    double time; 
    int side; 
    int oponent;// oponent id
    int* oponents;
    int oponent_type;
    int isActive;
    

}pokemon_t;

typedef struct pokemon_data
{
    pokemon_t* pokemon;
    pokemon_t** left;
    pokemon_t** right; 
    pthread_barrier_t* barrier;

}pokemon_data_t;

pokemon_t* pokemon_create(int id); 
void pokemon_destroy(pokemon_t* pokemon);
int callNext( pokemon_t* pokemon);// call the next pokemon to the battle 
int isActive(pokemon_t* pokemon);
int isWeak(pokemon_t* pokemon);
void* fight(void* args);
double calculate_damage(int power, double efectiveness, double bonus);
double calculate_efectiveness(int targetType, int attackType);
void setOrder(pokemon_t*,int);

#endif