cd C:\asm\RGB_LED
c: 
avrdude -p t13 -c usbasp -e -u -U flash:w:main.hex
timeout 2