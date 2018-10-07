#include "untils.h"
#include <avr/io.h>

void Delay()
{
    unsigned long i;
    for(i=0; i < 100000UL; i++)
    {
	volatile long j = i;
	(void)j;
    }
}
