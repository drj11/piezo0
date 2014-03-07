/*
    Continuity.

    Piezo wired between PB0 and GND.

    Shorting GND and PB2 will make Piezo beep.
    Shorting GND and PB1 will make Piezo beep one fifth higher.
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

  /* pull-ups */
  BIT_SET(PORTB, 1);
  BIT_SET(PORTB, 2);

  /* [ATtinyDS] 11.9 refers */
  /* CTC, Toggle ouput. */
  TCCR0A = 0x42;
  /* Set T0 clock to flck/64 */
  TCCR0B = 0x3;

  while(1) {
    if(!BIT_GET(PINB, 2)) {
      /* (59 + 1) * prescale * 2 = 7680 clocks per period
       * which is 1042Hz at a clock of 8MHz (musical note C6). */
      OCR0A = 59;
      /* OC0A */
      BIT_SET(DDRB, 0);
      delay(5);
    } else if(!BIT_GET(PINB, 1)) {
      /* 1562Hz G6 */
      OCR0A = 40;
      /* OC0A */
      BIT_SET(DDRB, 0);
      delay(5);
    } else {
      BIT_CLR(DDRB, 0);
    }
  }
}

