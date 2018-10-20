#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "uart.h"

/////
typedef unsigned char u8;
/////

/* ------------------------------------------------------------------------- */

/* 10 msec (in usec) */

#define TU_10MS 10000

/* minimalistic terminal */

static void do_term (struct uart *u0)
{
	int r;
	u8 tmp[64];
	struct termios tio;

	/* stdin non-blocking and un-canonized */

	fcntl (0, F_SETFL, fcntl (0, F_GETFL) | O_NONBLOCK);

	tcgetattr (0, &tio);
	tio.c_lflag &= ~(ICANON | ECHO);
	tcsetattr (0, TCSANOW, &tio);

	/* terminal poll */

	for ( ; ; ) {
		r = uart_read (u0, tmp, sizeof (tmp), 0, TU_10MS);
		if (r < 0)
			break;
		if (r >=0)
			printf ("%.*s", r, tmp);

		r = read (0, tmp, 64);
		if (r > 0)
			if (uart_write (u0, tmp, r))
				break;
	}
}

/* ------------------------------------------------------------------------- */

int main (int argc, char** argv)
{
        
	if (argc < 2)
	{
	    printf("Please provide device\r\n");
	    return -1;
	}
	
	char* dev = argv[1];
	printf("Device (%s)\r\n", dev);
	struct uart uart0, *u0 = &uart0;

	/* open serial device */

	u0->databits = 8;
	u0->parity = 0;
	u0->stopbits = 1;
	u0->hw_flowctrl = 0;
	u0->bps = 9600;
	    
	
	if (uart_open (u0, dev)) {
		fprintf (stderr, "cannot open serial device\n");
		goto err;
	}

	do_term (u0);

	/* done */

	uart_close (u0);

err:
	return 0;
}
