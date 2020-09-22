#include <assert.h>
#include <stdlib.h>

#include "array.h"


typedef struct array
{
	void** elements;
	size_t capacity;
	size_t count;
	pthread_mutex_t mutex;
} array_t;


array_t* array_create(size_t capacity)
{
	assert(capacity);
	pthread_mutex_init(&shared_data->mutexes[i], /*attr*/ NULL);
	array_t* array = calloc(1, sizeof(array_t));
	if ( array == NULL )
		return NULL;

	array->capacity = capacity;
	array->count = 0;

	array->elements = (void**)malloc( capacity * sizeof(void*) );
	if ( array->elements == NULL )
		return free(array), NULL;

	return array;
}

void array_destroy(array_t* array)
{
	assert(array);

	free(array->elements);
	free(array);
	pthread_mutex_destroy(&mutex);
}

int array_increase_capacity(array_t* array)
{

	//Como no estan definidos en el .h consideramos qe son metodos privados y no hay que cambiarlos
	assert(array);

	size_t new_capacity = 10 * array->capacity;
	void** new_elements = (void**)realloc( array->elements, new_capacity 
			* sizeof(void*) );
	if ( new_elements == NULL )
		return -1;

	array->capacity = new_capacity;
	array->elements = new_elements;

	return 0; // Success
}

int array_decrease_capacity(array_t* array)
{
	assert(array);

	size_t new_capacity = array->capacity / 10;
	if ( new_capacity < 10 )
		return 0;

	void** new_elements = (void**)realloc( array->elements, new_capacity * sizeof(void*) );
	if ( new_elements == NULL )
		return -1;

	array->capacity = new_capacity;
	array->elements = new_elements;

	return 0; // Success
}

size_t array_get_count(array_t* array)
{
	assert(array);

	pthread_mutex_lock(&mutex);
	size_t count = array->count;
	pthread_mutex_unlock(&mutex);

	return count;
}

void* array_get_element(array_t* array, size_t index)
{
	assert(array);
	assert( index < array_get_count(array) );

	pthread_mutex_lock(&mutex); 
	void* element = array->elements[index];
	pthread_mutex_unlock(&mutex); //El assert no sale del metodo y por tanto no deja el mutex bloqueado

	return element;
}

int array_append(array_t* array, void* element)
{
	assert(array);
	pthread_mutex_lock(&mutex);
	if ( array->count == array->capacity ){
		if ( array_increase_capacity(array) ){
			pthread_mutex_unlock(&mutex);
			return -1;
		}
	}
	assert( array->count < array->capacity );
	array->elements[array->count++] = element;
	pthread_mutex_unlock(&mutex);

	return 0; // Success
}

size_t array_find_first(array_t* array, const void* element, size_t start_pos)
{
	assert( array );
	pthread_mutex_lock(&mutex);

	for ( size_t index = start_pos; index < array->count; ++index ){
		if ( array->elements[index] == element ){
			pthread_mutex_unlock(&mutex);
			return index;
		}
	}
	pthread_mutex_unlock(&mutex);
	return array_not_found;
}

int array_remove_first(array_t* array, const void* element, size_t start_pos)
{
	assert( array );
	size_t index = array_find_first(array, element, start_pos);
	pthread_mutex_lock(&mutex);
	if ( index == array_not_found ){
		pthread_mutex_unlock(&mutex);
		return -1;
	}
	for ( --array->count; index < array->count; ++index )
		array->elements[index] = array->elements[index + 1];
	if ( array->count == array->capacity / 10 )
		array_decrease_capacity(array);
	pthread_mutex_unlock(&mutex);
	return 0; // Removed
}