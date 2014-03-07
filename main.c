/*
    Continuity.

    Piezo wired between PB2 and GND.

    Shorting PB0 and GND will make Piezo beep.
*/

#include <stdint.h>

#define M_BEGIN do {
#define M_END } while(0)

#define BIT_SET(P, B) M_BEGIN \
  *(volatile uint8_t *)(P) |= (1u << (B)); \
  M_END
#define BIT_CLR(P, B) M_BEGIN \
  *(volatile uint8_t *)(P) &=~ (1u << (B)); \
  M_END
#define BIT_GET(P, B) ((*(volatile uint8_t *)(P)) & (1u << (B)))

/*
These are SRAM addresses, recall SRAM address = IO address + 0x20.
*/
#define PORTB (0x38)
#define DDRB (0x37)
#define PINB (0x36)

void
delay(uint16_t i) {
  do {
    uint16_t n = 2000;
    __asm__ __volatile__ (
      "1: sbiw %0,1" "\n\t" // 2 cycles
      "brne 1b" : "=w" (n) : "0" (n) // 2 cycles
    );
    --i;
  } while(i);
}

int
main(void) {
  unsigned char i;

  BIT_SET(DDRB, 2);
  BIT_SET(PORTB, 0); /* pull-up */

  while(1) {
    if(!BIT_GET(PINB, 0)) {
      BIT_SET(PORTB, 2);
      delay(1);
      BIT_CLR(PORTB, 2);
      delay(1);
    }
  }
}

