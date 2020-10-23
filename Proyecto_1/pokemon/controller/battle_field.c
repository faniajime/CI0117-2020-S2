#include <stdlib.h>
#include <stdio.h>
#include "../Model/battle_field.h"
#include "../Model/pokemon.h"
#include "../Model/player.h"




battle_field_t* battle_field_create(){

    battle_field_t* battle_field = calloc(1, sizeof(battle_field_t));
    battle_field->left_side = calloc(1, sizeof(side_t)); //corroborar esto
    battle_field->right_side = calloc(1, sizeof(side_t)); //corroborar esto
    battle_field->battle_info = calloc(1, sizeof(battle_info_t));
    pthread_barrier_init(&battle_field->barrier, NULL, 6);
    pthread_barrier_init(&battle_field->barrier_control, NULL, 2);
    battle_field->mutex = (pthread_mutex_t*)calloc(2, sizeof(pthread_mutex_t));
    pthread_mutex_init(&battle_field->mutex[0], NULL);
    pthread_mutex_init(&battle_field->mutex[1], NULL);
    battle_field->left_side->mutex_array = (pthread_mutex_t*)calloc(3, sizeof(pthread_mutex_t));
    battle_field->right_side->mutex_array = (pthread_mutex_t*)calloc(3, sizeof(pthread_mutex_t));
    battle_field->equipoRocket = 0;
    for(int i = 0; i<3; ++i){
        pthread_mutex_init(&battle_field->left_side->mutex_array[i], NULL);
        pthread_mutex_lock(&battle_field->left_side->mutex_array[i]);
    }
    for(int i = 0; i<3; ++i){
        pthread_mutex_init(&battle_field->right_side->mutex_array[i], NULL);
        pthread_mutex_lock(&battle_field->right_side->mutex_array[i]);
    }
    return battle_field;
}

void battle_field_destroy(battle_field_t* battle_field){
    pthread_barrier_destroy(&battle_field->barrier);
    pthread_mutex_destroy(&battle_field->mutex[0]);
    pthread_mutex_destroy(&battle_field->mutex[1]);
    for(int i = 0; i<3; ++i){
        pthread_mutex_destroy(&battle_field->left_side->mutex_array[i]);
        
    }
    for(int i = 0; i<3; ++i){
        pthread_mutex_destroy(&battle_field->right_side->mutex_array[i]);
    }
    pthread_barrier_destroy(&battle_field->barrier_control);
    free(battle_field->left_side);
    free(battle_field->right_side);
    free(battle_field);
}