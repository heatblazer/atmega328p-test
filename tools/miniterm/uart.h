#ifndef __UART_H__
#define __UART_H__

#include <termios.h>

/* ------------------------------------------------------------------------- */

struct uart {
	int databits;				/* 5, 6, 7, 8 */
	int parity;				/* 0- no, 1- odd, 2- even */
	int stopbits;				/* 1, 2 */
	int hw_flowctrl;			/* 1- on */
	int bps;

	int fd;
	struct termios tio;
	int pbps;				/* weird POSIX bps */
};

void uart_flush (struct uart *p);
void uart_drain (struct uart *p);
void uart_set_sig (struct uart *p, int dtr, int rts, int brk);
void uart_set_fc (struct uart *p, int fc);
void uart_set_bps (struct uart *p, int bps);
int uart_read (struct uart *p, void *buf, int n, int sec, int usec);
int uart_write (struct uart *p, const void *buf, int n);
int uart_open (struct uart *p, const char *name);
void uart_close (struct uart *p);

#endif
