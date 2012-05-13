SampleScope
===========

A cheap, simple, and fun-to-build diy USB oscilloscope

![Screenshot](https://github.com/mru00/SampleScope/raw/master/doc/Screenshot1.png "Screenshot")

![PCB](https://github.com/mru00/SampleScope/blob/master/doc/P1100868.resized.jpg "PCB")


Schematics based on http://jonathanweaver.net/ppmscope.html


* usb oscilloscope
* two channels
* almost all parts can be sampled directly from the manufacturer
* almost no parts needed anyhow, some capacitors and resistors
* pcb is really simple an can be done on perfboard
* input range +- 12V
* bandwidth about 200kHz, 1MSPS
* currently 256 byte sample buffer (more to come)


Hardware
--------

* PIC18F4450 (going to replace it with PIC18F4550)
* USB 2.0 Full Speed HID device
* simple analog part
 * input limiting & buffering
 * optional DC removal
 * programmable gain amplifier
 * level shifter
 * ADC
* trigger system
 * can trigger on both channel 1, channel 2, or free running
 * can trigger on rising or falling edge
* input selection
 * sample single channel, two channel interleaved or two channel sequential

 
Firmware
--------

Very simple firmware based on the Microchip Application Library USB example, 
most parts written in C, sampling routines written in ASM.

Frontend
--------

PC software written in C++ / Qt. OS independence by using libHIDAPI.
* Hardware emulation
* Plots
 * Normal
 * XY plot
 * FFT
 * autocorrelation
* Measures
 * frequency
 * RMS
 * AVG Voltages


To do
-----

* build better PGA
* put more effort into PCB layout
* find cheaper DC-DC converter
* upgrade MCU to PIC18F4550 (more ram -> more samples!)
* put some effort into graphical frontend
* cleanup code a little bit, get build environment better (link with HIDAPI)

Probably I won't do any of those items. ;)
