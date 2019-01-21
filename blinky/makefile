DEVICE = attiny2313a
SHORT_DEVICE = t2313
CLOCK = 8000000
PROGRAMMER = -c avrispmkii
OBJECTS = main.o
# see http://www.engbedded.com/fusecalc
FUSES = -U lfuse:w:0xE4:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
AVRDUDE = avrdude $(PROGRAMMER) -p $(SHORT_DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf

# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
