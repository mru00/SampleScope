SampleScope
===========

A cheap, simple, and fun-to-build diy USB oscilloscope

![Screenshot](/mru00/SampleScope/raw/master/doc/Screenshot2.png "Screenshot")

![PCB](/mru00/SampleScope/raw/master/doc/P1100869.resized.jpg "PCB")


Schematics based on http://jonw0224.tripod.com/ppmscope.html


* USB oscilloscope
* two channels
* almost all parts can be sampled directly from the manufacturer
* almost no parts needed anyhow, some capacitors and resistors
* pcb is really simple an can be done on perfboard
* input range +- 12V
* bandwidth about 200kHz, 1MSPS
* 1024 samples 


Hardware
--------

* PIC18F4455
* USB 2.0 Full Speed HID device
* simple analog part
 * input limiting & buffering
 * optional DC removal
 * programmable gain amplifier (0.05, 0.1, 0.2, 0.5, 1, 2, 5, 10 V/div)
 * level shifter
 * ADC
 * no anti aliasing filter
* trigger system
 * can trigger on both channel 1, channel 2, or free running
 * can trigger on rising or falling edge
* time bases 'fastest' (~17u), 100u, 300u, 1m, 3m, 10m, 30m, 100m sec/div
* input selection
 * sample single channel, two channel interleaved or two channel sequential
* uses dc-dc converter (xp power IQ0515) to supply analog circuitry
* everything is powered from USB - no external supply required

* IC List
 * all parts can be sampled, at least as a student.
 * MAX114 as ADC
 * MAX532 as DAC: programmable gain amplifier and trigger voltage generation
 * MAX942 as trigger comparator
 * MAX4622 as AC/DC selector
 * MAX6225, MAX6241, MAX6250 as voltage references
 * TLE2082 as Op Amp
 * PIC18F4455 as brain
 
* PCB
 * as simple as it gets
 * analog parts are simple to wire
 * digital connections are "flying wires", verowire


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
* Measurements
 * frequency
 * RMS and average voltage
* software emulation mode
 * emulates hardware, allows testing without the actual device

* development
 * uses HIDAPI https://github.com/signal11/hidapi
 * libqt4
 * libfftw3
 * libqwt6
 * libusb-1.0

To do
-----

* build better PGA (the current one is the main bottleneck in bandwidth)
* put more effort into PCB layout
 * i fixed some errors "on-line" without rerouting the pcb
* find cheaper DC-DC converter
* put some effort into graphical frontend
 * more measurements
 * more switches and customization
 * clean calibration ui, calibration wizards
* cleanup code a little bit, get build environment better (link with HIDAPI)
* platform independence (HIDAPI, libusb)
* anti aliasing filter (!!!)


Probably I won't do any of those items. ;)

License
-------
    SampleScope
    Copyright (C) 2012 mru

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


