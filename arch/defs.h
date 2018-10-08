#pragma once

#define F_CPU 1000000
#define BAUD 9600
#define UBRR ((F_CPU)/8) / BAUD-1

#define ATMEGA328P_SCREEN_W 32
#define ATMEGA328P_SCREEN_H 32
