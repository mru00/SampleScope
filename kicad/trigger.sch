EESchema Schematic File Version 2  date Thu 03 May 2012 12:50:36 PM CEST
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
Sheet 4 6
Title "Trigger Section"
Date "3 may 2012"
Rev "1"
Comp "mru"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6900 4300 3400 4300
Wire Wire Line
	4150 5250 3650 5250
Wire Wire Line
	3650 5250 3650 4650
Wire Wire Line
	3650 4650 4150 4650
Connection ~ 4050 4650
Wire Wire Line
	4050 5350 4050 5150
Wire Wire Line
	4050 5150 4150 5150
Wire Wire Line
	4150 4850 4050 4850
Connection ~ 4150 5000
Wire Wire Line
	3800 5000 4150 5000
Wire Wire Line
	9150 5000 6900 5000
Connection ~ 6900 4300
Wire Wire Line
	6900 5000 6900 4300
Wire Wire Line
	4150 5050 4150 4950
Connection ~ 3950 5000
Wire Wire Line
	4150 4750 4150 4750
Wire Wire Line
	4050 4850 4050 4650
Wire Wire Line
	4150 5350 3400 5350
Wire Wire Line
	3400 5350 3400 4300
Connection ~ 4050 5350
NoConn ~ 4850 5050
Text GLabel 4850 4850 2    60   Input ~ 0
DAC_Trigger.CS
Text GLabel 4850 4950 2    60   Input ~ 0
DAC.DIN
Text GLabel 4850 5150 2    60   Input ~ 0
DAC.SCLK
$Comp
L DGND #PWR24
U 1 1 4F8F2E74
P 4850 4750
F 0 "#PWR24" H 4850 4750 40  0001 C CNN
F 1 "DGND" H 5000 4750 40  0000 C CNN
	1    4850 4750
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR25
U 1 1 4F8F2E6E
P 4850 5250
F 0 "#PWR25" H 4850 5250 40  0001 C CNN
F 1 "DGND" H 5000 5250 40  0000 C CNN
	1    4850 5250
	1    0    0    -1  
$EndComp
$Comp
L -12V #PWR013
U 1 1 4F8F2E69
P 4850 5350
F 0 "#PWR013" H 4850 5480 20  0001 C CNN
F 1 "-12V" H 4750 5350 30  0000 C CNN
	1    4850 5350
	-1   0    0    1   
$EndComp
$Comp
L +12V #PWR014
U 1 1 4F8F2E64
P 4850 4650
F 0 "#PWR014" H 4850 4600 20  0001 C CNN
F 1 "+12V" H 4950 4650 30  0000 C CNN
	1    4850 4650
	1    0    0    -1  
$EndComp
$Comp
L VAA #PWR20
U 1 1 4F8F2DC3
P 4150 4750
F 0 "#PWR20" H 4150 4810 30  0001 C CNN
F 1 "VAA" H 4100 4750 30  0000 C CNN
	1    4150 4750
	1    0    0    -1  
$EndComp
$Comp
L AGND #PWR015
U 1 1 4F8F2D52
P 3800 5000
F 0 "#PWR015" H 3800 5000 40  0001 C CNN
F 1 "AGND" H 3800 4930 50  0000 C CNN
	1    3800 5000
	1    0    0    -1  
$EndComp
$Comp
L DIL16 U2
U 1 1 4F8F2D36
P 4500 5000
F 0 "U2" H 4500 5450 60  0000 C CNN
F 1 "MAX532" V 4500 5000 50  0000 C CNN
	1    4500 5000
	1    0    0    -1  
$EndComp
Text GLabel 9150 5000 2    60   Output ~ 0
TriggerLevel
$EndSCHEMATC
