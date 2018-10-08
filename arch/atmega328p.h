#pragma once
#include "defs.h"

typedef enum _AError
{
    A_UNKNOWN,
    A_OK
} AError;

extern void blink(void);
extern void init_all(void);
extern void loop(void);

// TODO:
// functions for 
// set data directions
// set ports
