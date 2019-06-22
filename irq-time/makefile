## Configuration section begin.

mcu = ATTINY4313
clock = 12288000
objects_with_tests = main.o
objects = main.o

## Configuration section end.


## avr-gcc, see https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
ifeq ($(mcu), ATTINY2313A)
	device = attiny2313a
	short_device = t2313
else ifeq ($(mcu), ATTINY4313)
	device = attiny4313
	short_device = t4313
endif

## Fuses, see http://www.engbedded.com/fusecalc
fuses = -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m
programmer = avrispmkii

unittest_objects = unittests.o $(patsubst %.o,%_test.o,$(objects_with_tests))
avrdude = avrdude -c $(programmer) -p $(short_device)
compile = avr-gcc -Wall -Os -DF_CPU=$(clock) -mmcu=$(device)

all: main.hex

.c.o:
	$(compile) -c $< -o $@

.c.s:
	$(compile) -S $< -o $@

# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.
flash: all
	$(avrdude) -U flash:w:main.hex:i

fuse:
	$(avrdude) $(fuses)

install: flash fuse

clean:
	rm -f main.hex main.elf $(objects) unittests $(unittest_objects)

main.elf: $(objects)
	$(compile) -o main.elf $(objects)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(device) main.elf

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(compile) -E main.c

unittests: $(unittest_objects)
	g++ $^ -o $@

.PHONY: disasm clean flash all
