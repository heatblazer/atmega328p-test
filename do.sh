#!/bin/sh

rm *.out
rm *.o
#avr-gcc -mmcu=atmega328p -Wall -Os -c untils.c
avr-gcc -mmcu=atmega328p -Wall -Os -c arch/atmega328p.c
avr-gcc -mmcu=atmega328p  -Wall -Os  main.c *.o
echo $? 
avr-objdump -D a.out | less 


