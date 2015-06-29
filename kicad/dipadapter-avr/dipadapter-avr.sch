EESchema Schematic File Version 2
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
LIBS:atmega8
LIBS:atmel89cxxxx
LIBS:atmel-1
LIBS:atmel-2005
LIBS:avr
LIBS:avr-1
LIBS:avr-2
LIBS:avr-3
LIBS:avr-4
LIBS:hopf
LIBS:rfm-ash
LIBS:mycomponents
LIBS:dimadapter-avr-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Clima sensor board for panStamp"
Date "17 Mar 2015"
Rev "1.0"
Comp "www.panstamp.com"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PANSTAMP_02 U2
U 1 1 55911624
P 7750 3750
F 0 "U2" H 8150 4650 60  0000 C CNN
F 1 "PANSTAMP_02" H 7350 4650 60  0000 C CNN
F 2 "old_mymods:PANSTAMP_02_SPI" H 7750 3750 60  0001 C CNN
F 3 "" H 7750 3750 60  0000 C CNN
	1    7750 3750
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR01
U 1 1 55911794
P 7800 2500
F 0 "#PWR01" H 7800 2600 30  0001 C CNN
F 1 "VCC" H 7800 2600 30  0000 C CNN
F 2 "" H 7800 2500 60  0001 C CNN
F 3 "" H 7800 2500 60  0001 C CNN
	1    7800 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2600 7750 2550
Wire Wire Line
	7750 2550 7850 2550
Wire Wire Line
	7850 2550 7850 2600
Wire Wire Line
	7800 2500 7800 2550
Connection ~ 7800 2550
$Comp
L GND #PWR02
U 1 1 5591183D
P 7850 4950
F 0 "#PWR02" H 7850 4700 60  0001 C CNN
F 1 "GND" H 7850 4800 60  0000 C CNN
F 2 "" H 7850 4950 60  0000 C CNN
F 3 "" H 7850 4950 60  0000 C CNN
	1    7850 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 4800 7550 4900
Wire Wire Line
	7550 4900 8050 4900
Wire Wire Line
	8050 4900 8050 4800
Wire Wire Line
	7950 4800 7950 4900
Connection ~ 7950 4900
Wire Wire Line
	7850 4800 7850 4950
Connection ~ 7850 4900
Wire Wire Line
	7750 4800 7750 4900
Connection ~ 7750 4900
Wire Wire Line
	7650 4800 7650 4900
Connection ~ 7650 4900
Wire Wire Line
	6950 3100 6500 3100
Wire Wire Line
	6950 3200 6500 3200
Wire Wire Line
	6950 3300 6500 3300
Wire Wire Line
	6950 3500 6500 3500
Wire Wire Line
	6950 3600 6500 3600
Wire Wire Line
	6950 3700 6500 3700
Wire Wire Line
	6950 3800 6500 3800
Wire Wire Line
	6950 3900 6500 3900
Wire Wire Line
	6950 4000 6500 4000
Wire Wire Line
	6950 4100 6500 4100
Wire Wire Line
	6950 4200 6500 4200
Wire Wire Line
	8600 3200 9100 3200
Wire Wire Line
	8600 3400 9100 3400
Wire Wire Line
	8600 3500 9100 3500
Wire Wire Line
	8600 3600 9100 3600
Wire Wire Line
	8600 3700 9100 3700
Wire Wire Line
	8600 3800 9100 3800
Wire Wire Line
	8600 3900 9100 3900
Wire Wire Line
	8600 4000 9100 4000
Wire Wire Line
	8600 4100 9100 4100
Wire Wire Line
	8600 4200 9100 4200
Text Label 9100 3200 2    60   ~ 0
RESET
Text Label 6500 3500 0    60   ~ 0
A0
Text Label 6500 3600 0    60   ~ 0
A1
Text Label 6500 3700 0    60   ~ 0
A2
Text Label 6500 3800 0    60   ~ 0
A3
Text Label 6500 3900 0    60   ~ 0
A4
Text Label 6500 4000 0    60   ~ 0
A5
Text Label 6500 4100 0    60   ~ 0
A6
Text Label 6500 4200 0    60   ~ 0
A7
Text Label 9100 3500 2    60   ~ 0
PB0
$Comp
L PANSTAMP_AVR2 PS1
U 1 1 55913858
P 3350 3650
F 0 "PS1" H 3900 4750 60  0000 C CNN
F 1 "PANSTAMP_AVR2" H 2900 4750 60  0000 C CNN
F 2 "mysmd:PANSTAMP_2" H 3350 4050 60  0001 C CNN
F 3 "" H 3350 4050 60  0000 C CNN
	1    3350 3650
	1    0    0    -1  
$EndComp
NoConn ~ 4400 3450
Wire Wire Line
	4400 2900 5100 2900
Wire Wire Line
	4400 3350 4850 3350
Wire Wire Line
	4400 3950 4850 3950
Wire Wire Line
	4400 4050 4850 4050
Wire Wire Line
	4400 4150 4850 4150
Wire Wire Line
	4400 4250 4850 4250
