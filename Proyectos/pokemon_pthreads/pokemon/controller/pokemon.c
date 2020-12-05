#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../Model/pokemon.h"
#include "../Model/battle_field.h"
#include "../Model/referee.h"
#define HP 1500
#define BONUS 2.3


pokemon_t* pokemon_create(int id)
{

    pokemon_t* pokemon = calloc(1,sizeof(pokemon_t));
    pokemon->id = id;
    pokemon->species_name = get_pokemon_species_name(id);
    pokemon->fast_move = calloc(1, sizeof(fast_move_t));
    pokemon->charged_move = calloc(1,sizeof(charged_move_t));
    pokemon->fast_move->id = get_pokemon_fast_move_id(id);
    pokemon->charged_move->id = get_pokemon_charged_move_id(id);
    pokemon->type = calloc(1, sizeof(type_t));
    pokemon->type->id = get_pokemon_type_id(id);
    pokemon->type->typeName = get_type_name(id);
    pokemon->isActive = 0;
    pokemon->hp = 1500;
    pokemon->charged_move->power = get_move_power(pokemon->charged_move->id);
    pokemon->fast_move->power = get_move_power(pokemon->fast_move->id);
    pokemon->charged_move->energy = get_move_energy(pokemon->charged_move->id);
    pokemon->fast_move->energyGain = get_move_energy_gain(pokemon->fast_move->id);
    pokemon->fast_move->cooldown = get_move_cooldown(pokemon->fast_move->id);
    pokemon->charged_move->cooldown = get_move_cooldown(pokemon->charged_move->id);
    pokemon->fast_move->power = get_move_power(pokemon->fast_move->id);
    pokemon->charged_move->moveName = get_move_name(pokemon->charged_move->id);
    pokemon->fast_move->moveName = get_move_name(pokemon->fast_move->id);
    pokemon->charged_move->typeId = get_move_type_id(pokemon->charged_move->id);
    pokemon->fast_move->typeId = get_move_type_id(pokemon->fast_move->id);
    
    return pokemon;
}

void pokemon_destroy(pokemon_t* pokemon)
{
    free(pokemon);
}


int callNext( pokemon_t* pokemon)// call the next pokemon to the battle
{
    int isTeamWeak = 0;
    int side = pokemon->side;
    if(pokemon->order == 2){
        isTeamWeak = 0;
        return isTeamWeak;
    }else{
        if(pokemon->side == 0){
            pthread_mutex_unlock(&battle_field->left_side->mutex_array[pokemon->order+1]);
            battle_field->left_side->player->pokemon[pokemon->order+1]->isActive = 1;
            pthread_mutex_unlock(&battle_field->left_side->mutex_array[pokemon->order]);
            pokemon->isActive = 0;

        }else{
            pthread_mutex_unlock(&battle_field->right_side->mutex_array[pokemon->order+1]);
            battle_field->right_side->player->pokemon[pokemon->order+1]->isActive = 1;
            pthread_mutex_unlock(&battle_field->right_side->mutex_array[pokemon->order]);
            pokemon->isActive = 0;
        }
        return 1;
    }    

}

int isActive(pokemon_t* pokemon) // Pregunta si el pokemon esta activo
{
    int isActive= 0;
    if(!isWeak(pokemon) && pokemon->isActive==1)
    {
        isActive = 1;
    }
    return isActive;
}

int isWeak(pokemon_t* pokemon)
{
    int isWeak =0;
    if(pokemon->hp<=0)
    {
        isWeak = 1;
    }
    return isWeak;
}

double calculate_damage(int power, double efectiveness, double bonus)
{
    double damage = 0;
    //printf("power = %d * efectiveness =%f * bonus = %f\n", power,efectiveness,bonus);
    damage = floor(power*efectiveness*BONUS)+1;
    return damage;
}

double calculate_efectiveness(int targetType, int attackType)
{
    if(weaknesses_matrix[targetType][attackType])
    {
        return 1.6;
    }    
    else if(resistances_matrix[targetType][attackType])
    {
        return 0.625;
    }
    else if(immunities_matrix[targetType][attackType])
    {
        return 0.390625;
    }    
    else 
        return 1;
}



void setOrder(pokemon_t* pokemon,int order)
{
    pokemon->order = order;
}


