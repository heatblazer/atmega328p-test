#include "atmega328p.h"

#define F_CPU 1000000
#define BAUD 9600
#define UBRR ((F_CPU)/8) / BAUD-1


int main(void)
{
    struct Atmega328p* atm = CreateDevice(); 
    atm->Init();
    atm->SetupUART(UBRR);

#if 0    
    for(;;)
	atm->Blink();
#endif

#if 1
    atm->Test_Screen1(atm); // write some bytes to some coords 
#endif
#if 0
    for(;;)
    {
	PORTD ^= (1<< 7);
	atm->UART_Send('a');
	atm->UART_Send('b');
	atm->UART_Send('c');
	atm->UART_Send('d');
	atm->UART_Send('e');
    }
#endif
    return 0;
}
