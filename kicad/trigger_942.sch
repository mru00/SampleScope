EESchema Schematic File Version 2  date Mon 30 Apr 2012 12:31:37 AM CEST
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:max532
LIBS:osci-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 6
Title ""
Date "29 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 7400 4650 2    60   Input ~ 0
TriggerCh2
Text GLabel 6700 4550 0    60   Input ~ 0
TriggerCh1
Text GLabel 8650 4200 2    60   Output ~ 0
TriggerOutCh1
Wire Wire Line
	8650 4200 6350 4200
Wire Wire Line
	6350 4200 6350 4350
Wire Wire Line
	6350 4350 6700 4350
Wire Wire Line
	6550 4850 6550 4650
Wire Wire Line
	6550 4650 6700 4650
Connection ~ 6350 5300
Wire Wire Line
	6100 5300 7800 5300
Wire Wire Line
	7800 5300 7800 4550
Wire Wire Line
	7800 4550 7400 4550
Wire Wire Line
	6700 4450 6350 4450
Wire Wire Line
	6350 4450 6350 5300
Wire Wire Line
	7400 4350 7800 4350
Wire Wire Line
	7800 4350 7800 4050
Wire Wire Line
	7400 4450 8650 4450
Text GLabel 8650 4450 2    60   Output ~ 0
TriggerOutCh2
$Comp
L DGND #PWR1
U 1 1 4F9D3D9F
P 6550 4850
F 0 "#PWR1" H 6550 4850 40  0001 C CNN
F 1 "DGND" H 6550 4780 40  0000 C CNN
	1    6550 4850
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR2
U 1 1 4F9D3D92
P 7800 4050
F 0 "#PWR2" H 7800 4150 30  0001 C CNN
F 1 "VCC" H 7800 4150 30  0000 C CNN
	1    7800 4050
	1    0    0    -1  
$EndComp
Text GLabel 6100 5300 0    60   Input ~ 0
TriggerLevel
$Comp
L DIL8 P13
U 1 1 4F9D3CF0
P 7050 4500
F 0 "P13" H 7050 4750 60  0000 C CNN
F 1 "DIL8" V 7050 4500 50  0000 C CNN
	1    7050 4500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
