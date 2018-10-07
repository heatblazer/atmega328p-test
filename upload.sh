#!/bin/sh

avr-objcopy -O binary a.out flash.bin 
avrdude -p m328p -c usbasp -U flash:w:flash.bin:r


