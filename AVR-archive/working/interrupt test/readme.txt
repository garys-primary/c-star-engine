This program is a test for PCINT1 interrupt, built on attiny84A


Wiring diagram

                         _______________
                        |               |
          VCC <---------| 1          14 |--------> GND
                        |               |
 VCC <--( + LED - )-----| 2          13 |-----( - LED + )--> VCC
                        |               |
 VCC <--( + LED - )-----| 3          12 |-----( - LED + )--> VCC
                        |    ATtiny     |
       USBASP RST <-----| 4          11 |-----( - LED + )--> VCC
                        |      84A      |
     GND <--[ BTN ]-----| 5          10 |--
                        |               |
                      --| 6           9 |-----> USBASP SCK
                        |               |
    USBASP MOSI  <------| 7           8 |-----> USBASP MISO
                        |_______________|

Pin description

LED on PIN 2 - will spark if interrupt occurred
LED on PIN 3 - device is ready

LED on PIN 13 - Test register contains zero
LED on PIN 12 - Test register contains non-zero
LED on PIN 11 - Test register contains 0x10 (might have a bug)

Test register:
.def	UART_buffer = R23


The interrupt occurs on both 0->1 and 1->0 PIN 5 level change
The program contains software UART which can be deleted safely.

Author: garys