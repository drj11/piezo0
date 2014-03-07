CC=avr-gcc
CFLAGS=-mmcu=attiny85 -Os

main.hex: main.elf
	avr-objcopy -O ihex -R .eeprom $< $@

main.o: main.c

main.elf: main.o
	avr-gcc -Os -Wl,--gc-sections -mmcu=attiny85 -o $@ $<

flash: main.hex .PHONY
	avrdude -pattiny85 -cusbtiny -Uflash:w:main.hex

.PHONY:

# avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 foo.elf foo.eep # eeprom
