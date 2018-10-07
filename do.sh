#!/bin/sh

rm *.out
rm *.o
avr-gcc -mmcu=atmega328p -Wall -Os -c untils.c
avr-gcc -mmcu=atmega328p -Wall -Os -c atmega328.c
avr-gcc -mmcu=atmega328p  -Wall -Os  main.c untils.o atmega328.o
echo $? 
avr-objdump -D a.out | less 


