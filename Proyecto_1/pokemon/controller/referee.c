#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../Model/mapper.h"
#include "../Model/pokemon.h"
#include "../Model/player.h"
#include "../Model/battle_field.h"
#include <pthread.h>
#include <stdlib.h>

battle_field_t* battle_field;
typedef struct timespec walltime_t;

void walltime_start(walltime_t* start)
{
	clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start)
{
	walltime_t finish;
	clock_gettime(CLOCK_MONOTONIC, &finish);
	double elapsed = (finish.tv_sec - start->tv_sec);
	elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;
	return elapsed;
}

void fixed_sleep(useconds_t duration)
{
	usleep( 1000 * duration );
}


void startMapper(){
    initialize_data();
}

void showResults(battle_field_t* battle_field, double time){
    if(is_team_weak(battle_field->right_side->player->pokemon)){
        printf("El ganador es el jugador 2\n");
        for( int i =0; i<3; i++){
            printf("Su pokemon %s tardo %f segundos en juego\n", battle_field->right_side->player->pokemon[i]->species_name, battle_field->right_side->player->pokemon[i]->time);
        }
        printf("El perdedor fue el jugador 1\n");
        for( int i =0; i<3; i++){
            printf("Su pokemon %s tardo %f segundos en juego\n", battle_field->left_side->player->pokemon[i]->species_name, battle_field->left_side->player->pokemon[i]->time);
        }

    }else{
        printf("El ganador es el jugador 1\n");
        for( int i =0; i<3; i++){
            printf("El pokemon %s tardo %f segundos en juego\n", battle_field->left_side->player->pokemon[i]->species_name, battle_field->left_side->player->pokemon[i]->time);
        } 
        printf("El perdedor fue el jugador 2\n");
        for( int i =0; i<3; i++){
            printf("Su pokemon %s tardo %f segundos en juego\n", battle_field->right_side->player->pokemon[i]->species_name, battle_field->right_side->player->pokemon[i]->time);
        }
    }
    printf("La batalla tardo un total de %f segundos\n", time);

} // show winner and statistics

void endBattle(battle_field_t* battle_field){

    destroy_player(battle_field->left_side->player);
    destroy_player(battle_field->right_side->player);
    battle_field_destroy(battle_field);

} // destroy threads

void startPlayers(battle_field_t* battle_field){

    player_t* player_1 = team_create();
    player_t* player_2 = team_create();
    player_1->id = 1;
    player_2->id = 2;
    battle_field->left_side->player = player_1;
    battle_field->right_side->player = player_2;
}

//Start battle es nuestro metodo principal 
void main(void* args){
    printf("Bienvenido a las batallas pokemon \n");
    
    //Se inicia el timer 
    walltime_t timer;
    walltime_start(&timer);
    //Insertar el metodo de timer
    double time = 0;
    startMapper();
    battle_field = battle_field_create();
    startPlayers(battle_field);
    
    //inicializar las estructuras de dato
 
    battle_field->left_side->player->pokemon = calloc(3, sizeof(pokemon_t*));
    battle_field->right_side->player->pokemon = calloc(3, sizeof(pokemon_t*));
    battle_field->left_side->player->id = 1;
    battle_field->right_side->player->id = 2;
    
    int team_1 [3]= {21,2,3};
    int team_2 [3]= {18,10,6};

    //int* team_1 = chooseTeam(battle_field->left_side->player);
    //int* team_2 = chooseTeam(battle_field->right_side->player);

    // Se desbloquean los mutexes de los dos primeros pokemones
    pthread_mutex_unlock(&battle_field->left_side->mutex_array[0]);
    pthread_mutex_unlock(&battle_field->right_side->mutex_array[0]);

    pokemon_data_t* pokemon_izquierda = malloc(3* sizeof(pokemon_data_t));    
    pokemon_data_t* pokemon_derecha = malloc(3 * sizeof(pokemon_data_t));
    pthread_t* pokemons_left = malloc(3 * sizeof(pthread_t));
    pthread_t* pokemons_right = malloc(3 * sizeof(pthread_t));
    
    //Logica de batalla y creacion de hilos
    for(int i = 0; i<3; i++){
        pokemon_izquierda[i].pokemon = pokemon_create(team_1[i]);
        battle_field->left_side->player->pokemon[i] = pokemon_izquierda[i].pokemon;
        pokemon_izquierda[i].barrier = &battle_field->barrier;
        pokemon_izquierda[i].left = battle_field->left_side->player->pokemon;
        pokemon_izquierda[i].right = battle_field->right_side->player->pokemon;
        pokemon_izquierda[i].pokemon->side = 0;
        pokemon_izquierda[i].pokemon->order = i;
        pokemon_izquierda[i].pokemon->oponent = 0;
        pokemon_izquierda[i].pokemon->oponents = team_2;
        if(i == 0){
            pokemon_izquierda[i].pokemon->isActive = 1;
        }
        pthread_create(&pokemons_left[i], NULL, fight, ((void*)&pokemon_izquierda[i]));

    }
    for(int i = 0; i<3; i++){
        pokemon_derecha[i].pokemon = pokemon_create(team_2[i]);
        battle_field->right_side->player->pokemon[i] = pokemon_derecha[i].pokemon;
        pokemon_derecha[i].barrier = &battle_field->barrier;
        pokemon_derecha[i].left = battle_field->left_side->player->pokemon;
        pokemon_derecha[i].right = battle_field->right_side->player->pokemon;
        pokemon_derecha[i].pokemon->side = 1;
        pokemon_derecha[i].pokemon->order = i;
        pokemon_derecha[i].pokemon->oponent = 0;
        pokemon_derecha[i].pokemon->oponents = team_1;
        if(i == 0){
            pokemon_derecha[i].pokemon->isActive = 1;
        }
        pthread_create(&pokemons_right[i], NULL, fight, (void*)&pokemon_derecha[i]);
    }
    for( int i =0; i<3; i++){
        pthread_join(pokemons_right[i], NULL);
    }
    for( int i = 0; i<3; i++){
        pthread_join(pokemons_left[i], NULL);
    }
    showResults(battle_field, walltime_elapsed(&timer));
    endBattle(battle_field);
    free(pokemon_derecha);
    free(pokemon_izquierda);

}



