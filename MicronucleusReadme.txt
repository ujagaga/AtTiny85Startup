Micronucleus V2.03
==================

Micronucleus is a bootloader designed for AVR ATtiny microcontrollers with a minimal usb interface, cross platform libusb-based program upload tool, and a strong emphasis on bootloader compactness. To the authors knowledge this is, by far, the smallest USB bootloader for AVR ATtiny

The last release of the V1.x can be found here: https://github.com/micronucleus/micronucleus/tree/v1.11


Usage
=====

The bootloader allows uploading of new firmware via USB. In its usual configuration it is invoked at device reset and will identify to the host computer. If no communication is initiated by the host machine within a given time, the bootloader will time out and enter the user program, if one is present. 

For proper timing, the command line tool should to be started on the host computer _before_ the bootloader is invoked.

Windows machines will need to install the libusb drivers found in the /windows_drivers folder.  Clean Micronucleus devices without uploaded userprogram will not time out and allow sufficient time for proper driver installation. Linux and OS X do not require custom drivers.

Windows 10: Installing unsigned drivers became more difficult in Windows 10. Please use the Zadig driver installer as provided in the /windows_driver_installer folder.

Please invoke the command line tool with "micronucleus -help" for a list of available options.

The bootloader resides in the same memory as the user program, since the ATtiny series does not support a protected bootloader section. Therefore, special care has to be taken not to overwrite the bootloader if the user program uses the self programming features. The bootloader will patch itself into the reset vector of the user program. No other interrupt vectors are changed.

