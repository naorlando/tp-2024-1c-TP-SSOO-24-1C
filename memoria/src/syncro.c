#include "syncro.h"

void initialize_mutexes() 
{
    pthread_mutex_init(&MUTEX_DICTIONARY_IOS, NULL);
}