#ifndef _CARENA_H
#define _CARENA_H

// to access the functions' bodies use
// #include CARENA_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>

#define CARENA_STATUS_FREE    "FREE"
#define CARENA_STATUS_FULL    "FULL"

typedef struct{
    void *arena;
    size_t size;
    size_t capacity;
} Carena;

Carena* Carena_init();
void    Carena_free(Carena *carena);
void*   Carena_alloc(Carena *carena, size_t size);
void    Carena_status(Carena *carena);

#ifdef CARENA_IMPLEMENTATION

Carena* Carena_init(size_t capacity)
{
    Carena *carena = (Carena*) calloc(1, sizeof(*carena));
    if (carena != NULL){
        void *arena = calloc(1, capacity);
        if (arena == NULL){
            free(carena);
            return NULL;
        }
        carena->arena = arena;
        carena->size = 0;
        carena->capacity = capacity;
    }
}

void Carena_free(Carena *carena)
{
    if (carena != NULL){
        free(carena->arena);
        free(carena);
    }
}

void* Carena_alloc(Carena *carena, size_t size)
{
    if (carena == NULL || size == 0 || carena->size + size >= carena->capacity) return NULL;
    carena->size += size;
    return carena->arena + carena->size;
}

void Carena_status(Carena *carena)
{
    if (carena == NULL) return;
    printf("Carena: {size: %d, capacity: %d} => %d%c filled => \"%s\"\n", carena->size, carena->capacity, (int)(carena->size*100/carena->capacity), '%',(carena->size < carena->capacity)? CARENA_STATUS_FREE:CARENA_STATUS_FULL);
}
#endif // CARENA_IMPLEMENTATION

#endif // _CARENA_H