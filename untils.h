#pragma once

extern unsigned char reverse_byte(unsigned char);

extern void delay(unsigned int);

extern void uart_send(unsigned char);

extern unsigned char uart_recv(unsigned int);

extern void init_device(void);

extern void setup_uart(unsigned int); 

extern void uart_flush(void);

extern void setup_isr(void);
