# Atmel ICE programmer

## Kernel driver

Download the [https://www.avrfreaks.net/sites/default/files/forum_attachments/AtmelICE-kext-for-High-Sierra.zip](AtmelICE kernel extension).

    sudo chown -R root:wheel AtmelICE.kext
    sudo mv AtmelICE.kext /Library/Extensions/AtmelICE.kext
    sudo find /Library/Extensions/AtmelICE.kext/ -print0 | sudo xargs -0 xattr -c
    sudo kextload /Library/Extensions/AtmelICE.kext

Open System Preferences and allow the kernel extension from "Nicholas Sayer".

    $ system_profiler SPUSBDataType

should show "Atmel-ICE CMSIS-DAP".


## AVRdude

ATtiny 1614 uses UDPI
