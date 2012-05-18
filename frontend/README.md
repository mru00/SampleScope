Frontend/GUI
============

This directory contains the Frontend code.
The code is developed with Qt Creator, but builds without it.

I implemented a "emulation mode" which allows the software
to run without real hardware. This makes it easy to modifiy the
code and test it without real hardware.


Build
-----

    qmake
    make

should do the trick.

Requirements
------------

* qt4, qmake
* libqwt
* libfftw3
* libusb-1.0

HIDAPI
------

The frontend uses HIDAPI, https://github.com/signal11/hidapi.
I simply copied the files
* hid.h
* hid-libusb.c
into my source tree.

At some point, hid-libusb.c was renamed to hid.c and moved 
to a separate directory, libusb/hid.c. I sticked with the original
filename for no particular reason.

