EESchema Schematic File Version 2  date Thu 19 Apr 2012 05:18:36 AM CEST
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
Sheet 6 6
Title "Osci: MCU Section"
Date "19 apr 2012"
Rev "1"
Comp "mru"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 6300 2200 2    60   Output ~ 0
TriggerSelection
Text GLabel 6300 2100 2    60   Output ~ 0
ACDC.Ch2
Text GLabel 6300 2000 2    60   Output ~ 0
ACDC.Ch1
Text GLabel 6300 1900 2    60   Output ~ 0
DAC.DIN
Text GLabel 6300 1800 2    60   Output ~ 0
DAC.SCLK
Text GLabel 6300 1700 2    60   Output ~ 0
DAC_Trigger.CS
Text GLabel 6300 1600 2    60   Output ~ 0
DAC_Gain.CS
Text GLabel 4900 2500 0    60   Input ~ 0
LATCH.LE
Text GLabel 4950 3700 2    60   Output ~ 0
LATCH.LE
Text GLabel 4950 3800 2    60   Output ~ 0
ADC.A1
Text GLabel 4950 3900 2    60   Output ~ 0
ADC.A0
Text GLabel 4950 4000 2    60   Output ~ 0
ADC.CS
Text GLabel 4950 4100 2    60   Output ~ 0
ADC.RD
Text GLabel 4950 4200 2    60   Input ~ 0
ADC.INT
Text GLabel 4950 4300 2    60   Input ~ 0
TriggerOut
$Comp
L DGND #PWR66
U 1 1 4F8F467B
P 3350 5700
F 0 "#PWR66" H 3350 5700 40  0001 C CNN
F 1 "DGND" H 3350 5630 40  0000 C CNN
	1    3350 5700
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR65
U 1 1 4F8F4677
P 2750 5700
F 0 "#PWR65" H 2750 5700 40  0001 C CNN
F 1 "DGND" H 2750 5630 40  0000 C CNN
	1    2750 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 5600 2750 5700
Connection ~ 3350 5000
Wire Wire Line
	3350 5200 3350 4900
Wire Wire Line
	4250 4800 2750 4800
Wire Wire Line
	3350 4900 4250 4900
Wire Wire Line
	2750 4800 2750 5200
Connection ~ 2750 5000
Wire Wire Line
	3350 5600 3350 5700
$Comp
L C C4
U 1 1 4F8F466E
P 3350 5400
F 0 "C4" H 3400 5500 50  0000 L CNN
F 1 "C" H 3400 5300 50  0000 L CNN
	1    3350 5400
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 4F8F4669
P 2750 5400
F 0 "C3" H 2800 5500 50  0000 L CNN
F 1 "C" H 2800 5300 50  0000 L CNN
	1    2750 5400
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR68
U 1 1 4F8F36FD
P 4250 4700
F 0 "#PWR68" H 4250 4700 40  0001 C CNN
F 1 "DGND" H 4250 4630 40  0000 C CNN
	1    4250 4700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR67
U 1 1 4F8F36FA
P 4250 4600
F 0 "#PWR67" H 4250 4700 30  0001 C CNN
F 1 "VCC" H 4250 4700 30  0000 C CNN
	1    4250 4600
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR71
U 1 1 4F8F36F0
P 4950 4500
F 0 "#PWR71" H 4950 4500 40  0001 C CNN
F 1 "DGND" H 4950 4430 40  0000 C CNN
	1    4950 4500
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR70
U 1 1 4F8F36ED
P 4950 4400
F 0 "#PWR70" H 4950 4500 30  0001 C CNN
F 1 "VCC" H 4950 4500 30  0000 C CNN
	1    4950 4400
	1    0    0    -1  
$EndComp
Text Label 7450 5250 0    60   ~ 0
USB_D-
Text Label 7450 5150 0    60   ~ 0
USB_D+
Text Label 4950 5200 0    60   ~ 0
USB_D+
Text Label 4950 5300 0    60   ~ 0
USB_D-
$Comp
L DGND #PWR75
U 1 1 4F8F369D
P 7450 5350
F 0 "#PWR75" H 7450 5350 40  0001 C CNN
F 1 "DGND" H 7450 5280 40  0000 C CNN
	1    7450 5350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR74
U 1 1 4F8F368D
P 7450 5050
F 0 "#PWR74" H 7450 5150 30  0001 C CNN
F 1 "VCC" H 7450 5150 30  0000 C CNN
	1    7450 5050
	1    0    0    -1  
$EndComp
$Comp
L USB_2 J1
U 1 1 4F8F3640
P 7250 5200
F 0 "J1" H 7175 5450 60  0000 C CNN
F 1 "USB_2" H 7300 4900 60  0001 C CNN
F 4 "VCC" H 7575 5350 50  0001 C CNN "VCC"
F 5 "D+" H 7550 5250 50  0001 C CNN "Data+"
F 6 "D-" H 7550 5150 50  0001 C CNN "Data-"
F 7 "GND" H 7575 5050 50  0001 C CNN "Ground"
	1    7250 5200
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X1
U 1 1 4F8F3620
P 3050 5000
F 0 "X1" H 3050 5150 60  0000 C CNN
F 1 "CRYSTAL" H 3050 4850 60  0000 C CNN
	1    3050 5000
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR72
U 1 1 4F8F3603
P 5600 1550
F 0 "#PWR72" H 5600 1650 30  0001 C CNN
F 1 "VCC" H 5600 1650 30  0000 C CNN
	1    5600 1550
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR73
U 1 1 4F8F35FF
P 5600 2650
F 0 "#PWR73" H 5600 2650 40  0001 C CNN
F 1 "DGND" H 5600 2580 40  0000 C CNN
	1    5600 2650
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR69
U 1 1 4F8F35FA
P 4900 2600
F 0 "#PWR69" H 4900 2600 40  0001 C CNN
F 1 "DGND" H 4900 2530 40  0000 C CNN
	1    4900 2600
	1    0    0    -1  
$EndComp
Text GLabel 4900 2300 0    60   Input ~ 0
D7
Text GLabel 4900 2200 0    60   Input ~ 0
D6
Text GLabel 4900 2100 0    60   Input ~ 0
D5
Text GLabel 4900 2000 0    60   Input ~ 0
D4
Text GLabel 4900 1900 0    60   Input ~ 0
D3
Text GLabel 4900 1800 0    60   Input ~ 0
D2
Text GLabel 4900 1700 0    60   Input ~ 0
D1
Text GLabel 4900 1600 0    60   Input ~ 0
D0
Text GLabel 4950 4600 2    60   Output ~ 0
D7
Text GLabel 4950 4700 2    60   Output ~ 0
D6
Text GLabel 4950 4800 2    60   Output ~ 0
D5
Text GLabel 4950 4900 2    60   Output ~ 0
D4
Text GLabel 4950 5400 2    60   Output ~ 0
D3
Text GLabel 4950 5500 2    60   Output ~ 0
D2
Text GLabel 4250 5500 0    60   Output ~ 0
D1
Text GLabel 4250 5400 0    60   Output ~ 0
D0
$Comp
L 74LS573 U15
U 1 1 4F8F1D43
P 5600 2100
F 0 "U15" H 5750 2700 60  0000 C CNN
F 1 "74LS573" H 5850 1500 60  0000 C CNN
	1    5600 2100
	1    0    0    -1  
$EndComp
$Comp
L DIL40 U6
U 1 1 4F8F1D3A
P 4600 4550
F 0 "U6" H 4600 5600 70  0000 C CNN
F 1 "PIC18F4450" V 4600 4550 60  0000 C CNN
	1    4600 4550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
