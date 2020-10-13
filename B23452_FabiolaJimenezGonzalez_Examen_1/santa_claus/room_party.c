#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>


typedef enum {
    IN_ROOM,
    WAITING,
    NOT_IN_ROOM
} dean_state_t;


typedef struct
{
    size_t student_count;
    pthread_mutex_t student_modifier;
    pthread_mutex_t room_lock;
    pthread_mutex_t roomEmpty;
    dean_state_t dean;
    pthread_cond_t count_zero;

} room_t;

typedef struct
{
    room_t* room;
    size_t student_id;
} student_info_t;


void random_sleep(useconds_t min_milliseconds, useconds_t max_milliseconds)
{
	useconds_t duration = min_milliseconds;
	useconds_t range = max_milliseconds - min_milliseconds;
	if ( range > 0 )
		duration += rand() % range;
	usleep( 1000 * duration );
}


void* student_run(void* args)
{
    //se lee la informacion
    student_info_t* info = (student_info_t*) args;
    room_t* room = info->room;
    size_t student_id = info->student_id;
    //sleep random
    random_sleep(1000, 10000);

    //Entrar al cuarto
    pthread_mutex_lock(&room->room_lock); //Si el cuarto esta locked, se espera hasta que se pueda entrar
    pthread_mutex_lock(&room->student_modifier); //lock del modificador de student count
    
    ++room->student_count; //incremento de student count
    printf("Student %zu has entered the room. Capacity: %zu/%u    %s\n",student_id, room->student_count, 50, (room->student_count < 50)? "": "PARTY! Watch out for the Dean!");

    pthread_mutex_unlock(&room->student_modifier);
    pthread_mutex_unlock(&room->room_lock);
    
    //Se queda en el cuarto un rato

    //random sleep
    random_sleep(1000, 10000);
    
    //Salida del cuarto
    pthread_mutex_lock(&room->student_modifier);

    --room->student_count;
    printf("Student %zu has left the room. Capacity: %zu/%u   %s \n", student_id, room->student_count, 50, (room->dean == WAITING)? "RUN! The Dean has spotted the party!": "");
    
    //Si el dean está esperando y este es el ultimo estudiante, le avisa que ya no hay nadie.
    if(room->dean == WAITING && room->student_count == 0)
    {
        pthread_mutex_lock(&room->roomEmpty);
        pthread_cond_broadcast(&room->count_zero);
        pthread_mutex_unlock(&room->roomEmpty);
    }

    pthread_mutex_unlock(&room->student_modifier);

    return NULL;
}

void* dean_run(void* args)
{
    //se lee la informacion
    room_t* room = (room_t*) args;

    //random sleep
    random_sleep(3000, 10000);

    //Check para ver que se estén cumpliendo las reglas
    pthread_mutex_lock(&room->student_modifier);
    if(room->student_count >= 50 )
    {
        printf("\n\nTHE DEAN FOUND OUT ABOUT THE PARTY!\n\n");
        //se bloquea el cuarto
        room->dean = WAITING;

        //se desbloquea el modificador
        
        //se desbloquea el modificador para que los estudiantes salgan
        pthread_mutex_unlock(&room->student_modifier);
        pthread_cond_wait(&room->count_zero, &room->roomEmpty);
        //Se espera hasta que el ultimo estudiante salga

    }
    else
    {
        if (room->student_count == 0)
        {
            printf("\n\nThe Dean is searching the room....\n\n");
            pthread_mutex_lock(&room->room_lock);
            room->dean = IN_ROOM;
            pthread_mutex_unlock(&room->student_modifier);
            
        }
        else
        {
            printf("\n\nThe Dean checked the room but capacity is %zu/%u... not considered a party\n\n",room->student_count,50);
            pthread_mutex_unlock(&room->student_modifier);
        }
        
    }
    
    //Se desbloquea el cuarto si se estaba revisando/interrumpiendo una fiesta
    if (room->dean != NOT_IN_ROOM)
    {
        if(room->dean == IN_ROOM)
        {
            random_sleep(3000, 10000);
        }
        pthread_mutex_unlock(&room->room_lock);
        room->dean = NOT_IN_ROOM;
    }

    return NULL;
}


int main(void)
{
    srand( time(NULL) );

    //Crear los datos compartidos
    room_t* room = calloc(1, sizeof(room_t));
    room->dean = NOT_IN_ROOM;
    pthread_mutex_init(&room->room_lock,NULL);
    pthread_mutex_init(&room->student_modifier,NULL);
    pthread_mutex_init(&room->roomEmpty,NULL);
    pthread_cond_init(&room->count_zero, NULL);

    //Crear un arreglo de estudiantes
    pthread_t* student_list = (pthread_t*) malloc( (size_t) (100 * sizeof(pthread_t)) );
    student_info_t* student_data = (student_info_t*) malloc ((size_t) (100* sizeof(student_info_t)) );

    //Poner a correr todos los estudiantes
    for(size_t i = 0; i < 100; ++i)
    {
        student_data[i].room = room;
        student_data[i].student_id = i+1;
        pthread_create(&student_list[i], NULL, student_run, (void*) &student_data[i]);
    }

    //Poner a revisar al dean
    pthread_t dean;
    pthread_create(&dean, NULL, dean_run, (void*) room);

    //Join de los threads
    pthread_join(dean, NULL);
    for(size_t i = 0; i < 100; ++i)
    {
        pthread_join(student_list[i], NULL);
    }
    

    //Destroy de los mutex/semáforo
    pthread_mutex_destroy(&room->room_lock);
    pthread_mutex_destroy(&room->student_modifier);
    pthread_mutex_destroy(&room->roomEmpty);

    //Free de la memoria reservada
    free(room);
    free(student_data);
    free(student_list);
    return 0;
}