EESchema Schematic File Version 2  date Fri 20 Apr 2012 04:21:19 PM CEST
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
Date "20 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L -12V #PWR?
U 1 1 4F9167CF
P 6950 3400
F 0 "#PWR?" H 6950 3530 20  0001 C CNN
F 1 "-12V" H 6950 3500 30  0000 C CNN
	1    6950 3400
	-1   0    0    1   
$EndComp
$Comp
L +12V #PWR?
U 1 1 4F9167C9
P 7650 3100
F 0 "#PWR?" H 7650 3050 20  0001 C CNN
F 1 "+12V" H 7650 3200 30  0000 C CNN
	1    7650 3100
	1    0    0    -1  
$EndComp
$Comp
L DIL8 P?
U 1 1 4F9166EB
P 7300 3250
F 0 "P?" H 7300 3500 60  0000 C CNN
F 1 "TLE2082" V 7300 3250 50  0000 C CNN
	1    7300 3250
	1    0    0    -1  
$EndComp
Text GLabel 4750 3600 2    60   Input ~ 0
DAC.DIN
Text GLabel 4750 3500 2    60   Input ~ 0
DAC.SCLK
Text GLabel 4750 3400 2    60   Input ~ 0
GEN.CS
Text Label 5700 3200 0    60   ~ 0
VOUT
Text Label 4050 3150 0    60   ~ 0
COMP
Text Label 3100 3400 0    60   ~ 0
CAP/2.5
Text Label 3950 4850 0    60   ~ 0
GEN_OSC
Wire Wire Line
	4050 3600 3950 3600
Wire Wire Line
	3950 3600 3950 5100
Wire Wire Line
	3950 5100 3750 5100
Wire Wire Line
	5800 3450 5800 3200
Wire Wire Line
	5800 3200 4750 3200
Connection ~ 3850 4050
Wire Wire Line
	3100 4050 3850 4050
Wire Wire Line
	3100 4050 3100 3900
Wire Wire Line
	3450 3500 3450 3400
Wire Wire Line
	4050 3200 4050 3100
Wire Wire Line
	3850 4350 3850 3500
Wire Wire Line
	3850 3500 4050 3500
Wire Wire Line
	3850 2400 3850 3300
Wire Wire Line
	3850 3300 4050 3300
Wire Wire Line
	4750 3300 5550 3300
Wire Wire Line
	5550 3300 5550 4100
Wire Wire Line
	4050 2700 4050 2500
Wire Wire Line
	4050 2500 3850 2500
Connection ~ 3850 2500
Wire Wire Line
	4050 3400 3100 3400
Wire Wire Line
	3100 3400 3100 3500
Connection ~ 3450 3400
Wire Wire Line
	3450 3900 3450 4050
Connection ~ 3450 4050
Wire Wire Line
	5800 3850 5800 4100
$Comp
L DGND #PWR1
U 1 1 4F915645
P 3050 5100
F 0 "#PWR1" H 3050 5100 40  0001 C CNN
F 1 "DGND" H 3050 5030 40  0000 C CNN
	1    3050 5100
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR01
U 1 1 4F915636
P 3750 4800
F 0 "#PWR01" H 3750 4900 30  0001 C CNN
F 1 "VCC" H 3750 4900 30  0000 C CNN
	1    3750 4800
	1    0    0    -1  
$EndComp
NoConn ~ 3750 4900
NoConn ~ 3750 5000
NoConn ~ 3050 5000
NoConn ~ 3050 4900
NoConn ~ 3050 4800
$Comp
L DIL8 P9
U 1 1 4F91561E
P 3400 4950
F 0 "P9" H 3400 5200 60  0000 C CNN
F 1 "DIL8" V 3400 4950 50  0000 C CNN
	1    3400 4950
	1    0    0    -1  
$EndComp
$Comp
L AGND #PWR02
U 1 1 4F915453
P 5800 4100
F 0 "#PWR02" H 5800 4100 40  0001 C CNN
F 1 "AGND" H 5800 4030 50  0000 C CNN
	1    5800 4100
	1    0    0    -1  
$EndComp
$Comp
L AGND #PWR03
U 1 1 4F91531F
P 5550 4100
F 0 "#PWR03" H 5550 4100 40  0001 C CNN
F 1 "AGND" H 5550 4030 50  0000 C CNN
	1    5550 4100
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR4
U 1 1 4F915318
P 3850 4350
F 0 "#PWR4" H 3850 4350 40  0001 C CNN
F 1 "DGND" H 3850 4280 40  0000 C CNN
	1    3850 4350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR04
U 1 1 4F9152FC
P 3850 2400
F 0 "#PWR04" H 3850 2500 30  0001 C CNN
F 1 "VCC" H 3850 2500 30  0000 C CNN
	1    3850 2400
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 4F9152CC
P 5800 3650
F 0 "C9" H 5850 3750 50  0000 L CNN
F 1 "C" H 5850 3550 50  0000 L CNN
	1    5800 3650
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 4F91522A
P 3150 2900
F 0 "C6" H 3200 3000 50  0000 L CNN
F 1 "C" H 3200 2800 50  0000 L CNN
	1    3150 2900
	1    0    0    -1  
$EndComp
$Comp
L CP1 C5
U 1 1 4F91521F
P 3100 3700
F 0 "C5" H 3150 3800 50  0000 L CNN
F 1 "10u" H 3150 3600 50  0000 L CNN
	1    3100 3700
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 4F915208
P 3450 3700
F 0 "C7" H 3500 3800 50  0000 L CNN
F 1 "u1" H 3500 3600 50  0000 L CNN
	1    3450 3700
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 4F915200
P 4050 2900
F 0 "C8" H 4100 3000 50  0000 L CNN
F 1 "u01" H 4100 2800 50  0000 L CNN
	1    4050 2900
	1    0    0    -1  
$EndComp
$Comp
L DIL10 P11
U 1 1 4F9151D1
P 4400 3400
F 0 "P11" H 4400 3700 60  0000 C CNN
F 1 "AD9833" V 4400 3400 50  0000 C CNN
	1    4400 3400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
