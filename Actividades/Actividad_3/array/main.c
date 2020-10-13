#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "array.h"

#define MIN_CNT 10
#define MAX_CNT 20

void print_array(const char* name, array_t* array)
{
	printf("%s =", name);
	for ( size_t index = 0; index < array_get_count(array); ++index )
		printf(" %zu", (size_t)array_get_element(array, index));
	putchar('\n');
	fflush(stdout);
}

int main()
{
	array_t* array1 = array_create(MAX_CNT);
	array_t* array2 = array_create(MAX_CNT);

	srand( (unsigned int)((unsigned long)time(NULL) + clock()) );
	for ( size_t index = 0, count = MIN_CNT + rand() % MAX_CNT; index < count; ++index )
	{
		array_append( array1, (void*)((size_t)rand() % 100) );
		array_append( array2, (void*)((size_t)rand() % 1000) );
	}

	print_array("array1", array1);
	print_array("array2", array2);

	array_destroy(array1);
	array_destroy(array2);

	return 0;
}