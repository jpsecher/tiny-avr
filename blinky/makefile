MCU = ATTINY13A

## Fuses, see http://www.engbedded.com/fusecalc

ifeq ($(MCU), ATTINY13A)
DEVICE = attiny13a
SHORT_DEVICE = t13
FUSES = -U lfuse:w:0x7A:m -U hfuse:w:0xFF:m
CLOCK = 8000000
endif

ifeq ($(MCU), ATTINY2313A)
DEVICE = attiny2313a
SHORT_DEVICE = t2313
FUSES = -U lfuse:w:0xE4:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
CLOCK = 8000000
endif

PROGRAMMER = avrispmkii
OBJECTS = main.o
AVRDUDE = avrdude -c $(PROGRAMMER) -p $(SHORT_DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.
flash: all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

clean:
	rm -f main.hex main.elf $(OBJECTS)

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c

.PHONY: disasm clean flash all
