#pragma once
#include "defs.h"
#include <avr/io.h>


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
