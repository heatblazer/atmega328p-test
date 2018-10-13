#!/bin/sh

rm *.out
rm *.o
avr-gcc -mmcu=atmega328p -Wall -Os -c untils.c # for helpers
#avr-gcc -mmcu=atmega328p -Wall -Os -c ringbuffer.c # helper rbuffer
avr-gcc -mmcu=atmega328p -Wall -Os -c arch/atmega328p.c
avr-gcc -mmcu=atmega328p -Wall -Os  main.c *.o
echo $? 
avr-objdump -D a.out | less 