Wire Wire Line
	4400 4350 4850 4350
Wire Wire Line
	1850 2750 2300 2750
Wire Wire Line
	1850 2850 2300 2850
Wire Wire Line
	1850 2950 2300 2950
Wire Wire Line
	1850 3050 2300 3050
Wire Wire Line
	1850 3150 2300 3150
Wire Wire Line
	1850 3250 2300 3250
Wire Wire Line
	1850 3350 2300 3350
Wire Wire Line
	1850 3450 2300 3450
Wire Wire Line
	1850 3750 2300 3750
Wire Wire Line
	1850 3850 2300 3850
Wire Wire Line
	1850 3950 2300 3950
Wire Wire Line
	1850 4050 2300 4050
Wire Wire Line
	1850 4150 2300 4150
Wire Wire Line
	1850 4250 2300 4250
Wire Wire Line
	1850 4350 2300 4350
$Comp
L GND #PWR03
U 1 1 5591397B
P 3350 5050
F 0 "#PWR03" H 3350 4800 60  0001 C CNN
F 1 "GND" H 3350 4900 60  0000 C CNN
F 2 "" H 3350 5050 60  0000 C CNN
F 3 "" H 3350 5050 60  0000 C CNN
	1    3350 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 4900 3050 5000
Wire Wire Line
	3050 5000 3650 5000
Wire Wire Line
	3650 5000 3650 4900
Wire Wire Line
	3550 4900 3550 5000
Connection ~ 3550 5000
Wire Wire Line
	3450 4900 3450 5000
Connection ~ 3450 5000
Wire Wire Line
	3350 4900 3350 5050
Connection ~ 3350 5000
Wire Wire Line
	3250 4900 3250 5000
Connection ~ 3250 5000
Wire Wire Line
	3150 4900 3150 5000
Connection ~ 3150 5000
$Comp
L VCC #PWR04
U 1 1 55913B9A
P 4500 3150
F 0 "#PWR04" H 4500 3250 30  0001 C CNN
F 1 "VCC" H 4500 3250 30  0000 C CNN
F 2 "" H 4500 3150 60  0001 C CNN
F 3 "" H 4500 3150 60  0001 C CNN
	1    4500 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3250 4500 3250
Wire Wire Line
	4500 3250 4500 3150
Text Label 4850 3350 2    60   ~ 0
RESET
Text Label 1850 2750 0    60   ~ 0
PB1
Text Label 1850 2850 0    60   ~ 0
PB0
Text Label 1850 2950 0    60   ~ 0
A7
Text Label 1850 3050 0    60   ~ 0
A6
Text Label 1850 3150 0    60   ~ 0
A3
Text Label 1850 3250 0    60   ~ 0
A2
Text Label 1850 3350 0    60   ~ 0
A1
Text Label 1850 3450 0    60   ~ 0
A0
Text Label 1850 3750 0    60   ~ 0
PD1
Text Label 1850 3850 0    60   ~ 0
PD0
Text Label 1850 3950 0    60   ~ 0
A5
Text Label 1850 4050 0    60   ~ 0
A4
Text Label 1850 4150 0    60   ~ 0
SPI_SCK
Text Label 1850 4250 0    60   ~ 0
SPI_MISO
Text Label 1850 4350 0    60   ~ 0
SPI_MOSI
Text Label 4850 3950 2    60   ~ 0
PD3
Text Label 4850 4050 2    60   ~ 0
PD4
Text Label 4850 4150 2    60   ~ 0
PD5
Text Label 4850 4250 2    60   ~ 0
PD6
Text Label 4850 4350 2    60   ~ 0
PD7
Text Label 6500 3100 0    60   ~ 0
SPI_SCK
Text Label 6500 3200 0    60   ~ 0
SPI_MISO
Text Label 6500 3300 0    60   ~ 0
SPI_MOSI
Text Label 9100 3400 2    60   ~ 0
PB1
Text Label 9100 3600 2    60   ~ 0
PD7
Text Label 9100 3700 2    60   ~ 0
PD6
Text Label 9100 3800 2    60   ~ 0
PD5
Text Label 9100 3900 2    60   ~ 0
PD4
Text Label 9100 4000 2    60   ~ 0
PD3
Text Label 9100 4100 2    60   ~ 0
PD1
Text Label 9100 4200 2    60   ~ 0
PD0
$Comp
L ANTENNA U1
U 1 1 55914024
P 5550 3000
F 0 "U1" H 5650 3250 60  0000 C CNN
F 1 "ANTENNA" H 5750 2750 60  0000 C CNN
F 2 "mysmd:SMD_SMA_SPECIAL" H 5550 3000 60  0001 C CNN
F 3 "" H 5550 3000 60  0000 C CNN
	1    5550 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 55914259
P 5100 3200
F 0 "#PWR05" H 5100 2950 60  0001 C CNN
F 1 "GND" H 5100 3050 60  0000 C CNN
F 2 "" H 5100 3200 60  0000 C CNN
F 3 "" H 5100 3200 60  0000 C CNN
	1    5100 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3100 5100 3200
$EndSCHEMATC
