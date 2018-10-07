#include "atmega328p.h"
#include "untils.h"

struct internal
{
    int error_code;
    int baud_rate; 
    char buffer[10];
    unsigned char screen[ATMEGA328P_SCREEN_H * ATMEGA328P_SCREEN_W];
};

static void _uart_recv(char* ret)
{
    while ( !(UCSR0A & (1<<RXC0 /* 7 */)) ) {/* check the status register for reading */}
    *ret = UDR0;
}

static void _uart_send(unsigned char data)
{
     while ( !( UCSR0A & (1<<UDRE0/* 5 */)) )
     {
	 // check the status reg for 5 bit
     }
    UDR0 = data;
}

static void _blink(void)
{
    PORTB = 1 << 5;
    Delay();
    PORTB = 0 << 5;
    Delay(); 	    
}

static void _init(void)
{
    DDRB  |= 1 << 5; //set PB5 for output	
}

// setup uart 
static AError _setup_uart(unsigned int ubrr)
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

static void _clr_scn(struct Atmega328p* _this)
{
    unsigned long i;
    for(i=0; i < ATMEGA328P_SCREEN_H * ATMEGA328P_SCREEN_W; i++)
    {
	_this->_private->screen[i] = 'A';
    }

}

static void _put_pixl(struct Atmega328p* _this, unsigned long x, unsigned long y, unsigned char pix)
{
    if (!_this)
	return;
    _this->_private->screen[x * ATMEGA328P_SCREEN_H + y] = pix;
}


static void dummy(void)
{
    // todo 
}

////////////////////////////////////////////////////////////////////////
static void   _tst_scrn(struct Atmega328p* _this)
{
    for(;;)
    {
	unsigned long i = 0;
	for(i=0; i < ATMEGA328P_SCREEN_H * ATMEGA328P_SCREEN_W; i++)
	{
	    PORTD ^= (1<< 7);
	    _this->UART_Send(_this->_private->screen[i]);
	}
    }
}


////////////////////////////////////////////////////////////////////////
static struct internal s_internal;
static struct Atmega328p s_Instance;


struct Atmega328p* CreateDevice(void)
{
    s_Instance._private = &s_internal;
    s_Instance.Blink = &_blink;
    s_Instance.Init = &_init;
    s_Instance.SetupUART = &_setup_uart;
    s_Instance.Clear_screen = &_clr_scn;
    
    s_Instance.UART_Send = &_uart_send;
    s_Instance.Uart_Recv = &_uart_recv;
    s_Instance.Put_pixel = &_put_pixl;
    
    s_Instance.Init_ISR = &dummy;
    s_Instance.Init_timers = &dummy; 
    // some setup
    
    
    _clr_scn(&s_Instance);
    s_Instance.Test_Screen1 = &_tst_scrn;
    _put_pixl(&s_Instance, 10, 10, 'H');
    _put_pixl(&s_Instance, 20, 20, 'U');
    _put_pixl(&s_Instance, 30, 30, 'I');
        
    return &s_Instance;
}
