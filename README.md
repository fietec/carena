# Documentation carena.h

**carena** or c-arena is a lightweight and (overly) simplistic arena allocator in c.

## How to install 

1. Download the header file or clone the repository
2. Once for every process, to have access to the function bodies, define the implementation header

```c
#define CARENA_IMPLEMENTATION
```
3. Include `carena.h` in your project

```c
#include "carena.h"
```

## How to use
The size of an arena is defined by the `CARENA_CAPACITY` macro inside of `carena.h`.
Edit this macro if you want to alter this default capacity.
```c
#define CARENA_IMPLEMENTATION
#include "carena.h"

// a test structure, which we'll allocate a bunch of
typedef struct{
    int i1;
    int i2;
} Test;

int main(void)
{
    Carena *arena = Carena_init(); // Initialize the Arena
    
    for (int i=0; i<1000; ++i){
        Test *test = Carena_alloc(arena, sizeof(Test));  // use Carena_alloc to allocate memory, just like with malloc
        *test = (Test) {69, i};
    }
    Carena_status(arena); // show the current status of the arena
    Carena_free(arena); // free the entire arena buffer
    return 0;
}
```

## Function outlines

```c
Carena* Carena_init();
void    Carena_free(Carena *carena);
void*   Carena_alloc(Carena *carena, size_t size); // Note: memory is zero initialized by default
void    Carena_status(Carena *carena);
```