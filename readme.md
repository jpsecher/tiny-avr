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

    $ brew cask install arduino

Select the AVR-ISP-MK2 programmer by Tools->Programmer->AVRISPmkII.

Install arduino-tiny package by Arduino->Preferences->Settings->AdditionalBoardsManagerURLs

    http://www.leonardomiliani.com/repository/package_leonardomiliani.com_index.json

Replace the bundled `avrdude` with the Brew version:

  $ cp /usr/local/Cellar/avrdude/6.3_1/bin/avrdude \
       ~/Library/Arduino15/packages/arduino/tools/avrdude/6.0.1-arduino5/bin/avrdude

(or use https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json ?)

Load the new boards by Tools->Board->BoardsManager and search for `attiny`.  Click Install, and after that the boards status should read INSTALLED.

## ATtiny2313A

Select the board by Tools->Board->ATtiny2313/4313, and the speed Tools->Micro->ATtiny2313@8MHz.

Set the programmer's TARGET jumper to ON and the POWER jumper to 5V so that it can provide power to the ATtiny microcontroller.

Use in-system programmer (ISP/ICSP) output.  Make a cable that can connect the ATtiny (target) to the programmer, cf. the [wiring from the programmer](icsp-cable.png) and how I [split out a 10-wire cable](icsp-wire-split.png), [soldered stiff, coloured wires](cable-heatshrink.jpg) to it, and [finished it up](cable-finished.jpg).

Check that communication works:

    $ avrdude -c avrispmkii -p t2313
    avrdude: AVR device initialized and ready to accept instructions
    Reading | ################################################## | 100% 0.00s
    avrdude: Device signature = 0x1e910a (probably t2313)
    avrdude: safemode: Fuses OK (E:FF, H:DF, L:64)

Make it blink:

    #define PIN_16 13
    void setup() {
      pinMode(PIN_16, OUTPUT);
    }
    void loop() {
      digitalWrite(PIN_16, HIGH);
      delay(1000);
      digitalWrite(PIN_16, LOW);
      delay(1000);
    }


The [datasheet](http://ww1.microchip.com/downloads/en/devicedoc/8246s.pdf).


## ATtiny 13A

The [datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/doc8126.pdf).

## ATtiny10

The [datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-8127-avr-8-bit-microcontroller-attiny4-attiny5-attiny9-attiny10_datasheet.pdf).

## Notes

To get rid of an old installation:

    $ brew cask uninstall arduino
    $ rm -rf ~/Library/Arduino15

AVRDude from source:

    $ svn co svn://svn.savannah.nongnu.org/avrdude/trunk avrdude
    $ cd avrdude/avrdude

Patch:

    --- stk500v2.c  (revision 1429)
    +++ stk500v2.c  (working copy)
    @@ -1633,7 +1633,7 @@
         PDATA(pgm)->pgmtype = PGMTYPE_AVRISP_MKII;
         pgm->set_sck_period = stk500v2_set_sck_period_mk2;
         pgm->fd.usb.max_xfer = USBDEV_MAX_XFER_MKII;
    -    pgm->fd.usb.rep = USBDEV_BULK_EP_READ_MKII;
    +    pgm->fd.usb.rep = 0;
         pgm->fd.usb.wep = USBDEV_BULK_EP_WRITE_MKII;
         pgm->fd.usb.eep = 0;           /* no seperate EP for events */
     #else

Build:

    $ ./bootstrap
    $ ./configure
    $ make


- http://svn.savannah.gnu.org/viewvc/avrdude/trunk/avrdude/

Arduino IDE upload problems:

- https://www.avrfreaks.net/forum/olimex-isp-mk2-error-mac-osx-109

