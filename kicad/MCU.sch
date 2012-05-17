EESchema Schematic File Version 2  date Thu 17 May 2012 08:33:02 PM CEST
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
Sheet 5 5
Title "SampleScope"
Date "17 may 2012"
Rev "1"
Comp "mru"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	3500 5550 3500 5200
Wire Wire Line
	3500 5200 4250 5200
Wire Wire Line
	4250 4800 2400 4800
Wire Wire Line
	3500 6050 3500 6150
Connection ~ 7250 3550
Wire Wire Line
	7250 3550 7250 3350
Wire Wire Line
	7400 3700 7400 3550
Wire Wire Line
	7400 3550 7050 3550
Wire Wire Line
	7050 3550 7050 3700
Connection ~ 4100 3600
Wire Wire Line
	3800 3400 3800 3600
Wire Wire Line
	3800 3600 4250 3600
Wire Wire Line
	4950 3700 5800 3700
Wire Wire Line
	5800 3700 5800 3000
Wire Wire Line
	5800 3000 5900 3000
Wire Wire Line
	4100 3600 4100 2600
Wire Wire Line
	4100 2600 5900 2600
Wire Wire Line
	3800 5400 3800 5300
Wire Wire Line
	3800 5300 4250 5300
Wire Wire Line
	3000 5600 3000 5700
Connection ~ 2400 5000
Wire Wire Line
	2400 4800 2400 5200
Wire Wire Line
	3000 5200 3000 4900
Connection ~ 3000 5000
Wire Wire Line
	2400 5600 2400 5700
Wire Wire Line
	3800 5800 3800 5950
Wire Wire Line
	5900 2900 5700 2900
Wire Wire Line
	5700 2900 5700 3600
Wire Wire Line
	5700 3600 4950 3600
Wire Wire Line
	5900 3100 5900 3800
Wire Wire Line
	5900 3800 4950 3800
Wire Wire Line
	3800 2900 3800 2700
Wire Wire Line
	7400 4100 7400 4300
Wire Wire Line
	7400 4300 7050 4300
Wire Wire Line
	7050 4300 7050 4100
Wire Wire Line
	7250 4500 7250 4300
Connection ~ 7250 4300
Wire Wire Line
	3500 6550 3500 6650
Wire Wire Line
	3000 4900 4250 4900
$Comp
L DGND #PWR58
U 1 1 4F9924F5
P 3500 6650
F 0 "#PWR58" H 3500 6650 40  0001 C CNN
F 1 "DGND" H 3500 6580 40  0000 C CNN
	1    3500 6650
	1    0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 4F9924E5
P 3500 5800
F 0 "R17" V 3580 5800 50  0000 C CNN
F 1 "120R" V 3500 5800 50  0000 C CNN
	1    3500 5800
	1    0    0    -1  
$EndComp
$Comp
L LED D9
U 1 1 4F9924DD
P 3500 6350
F 0 "D9" H 3500 6450 50  0000 C CNN
F 1 "LED" H 3500 6250 50  0000 C CNN
	1    3500 6350
	0    1    1    0   
$EndComp
$Comp
L DGND #PWR68
U 1 1 4F953385
P 7250 4500
F 0 "#PWR68" H 7250 4500 40  0001 C CNN
F 1 "DGND" H 7250 4430 40  0000 C CNN
	1    7250 4500
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR041
U 1 1 4F95337A
P 7250 3350
F 0 "#PWR041" H 7250 3450 30  0001 C CNN
F 1 "VCC" H 7250 3450 30  0000 C CNN
	1    7250 3350
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 4F953362
P 7400 3900
F 0 "C7" H 7450 4000 50  0000 L CNN
F 1 "100n" H 7450 3800 50  0000 L CNN
	1    7400 3900
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 4F95335D
P 7050 3900
F 0 "C6" H 7100 4000 50  0000 L CNN
F 1 "100n" H 7100 3800 50  0000 L CNN
	1    7050 3900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR042
U 1 1 4F95322F
P 3800 2700
F 0 "#PWR042" H 3800 2800 30  0001 C CNN
F 1 "VCC" H 3800 2800 30  0000 C CNN
	1    3800 2700
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 4F953219
P 3800 3150
F 0 "R16" V 3880 3150 50  0000 C CNN
F 1 "10k" V 3800 3150 50  0000 C CNN
	1    3800 3150
	1    0    0    -1  
$EndComp
Text GLabel 4950 3900 2    60   Output ~ 0
ACDC.Ch2
$Comp
L DGND #PWR66
U 1 1 4F95309D
P 5900 2800
F 0 "#PWR66" H 5900 2800 40  0001 C CNN
F 1 "DGND" H 5900 2730 40  0000 C CNN
	1    5900 2800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR043
