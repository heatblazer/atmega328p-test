#include "atmega328p.h"
#include "../untils.h"

#include <avr/io.h> 
#include <avr/interrupt.h> 

#include "../ringbuffer.h"

T_RING_BUFFER(unsigned char, rb, 64);

static unsigned long test = 0;



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
    delay(0);
    PORTB|= 1 << 5;
}

void init_device(void) //init_gpio
{
    PORTB |= 0 << 5;
    DDRB  |= 1 << 5; //set PB5 for output	
}

// setup uart 
void setup_uart(unsigned int ubrr)
{
    UCSR0B = (0<<RXEN0)|(0<<TXEN0); 

    // set hiher and lower 
    UBRR0H = ubrr >> 8;
    UBRR0L = ubrr;
    
    UCSR0A = (1<<U2X0) | (0 << MPCM0) ; //ux20 1 for asynchronoys ops, write it 0 to synchro. 
    UCSR0C =  (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stop
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0);    
    // make outpt 
    DDRD |= (1 << 7);
}
// flush, kind of getchar() getchar()
void uart_flush(void)
{
    unsigned char unused = 0;
    while (!(UCSR0A & (1 << RXC0))) ; 
    unused = UDR0;
}

void setup_isr(void)
{
    // not yet, just hint
//    EICRA |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3); 
//    PCICR |= (1 << 0) | (1 << 1) | (1 << 2); 
    //PCMSK0 |= (1 << PCINT0);     // mask the status
   // PCMSK2 |= (1 << PCINT17); //(1 << PCINT16) | (1 << PCINT17); // PCINT16
    // port b5 is PCINT5, when it blinks, maybe produce int
    //PCICR |= (1 << PCIE0);
   // PCIFR |= (1 << PCIF0) | (1 << PCIF1) | (1 << PCIF2); 
//     UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); 
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

// 0x0028 USART_TXC USART Tx Complete

// notes
// ISR
//Name:
//EICRA
//Offset: 0x69
//Reset:
//0x00



static void   test_prog1(void)
{
    for(;;)
    {
	unsigned long i = 0;
	for(i=0; i < ATMEGA328P_SCREEN_H * ATMEGA328P_SCREEN_W; i++)
	{
	    PORTD ^= (1<< 7);
	    uart_send(screen[i]);
	    uart_flush();
	}
    }
}


static void test_prog2(void)
{
    
}

void init_all(void)
{
    init_device();
    setup_uart(UBRR);
    setup_isr();
    sei();
    
}

void loop(void)
{
    test_prog1();
}


ISR(USART_RX_vect)
{
    PORTB ^= 1 << 5;
    rb_put_data(UDR0);
}


ISR(USART_TX_vect)
{
}
