EESchema Schematic File Version 2  date Thu 19 Apr 2012 05:18:35 AM CEST
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
EELAYER 43  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 6
Title ""
Date "19 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DGND #PWR4
U 1 1 4F8F589D
P 2250 3200
F 0 "#PWR4" H 2250 3200 40  0001 C CNN
F 1 "DGND" H 2250 3130 40  0000 C CNN
	1    2250 3200
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR3
U 1 1 4F8F5892
P 2250 3100
F 0 "#PWR3" H 2250 3200 30  0001 C CNN
F 1 "VCC" H 2250 3200 30  0000 C CNN
	1    2250 3100
	1    0    0    -1  
$EndComp
NoConn ~ 2250 2900
$Comp
L VCC #PWR5
U 1 1 4F8F5794
P 2900 1950
F 0 "#PWR5" H 2900 2050 30  0001 C CNN
F 1 "VCC" H 2900 2050 30  0000 C CNN
	1    2900 1950
	1    0    0    -1  
$EndComp
Connection ~ 2900 2050
Wire Wire Line
	2900 2050 2900 1950
Wire Wire Line
	3050 2600 3050 2850
Wire Wire Line
	3050 2850 2700 2850
Wire Wire Line
	2700 2850 2700 2700
Wire Wire Line
	3050 2200 3050 2050
Wire Wire Line
	3050 2050 2700 2050
Wire Wire Line
	2700 2050 2700 2200
Wire Wire Line
	2850 2850 2850 3000
Wire Wire Line
	2850 3000 2250 3000
Connection ~ 2850 2850
$Comp
L C C5
U 1 1 4F8F577A
P 3050 2400
F 0 "C5" H 3100 2500 50  0000 L CNN
F 1 "C" H 3100 2300 50  0000 L CNN
	1    3050 2400
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 4F8F5777
P 2700 2450
F 0 "R13" V 2780 2450 50  0000 C CNN
F 1 "R" V 2700 2450 50  0000 C CNN
	1    2700 2450
	1    0    0    -1  
$EndComp
Text GLabel 1550 3000 0    60   Output ~ 0
TimerCounter.Out
NoConn ~ 1550 3100
$Comp
L DGND #PWR2
U 1 1 4F8F56D6
P 1550 3200
F 0 "#PWR2" H 1550 3200 40  0001 C CNN
F 1 "DGND" H 1550 3130 40  0000 C CNN
	1    1550 3200
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR1
U 1 1 4F8F567B
P 1550 2900
F 0 "#PWR1" H 1550 3000 30  0001 C CNN
F 1 "VCC" H 1550 3000 30  0000 C CNN
	1    1550 2900
	1    0    0    -1  
$EndComp
$Comp
L DIL8 P9
U 1 1 4F8F55CE
P 1900 3050
F 0 "P9" H 1900 3300 60  0000 C CNN
F 1 "ICL7242" V 1900 3050 50  0000 C CNN
	1    1900 3050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
