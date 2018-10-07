#pragma once
#include "defs.h"
#include "untils.h"
#include <avr/io.h>

struct internal;

typedef enum _AError
{
    A_UNKNOWN,
    A_OK
} AError;

// TODO:
// functions for 
// set data directions
// set ports

struct Atmega328p
{
    struct internal* _private;
    void (*Init)(void);
    void (*Blink)(void);
    AError (*SetupUART)(unsigned int);    
    void (*UART_Send)(unsigned char);
    void (*Uart_Recv)(char* ret);
    void (*Clear_screen)(struct Atmega328p*);
    void (*Put_pixel)(struct Atmega328p*, unsigned long, unsigned long, unsigned char);
    void (*Test_Screen1)(struct Atmega328p*);
    void (*Init_timers)(void); // todo
    void (*Init_ISR)(void); // todo
};


extern struct Atmega328p* CreateDevice();


