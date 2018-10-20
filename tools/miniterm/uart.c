#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include "uart.h"

/////
typedef unsigned char u8;
/////

/* ------------------------------------------------------------------------- */

/* line break time in msec (Linux specific) */

#define T_BRK 1

#define TU_10MS 10000

/* ------------------------------------------------------------------------- */

void uart_flush (struct uart *p)
{
	tcflush (p->fd, TCIOFLUSH);
}

void uart_drain (struct uart *p)
{
	tcdrain (p->fd);
}

void uart_set_sig (struct uart *p, int dtr, int rts, int brk)
{
	int ctl;

	ioctl (p->fd, TIOCMGET, &ctl);
	ctl &= ~(TIOCM_DTR | TIOCM_RTS);
	if (dtr)
		ctl |= TIOCM_DTR;
	if (rts)
		ctl |= TIOCM_RTS;
	ioctl (p->fd, TIOCMSET, &ctl);

	if (brk)
		tcsendbreak (p->fd, T_BRK);
}

void uart_set_fc (struct uart *p, int fc)
{
	p->hw_flowctrl = fc;

	if (fc)
		p->tio.c_cflag |= CRTSCTS;
	else
		p->tio.c_cflag &= ~CRTSCTS;

	tcsetattr (p->fd, TCSANOW, &p->tio);
}

void uart_set_bps (struct uart *p, int bps)
{
	switch (p->bps = bps) {
	case 1200:
		p->pbps = B1200;
		break;

	case 2400:
		p->pbps = B2400;
		break;

	case 4800:
		p->pbps = B4800;
		break;

	case 9600:
	default:
		p->pbps = B9600;
		break;

	case 19200:
		p->pbps = B19200;
		break;

	case 38400:
		p->pbps = B38400;
		break;

	case 57600:
		p->pbps = B57600;
		break;

	case 115200:
		p->pbps = B115200;
		break;

	case 230400:
		p->pbps = B230400;
	}

	cfsetispeed (&p->tio, p->pbps);
	cfsetospeed (&p->tio, p->pbps);
	tcsetattr (p->fd, TCSANOW, &p->tio);
}

int uart_read (struct uart *p, void *buf, int n, int sec, int usec)
{
	int r, t;
	u8 *b;
	fd_set fds;
	struct timeval tv;

	b = (u8 *)buf;
	t = 0;

	FD_ZERO (&fds);
	FD_SET (p->fd, &fds);
	tv.tv_sec = sec;			/* cumulative time */
	tv.tv_usec = usec;

	for ( ; n ; ) {
		r = select (p->fd + 1, &fds, 0, 0, &tv);
		if (!r)				/* timeout? */
			break;
		else if (r < 0) {
			fprintf (stderr, "%s: cannot select()\n", __FUNCTION__);
			return -1;
		}
		r = read (p->fd, b + t, n);
		if (r < 0) {
			fprintf (stderr, "%s: cannot read()\n", __FUNCTION__);
			return -1;
		}
		t += r;
		n -= r;
	}

	return t;
}

int uart_write (struct uart *p, const void *buf, int n)
{
	u8 *b;
	int w;

	b = (u8 *)buf;
	while (n) {
		w = write (p->fd, b, n);
		if (w == -1) {
			usleep (TU_10MS);
			continue;
		} else if (w < 0)
			return 1;
		b += w;
		n -= w;
	}

	return 0;
}

int uart_open (struct uart *p, const char *name)
{
	/* open serial device */

	p->fd = open (name, O_RDWR | O_NOCTTY | O_NDELAY);
	if (p->fd < 0)
		return 1;

	memset (&p->tio, 0, sizeof (p->tio));

	/* set new serial discipline */

	p->tio.c_iflag = IGNBRK;
	p->tio.c_oflag = 0;
	p->tio.c_cflag = CREAD | CLOCAL;
	switch (p->databits) {
	case 5:
		p->tio.c_cflag |= CS5;
		break;

	case 6:
		p->tio.c_cflag |= CS6;
		break;

	case 7:
		p->tio.c_cflag |= CS7;
		break;

	case 8:
	default:
		p->tio.c_cflag |= CS8;

	}
	if (p->parity) {
		p->tio.c_cflag |= PARENB;
		if (p->parity & 1)
			p->tio.c_cflag |= PARODD;
	}
	if (p->stopbits == 2)
		p->tio.c_cflag |= CSTOPB;
	p->tio.c_lflag = 0;

	p->tio.c_cc[VTIME] = 10;
	p->tio.c_cc[VMIN] = 1;

	uart_set_fc (p, p->hw_flowctrl);
	uart_set_bps (p, p->bps);

	uart_flush (p);

	return 0;
}

void uart_close (struct uart *p)
{
	close (p->fd);
}
