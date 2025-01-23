#ifndef _CARENA_H
#define _CARENA_H

// to access the functions' bodies use
// #include CARENA_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifndef ALLOCATOR
	#define ALLOCATOR
	typedef void* (*Allocator) (size_t);
    typedef void  (*Deallocator) (void*);
#endif // ALLOCATOR

#define CARENA_REGION_CAPACITY  32

#define carena_error(msg, ...) (fprintf(stderr, "%s:%d [ERROR]" (msg)"\n", __FILE__, __LINE__))
#define carena_assert(state, msg, ...) do{if(!(state)){carena_error((msg), __VA_ARGS__); exit(1);}}while(0)

typedef struct Carena Carena;
typedef struct CarenaRegion CarenaRegion;

struct Carena{
    CarenaRegion *first, *last;
};

struct CarenaRegion{
    size_t size;
    size_t capacity;
    CarenaRegion *next;
    uintptr_t data[];
};

void* carena_alloc(Carena *carena, size_t size);
void  carena_free(Carena *carena);

#endif // _CARENA_H

#ifdef CARENA_IMPLEMENTATION

CarenaRegion* carena_new_region(size_t capacity)
{
    // this currently uses malloc, maybe enable backend customization later
    size_t size = sizeof(CarenaRegion) + sizeof(uintptr_t)*capacity;
    CarenaRegion *region = malloc(size);
    carena_assert(region != NULL, "Failed to allocate new region!");
    region->size = 0;
    region->capacity = capacity;
    region->next = NULL;
    return region;
}

void* carena_alloc(Carena *arena, size_t size)
{
    if (arena == NULL) return NULL;
    size_t all_size = (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
    if (arena->first == NULL){
        carena_assert(arena->last == NULL, "Invalid arena state!: first:%p, last:%p", arena->first, arena->last);
        size_t capacity = (all_size > CARENA_REGION_CAPACITY)? all_size:CARENA_REGION_CAPACITY;
        CarenaRegion *region = carena_new_region(capacity);
        arena->first = region;
        arena->last = region;
    }
    CarenaRegion *last = arena->last;
    if (last->size + all_size > last->capacity){
        carena_assert(arena->last == NULL, "Invalid arena state!: size:%zu, capacity:%zu, next:%p", last->size, last->capacity, last->next);
        size_t capacity = (all_size > CARENA_REGION_CAPACITY)? all_size:CARENA_REGION_CAPACITY;
        last->next = carena_new_region(capacity);
        arena->last = last->next;
    }
    void *result = &arena->last->data[arena->last->size];
    arena->last->size += all_size;
    return result;
}

void carena_free(Carena *arena)
{
    if (arena == NULL) return;
    CarenaRegion *region = arena->first;
    while (region != NULL){
        CarenaRegion *temp = region;
        region = temp->next;
        free(temp);
    }
    arena->first = NULL
    arena->last = NULL;
}

#endif // CARENA_IMPLEMENTATION