U 1 1 4F953093
P 5900 2700
F 0 "#PWR043" H 5900 2800 30  0001 C CNN
F 1 "VCC" H 5900 2800 30  0000 C CNN
	1    5900 2700
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR60
U 1 1 4F91B2A9
P 3800 5950
F 0 "#PWR60" H 3800 5950 40  0001 C CNN
F 1 "DGND" H 3800 5880 40  0000 C CNN
	1    3800 5950
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 4F91B246
P 3800 5600
F 0 "C5" H 3850 5700 50  0000 L CNN
F 1 "u220" H 3850 5500 50  0000 L CNN
	1    3800 5600
	1    0    0    -1  
$EndComp
Text GLabel 4250 3700 0    60   Output ~ 0
GEN.CS
$Comp
L CONN_6 P10
U 1 1 4F8FFC2D
P 6250 2850
F 0 "P10" V 6200 2850 60  0000 C CNN
F 1 "ICSP" V 6300 2850 60  0000 C CNN
	1    6250 2850
	1    0    0    -1  
$EndComp
Text GLabel 4250 4200 0    60   Output ~ 0
TriggerSelection
Text GLabel 4950 4000 2    60   Output ~ 0
ACDC.Ch1
Text GLabel 4250 4100 0    60   Output ~ 0
DAC.DIN
Text GLabel 4250 4000 0    60   Output ~ 0
DAC.SCLK
Text GLabel 4250 3800 0    60   Output ~ 0
DAC_Trigger.CS
Text GLabel 4250 3900 0    60   Output ~ 0
DAC_Gain.CS
Text GLabel 4250 4400 0    60   Output ~ 0
ADC.A1
Text GLabel 4250 4300 0    60   Output ~ 0
ADC.A0
Text GLabel 4250 4500 0    60   Output ~ 0
ADC.CS
Text GLabel 4950 4100 2    60   Output ~ 0
ADC.RD
Text GLabel 4950 4200 2    60   Input ~ 0
ADC.INT
Text GLabel 4950 4300 2    60   Input ~ 0
TriggerOut
$Comp
L DGND #PWR57
U 1 1 4F8F467B
P 3000 5700
F 0 "#PWR57" H 3000 5700 40  0001 C CNN
F 1 "DGND" H 3000 5630 40  0000 C CNN
	1    3000 5700
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR56
U 1 1 4F8F4677
P 2400 5700
F 0 "#PWR56" H 2400 5700 40  0001 C CNN
F 1 "DGND" H 2400 5630 40  0000 C CNN
	1    2400 5700
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 4F8F466E
P 3000 5400
F 0 "C4" H 3050 5500 50  0000 L CNN
F 1 "22p" H 3050 5300 50  0000 L CNN
	1    3000 5400
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 4F8F4669
P 2400 5400
F 0 "C3" H 2450 5500 50  0000 L CNN
F 1 "22p" H 2450 5300 50  0000 L CNN
	1    2400 5400
	1    0    0    -1  
$EndComp
$Comp
L DGND #PWR62
U 1 1 4F8F36FD
P 4250 4700
F 0 "#PWR62" H 4250 4700 40  0001 C CNN
F 1 "DGND" H 4250 4630 40  0000 C CNN
	1    4250 4700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR044
U 1 1 4F8F36FA
P 4250 4600
F 0 "#PWR044" H 4250 4700 30  0001 C CNN
F 1 "VCC" H 4250 4700 30  0000 C CNN
	1    4250 4600
	0    -1   -1   0   
$EndComp
$Comp
L DGND #PWR64
U 1 1 4F8F36F0
P 4950 4500
F 0 "#PWR64" H 4950 4500 40  0001 C CNN
F 1 "DGND" H 4950 4430 40  0000 C CNN
	1    4950 4500
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR045
U 1 1 4F8F36ED
P 4950 4400
F 0 "#PWR045" H 4950 4500 30  0001 C CNN
F 1 "VCC" H 4950 4500 30  0000 C CNN
	1    4950 4400
	1    0    0    -1  
$EndComp
Text Label 7450 5150 0    60   ~ 0
USB_D-
Text Label 7450 5250 0    60   ~ 0
USB_D+
Text Label 4950 5200 0    60   ~ 0
USB_D+
Text Label 4950 5300 0    60   ~ 0
USB_D-
$Comp
L DGND #PWR70
U 1 1 4F8F369D
P 7450 5350
F 0 "#PWR70" H 7450 5350 40  0001 C CNN
F 1 "DGND" H 7450 5280 40  0000 C CNN
	1    7450 5350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR046
U 1 1 4F8F368D
P 7450 5050
F 0 "#PWR046" H 7450 5150 30  0001 C CNN
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
P 2700 5000
F 0 "X1" H 2700 5150 60  0000 C CNN
F 1 "20MHz" H 2700 4850 60  0000 C CNN
	1    2700 5000
	-1   0    0    1   
$EndComp
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
L DIL40 U6
U 1 1 4F8F1D3A
P 4600 4550
F 0 "U6" H 4600 5600 70  0000 C CNN
F 1 "PIC18F4450" V 4600 4550 60  0000 C CNN
	1    4600 4550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
