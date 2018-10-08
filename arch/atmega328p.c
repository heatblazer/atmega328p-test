#include "atmega328p.h"
#include "../untils.h"


// global screen buffer
static unsigned char screen[ATMEGA328P_SCREEN_H * ATMEGA328P_SCREEN_W] = {0};

static void clear_screen(void)
{
	
    unsigned long i;
    for(i=0; i < ATMEGA328P_SCREEN_H * ATMEGA328P_SCREEN_W; i++)
    {
	screen[i] = 'A';
    }
}

static void _put_pixl(unsigned int x, unsigned int y, unsigned char pix)
{
    screen[x * ATMEGA328P_SCREEN_H + y] = pix;
}


unsigned char uart_recv(unsigned int timeout)
{
    (void)timeout; 
    while ( !(UCSR0A & (1<<RXC0 /* 7 */)) ) {/* check the status register for reading */}
    return UDR0;
}

void uart_send(unsigned char data)
{
     while ( !( UCSR0A & (1<<UDRE0/* 5 */)) )
     {
	 // check the status reg for 5 bit
     }
    UDR0 = data;
}

void blink(void)
{
    PORTB = 1 << 5;
    delay(0);
    PORTB = 0 << 5;
    delay(0);	    
}

void init_device(void)
{
    DDRB  |= 1 << 5; //set PB5 for output	
}

// setup uart 
void setup_uart(unsigned int ubrr)
{
    // set hiher and lower 
    UBRR0H = ubrr >> 8;
    UBRR0L = ubrr;
    
    //MPCM0: - multiproc communication mode, not used 
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    UCSR0A = (1<<UDRE0)|(1<<U2X0); //ux20 1 for asynchronoys ops, write it 0 to synchro.

    UCSR0C =  (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stop
    
    // data direction to D port
    DDRD |= (1 << 7);
    return A_OK;    
}



void delay(unsigned int msec)
{
    (void)msec;
    unsigned long i;
    for(i=0; i < 100000UL; i++)
    {
	volatile long j = i;
	(void)j;
    } 
}

////////////////////////////////////////////////////////////////////////
static void   test_prog1(void)
{
    for(;;)
    {
	unsigned long i = 0;
	for(i=0; i < ATMEGA328P_SCREEN_H * ATMEGA328P_SCREEN_W; i++)
	{
	    PORTD ^= (1<< 7);
	    uart_send(screen[i]);
	}
    }
}

void init_all(void)
{
    init_device();
    setup_uart(UBRR);
}

void loop(void)
{
    test_prog1();
}
