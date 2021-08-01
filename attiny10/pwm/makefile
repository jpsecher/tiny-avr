MCU = ATTINY10

## Fuses, see http://www.engbedded.com/fusecalc

ifeq ($(MCU), ATTINY10)
DEVICE = attiny10
FUSES = -U fuse:w:0xfe:m
SHORT_DEVICE = t10
endif

PROGRAMMER = avrispmkii
OBJECTS = main.o
AVRDUDE = avrdude -c $(PROGRAMMER) -p $(SHORT_DEVICE)
COMPILE = avr-gcc -Wall -Os -mmcu=$(DEVICE)

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

# fuse:
# 	$(AVRDUDE) $(FUSES)

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

.PHONY: all clean cpp disasm flash fuse
