# Programming Atmel ATtiny on MacOS

The programmer I use is an AVR-ISP-MK2 from Olimex.

## Programmer

Check that the programmer is connected:

    $ system_profiler SPUSBDataType
      ...
        AVRISP mkII:

          Product ID: 0x2104
          Vendor ID: 0x03eb  (Atmel Corporation)
          Version: 2.00
          Serial Number: 764215213308
          Speed: Up to 12 Mb/sec
          Manufacturer: ATMEL
          Location ID: 0x14700000 / 19
          Current Available (mA): 500
          Extra Operating Current (mA): 0

To communicate with the programmer, install avrdude:

    $ brew install avrdude

Check that communication works:

    $ avrdude -c avrispmkii -p t10

The LED on the programmer should change from orange to green light.

## Arduino IDE 1.8

Select the AVR-ISP-MK2 programmer by Tools->Programmer->AVRISPmkII.

Install arduino-tiny package by Arduino->Preferences->Settings->AdditionalBoardsManagerURLs

    http://www.leonardomiliani.com/repository/package_leonardomiliani.com_index.json

(or? https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json)

Load the new boards by Tools->Board->BoardsManager and search for `tiny`.  Click Install, and after that the boards status should read INSTALLED.

## ATtiny2313A

Select the board by Tools->Board->ATtiny2313/4313, and the speed Tools->Micro->ATtiny2313@8MHz.

Set the programmer's TARGET jumper to ON and the POWER jumper to 5V so that it can provide power to the ATtiny microcontroller.

Use in-system programmer (ISP/ICSP) output.  Make a cable that can connect the ATtiny (target) to the programmer, cf. the [wiring from the programmer](icsp-cable.png) and how I [split out a 10-wire cable](icsp-wire-split.jpg).



The [datasheet](http://ww1.microchip.com/downloads/en/devicedoc/8246s.pdf).


## ATtiny 13A

The [datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/doc8126.pdf).

## ATtiny10

The [datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-8127-avr-8-bit-microcontroller-attiny4-attiny5-attiny9-attiny10_datasheet.pdf).

