/*
    Continuity.

    Piezo wired between PB0 and GND.

    Shorting PB2 and GND will make Piezo beep.
    Piezo waveform is controlled by Timer 0, and
    it is gated on or off using the DDRB register.

    REFERENCES

    [ATtinyDS] Atmel ATtiny 25/45/85 Datasheet
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

#define TCCR0B (*(volatile uint8_t *)(0x53))
#define TCCR0A (*(volatile uint8_t *)(0x4A))
#define OCR0A (*(volatile uint8_t *)(0x49))

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

  BIT_SET(PORTB, 2); /* pull-up */

  /* [ATtinyDS] 11.9 refers */
  /* CTC, Toggle ouput. */
  TCCR0A = 0x42;
  /* (61 + 1) * prescale * 2 = 7936 clocks per period
   * which is 1008Hz at a clock of 8MHz. */
  OCR0A = 61;
  /* Set T0 clock to flck/64 */
  TCCR0B = 0x3;

  while(1) {
    if(!BIT_GET(PINB, 2)) {
      /* OC0A */
      BIT_SET(DDRB, 0);
      delay(5);
    } else {
      BIT_CLR(DDRB, 0);
    }
  }
}