// Metodo principal que se llama cuando se crean los hilos
void* fight(void* args)
{
    walltime_t timer;
    walltime_start(&timer);
    
    pokemon_data_t* data = (pokemon_data_t*) args;
    pokemon_t* pokemon = data->pokemon;
    int side = pokemon->side;
    double oponent_health = 0;
    // stop pokemon
    pthread_barrier_wait(data->barrier); //Cuando se creen los pokemones pasarle por referencia el barrier
    
    if (side == 0){
        pthread_mutex_lock(&battle_field->left_side->mutex_array[pokemon->order]);
        battle_field->battle_info->left_health = HP;
    }else{
        pthread_mutex_lock(&battle_field->right_side->mutex_array[pokemon->order]);
        battle_field->battle_info->right_health = HP;
    }
    
    
    // pokemon order
    //mutex y variable de condicion?
    double damage = 0;
    double efectiveness = 0;
    int oponent_id = 0;
    int oponent_type = 0;

    walltime_elapsed(&timer);
    printf("El pokemon %s esta listo para pelear \n", pokemon->species_name);
    if( side == 0 )
    {
        while(!(oponent_health < 0) && !(battle_field->equipoRocket))
        {
             //Si estamos en la izquierda
            while(!isWeak(pokemon) && !(battle_field->equipoRocket))
            {
                battle_field->battle_info->left_id = pokemon->id;
                oponent_id = battle_field->battle_info->right_id;
                oponent_type = get_pokemon_type_id(pokemon->oponents[oponent_id]);
                if(pokemon->energy >= pokemon->charged_move->energy)
                {
                    //sinc
                    fixed_sleep((useconds_t)1000);
                    //sinc
                    
                    efectiveness = calculate_efectiveness(oponent_type, pokemon->charged_move->typeId);
                    double damage = calculate_damage(pokemon->charged_move->power, efectiveness,BONUS);
                    pthread_mutex_lock(&battle_field->mutex[1]);
                    battle_field->battle_info->right_health -= calculate_damage(pokemon->charged_move->power, efectiveness,BONUS);
                    oponent_health = battle_field->battle_info->right_health;
                    pthread_mutex_unlock(&battle_field->mutex[1]);
                    pokemon->energy -= pokemon->charged_move->energy;
                    pthread_mutex_lock(&battle_field->mutex[0]);
                    printf("El pokemon %s acaba de hacer un ataque cargado, su salud es %f\n", pokemon->species_name,  battle_field->battle_info->left_health);
                    pokemon->hp = battle_field->battle_info->left_health;
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                    fixed_sleep((useconds_t)pokemon->charged_move->cooldown);
        
                }
                else
                {
                    //sinc
                    fixed_sleep((useconds_t)1000);
                    //sinc
                    efectiveness = calculate_efectiveness(oponent_type, pokemon->fast_move->typeId);
                    pthread_mutex_lock(&battle_field->mutex[1]);
                    battle_field->battle_info->right_health -= calculate_damage(pokemon->fast_move->power, efectiveness,BONUS);
                    oponent_health = battle_field->battle_info->right_health;
                    pthread_mutex_unlock(&battle_field->mutex[1]);
                    pokemon->energy += pokemon->fast_move->energyGain;
                    printf("El pokemon %s acaba de hacer un ataque normal, su salud es %f\n", pokemon->species_name, pokemon->hp);
                    pthread_mutex_lock(&battle_field->mutex[0]);
                    pokemon->hp = battle_field->battle_info->left_health;
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                    fixed_sleep((useconds_t)pokemon->charged_move->cooldown);
                }
                pthread_mutex_lock(&battle_field->mutex[0]);
                if(oponent_health <=0 && battle_field->equipoRocket){
                    pokemon->time = walltime_elapsed(&timer);   
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                    pthread_exit(pokemon);
                }    
                pthread_mutex_unlock(&battle_field->mutex[0]);
            }
            
            if(is_team_weak(data->right)){
                pthread_mutex_lock(&battle_field->mutex[0]);
                battle_field->equipoRocket =1;
                pthread_mutex_unlock(&battle_field->mutex[0]);
                printf("***********equipo1 se debilito\n");
                return args;
            }else if(isWeak(pokemon)){
                
                printf("El pokemon %s esta debilitado\n", pokemon->species_name);
                if(pokemon->order<2){
                    callNext(pokemon);
                }
                pokemon->time = walltime_elapsed(&timer);   
                if(pokemon->order == 2){
                    pthread_mutex_lock(&battle_field->mutex[0]);
                    printf("EQUIPO ROCKEt\n");
                    battle_field->equipoRocket =1;
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                }
                pthread_exit(pokemon);
            }
            if(pokemon->hp < 0){
                pthread_exit(pokemon);
            }
        }
    }
    else
    { //si estamos en la derecha
        while(!(oponent_health <0) && !(battle_field->equipoRocket))
        {
            while(!isWeak(pokemon) && !(battle_field->equipoRocket))
            {
                battle_field->battle_info->right_id = pokemon->id;
                oponent_id = battle_field->battle_info->left_id;
                oponent_type = get_pokemon_type_id(pokemon->oponents[oponent_id]);
                
                if(pokemon->energy >= pokemon->charged_move->energy)
                {
                    //sinc
                    fixed_sleep((useconds_t)1000);
                    //sinc
                    efectiveness = calculate_efectiveness(oponent_type, pokemon->charged_move->typeId);
                    pthread_mutex_lock(&battle_field->mutex[0]);
                    battle_field->battle_info->left_health -= calculate_damage(pokemon->charged_move->power, efectiveness,BONUS);
                    oponent_health = battle_field->battle_info->left_health;
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                    pokemon->energy -= pokemon->charged_move->energy;
                    printf("El pokemon %s acaba de hacer un ataque cargado, su salud es %f\n", pokemon->species_name, pokemon->hp);
                    pthread_mutex_lock(&battle_field->mutex[1]);
                    pokemon->hp = battle_field->battle_info->right_health;
                    pthread_mutex_unlock(&battle_field->mutex[1]);
                    fixed_sleep((useconds_t)pokemon->charged_move->cooldown);
                }
                else
                {
                    //sinc
                    fixed_sleep((useconds_t)1000);
                    //sinc
                    efectiveness = calculate_efectiveness(oponent_type, pokemon->fast_move->typeId);
                    pthread_mutex_lock(&battle_field->mutex[0]);
                    battle_field->battle_info->left_health -= calculate_damage(pokemon->fast_move->power, efectiveness,BONUS);
                    oponent_health = battle_field->battle_info->left_health;
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                    pokemon->energy += pokemon->fast_move->energyGain;
                    printf("El pokemon %s acaba de hacer un ataque normal, su salud es %f\n", pokemon->species_name, pokemon->hp);
                    pthread_mutex_lock(&battle_field->mutex[1]);
                    pokemon->hp = battle_field->battle_info->right_health;
                    pthread_mutex_unlock(&battle_field->mutex[1]);
                    fixed_sleep((useconds_t)pokemon->charged_move->cooldown);
                }
                pthread_mutex_lock(&battle_field->mutex[0]);
                if(oponent_health <=0 && battle_field->equipoRocket){
                    pokemon->time = walltime_elapsed(&timer);   
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                    pthread_exit(pokemon);
                }
                pthread_mutex_unlock(&battle_field->mutex[0]);
            }
            if(is_team_weak(data->right)){
                pthread_mutex_lock(&battle_field->mutex[0]);
                battle_field->equipoRocket = 1;
                pthread_mutex_unlock(&battle_field->mutex[0]);
                printf("************equipo2 se debilito\n");
                pokemon->time = walltime_elapsed(&timer);   
                pthread_exit(pokemon);
            }else if(isWeak(pokemon)){
                printf("El pokemon %s esta debilitado\n", pokemon->species_name);
                if(pokemon->order<2){
                    callNext(pokemon);
                }
                pokemon->time = walltime_elapsed(&timer);   
                if(pokemon->order ==2){
                    printf("EQUIPO ROCKEt\n");
                    pthread_mutex_lock(&battle_field->mutex[0]);
                    battle_field->equipoRocket =1;
                    pthread_mutex_unlock(&battle_field->mutex[0]);
                    pthread_exit(pokemon); 
                }
            }
            if(pokemon->hp < 0){
                pthread_exit(pokemon);
            }
        }
    }
}

