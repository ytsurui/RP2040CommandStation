EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_02x14_Odd_Even J1
U 1 1 621A19DC
P 2150 6350
F 0 "J1" H 2200 7167 50  0000 C CNN
F 1 "External_Conn_1" H 2200 7076 50  0000 C CNN
F 2 "footprint:PinHeader_Reverse_2x14_2_54mm" H 2150 6350 50  0001 C CNN
F 3 "~" H 2150 6350 50  0001 C CNN
	1    2150 6350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x14_Odd_Even J2
U 1 1 621A40D9
P 4950 6350
F 0 "J2" H 5000 7167 50  0000 C CNN
F 1 "External_Conn_2" H 5000 7076 50  0000 C CNN
F 2 "footprint:PinHeader_Reverse_2x14_2_54mm" H 4950 6350 50  0001 C CNN
F 3 "~" H 4950 6350 50  0001 C CNN
	1    4950 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5750 2700 5750
Wire Wire Line
	2700 5750 2700 5550
Wire Wire Line
	2450 6250 2700 6250
Connection ~ 2700 6250
Wire Wire Line
	2700 6250 2700 7250
Wire Wire Line
	2450 5850 2700 5850
Wire Wire Line
	2700 5850 2700 6250
Text Label 3350 5950 2    50   ~ 0
~LCD_RST
Text Label 3350 6050 2    50   ~ 0
I2C1_SDA
Text Label 3350 6150 2    50   ~ 0
I2C1_SCL
Text Label 3350 7050 2    50   ~ 0
KEY_COL4_IN
Text Label 3350 6950 2    50   ~ 0
KEY_COL3_IN
Text Label 3350 6850 2    50   ~ 0
KEY_COL2_IN
Text Label 3350 6750 2    50   ~ 0
KEY_COL1_IN
Text Label 3350 6350 2    50   ~ 0
KEY_ROW1_SEL
Text Label 3350 6550 2    50   ~ 0
KEY_ROW3_SEL
Wire Wire Line
	2450 6350 3350 6350
Wire Wire Line
	2450 6450 3350 6450
Wire Wire Line
	2450 6550 3350 6550
Wire Wire Line
	2450 6650 3350 6650
Text Label 3350 6650 2    50   ~ 0
KEY_ROW4_SEL
Text Label 3350 6450 2    50   ~ 0
KEY_ROW2_SEL
Wire Wire Line
	2450 6750 3350 6750
Wire Wire Line
	2450 6850 3350 6850
Wire Wire Line
	2450 6950 3350 6950
Wire Wire Line
	2450 7050 3350 7050
Wire Wire Line
	1950 5750 1700 5750
Wire Wire Line
	1700 5750 1700 5850
Wire Wire Line
	1950 7050 1700 7050
Connection ~ 1700 7050
Wire Wire Line
	1700 7050 1700 7250
Wire Wire Line
	1950 6950 1700 6950
Connection ~ 1700 6950
Wire Wire Line
	1700 6950 1700 7050
Wire Wire Line
	1950 6850 1700 6850
Connection ~ 1700 6850
Wire Wire Line
	1700 6850 1700 6950
Wire Wire Line
	1950 6750 1700 6750
Connection ~ 1700 6750
Wire Wire Line
	1700 6750 1700 6850
Wire Wire Line
	1950 6650 1700 6650
Connection ~ 1700 6650
Wire Wire Line
	1700 6650 1700 6750
Wire Wire Line
	1950 6550 1700 6550
Connection ~ 1700 6550
Wire Wire Line
	1700 6550 1700 6650
Wire Wire Line
	1950 6450 1700 6450
Connection ~ 1700 6450
Wire Wire Line
	1700 6450 1700 6550
Wire Wire Line
	1950 6350 1700 6350
Connection ~ 1700 6350
Wire Wire Line
	1700 6350 1700 6450
Wire Wire Line
	1950 6250 1700 6250
Connection ~ 1700 6250
Wire Wire Line
	1700 6250 1700 6350
Wire Wire Line
	1950 6150 1700 6150
Connection ~ 1700 6150
Wire Wire Line
	1700 6150 1700 6250
Wire Wire Line
	1950 6050 1700 6050
Connection ~ 1700 6050
Wire Wire Line
	1700 6050 1700 6150
Wire Wire Line
	1950 5950 1700 5950
Connection ~ 1700 5950
Wire Wire Line
	1700 5950 1700 6050
Wire Wire Line
	1950 5850 1700 5850
Connection ~ 1700 5850
Wire Wire Line
	1700 5850 1700 5950
$Comp
L power:+3.3V #PWR0101
U 1 1 621B2505
P 2700 5550
F 0 "#PWR0101" H 2700 5400 50  0001 C CNN
F 1 "+3.3V" H 2715 5723 50  0000 C CNN
F 2 "" H 2700 5550 50  0001 C CNN
F 3 "" H 2700 5550 50  0001 C CNN
	1    2700 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 621B277D
P 2700 7250
F 0 "#PWR0102" H 2700 7000 50  0001 C CNN
F 1 "GND" H 2705 7077 50  0000 C CNN
F 2 "" H 2700 7250 50  0001 C CNN
F 3 "" H 2700 7250 50  0001 C CNN
	1    2700 7250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 621B29E9
P 1700 7250
F 0 "#PWR0103" H 1700 7000 50  0001 C CNN
F 1 "GND" H 1705 7077 50  0000 C CNN
F 2 "" H 1700 7250 50  0001 C CNN
F 3 "" H 1700 7250 50  0001 C CNN
	1    1700 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 6150 3350 6150
Wire Wire Line
	2450 6050 3350 6050
Wire Wire Line
	2450 5950 3350 5950
NoConn ~ 4750 5750
NoConn ~ 4750 5850
NoConn ~ 5250 5850
NoConn ~ 5250 5750
Wire Wire Line
	5250 5950 5500 5950
Wire Wire Line
	5500 5950 5500 6050
Wire Wire Line
	5250 7050 5500 7050
Connection ~ 5500 7050
Wire Wire Line
	5500 7050 5500 7250
Wire Wire Line
	5250 6950 5500 6950
Connection ~ 5500 6950
Wire Wire Line
	5500 6950 5500 7050
Connection ~ 5500 6850
Wire Wire Line
	5500 6850 5500 6950
Wire Wire Line
	5250 6850 5500 6850
Wire Wire Line
	5250 6750 5500 6750
Connection ~ 5500 6750
Wire Wire Line
	5500 6750 5500 6850
Wire Wire Line
	5250 6650 5500 6650
Connection ~ 5500 6650
Wire Wire Line
	5500 6650 5500 6750
Wire Wire Line
	5250 6550 5500 6550
Connection ~ 5500 6550
Wire Wire Line
	5500 6550 5500 6650
Wire Wire Line
	5250 6450 5500 6450
Connection ~ 5500 6450
Wire Wire Line
	5500 6450 5500 6550
Wire Wire Line
	5250 6350 5500 6350
Connection ~ 5500 6350
Wire Wire Line
	5500 6350 5500 6450
Wire Wire Line
	5250 6250 5500 6250
Connection ~ 5500 6250
Wire Wire Line
	5500 6250 5500 6350
Wire Wire Line
	5250 6150 5500 6150
Connection ~ 5500 6150
Wire Wire Line
	5500 6150 5500 6250
Wire Wire Line
	5250 6050 5500 6050
Connection ~ 5500 6050
Wire Wire Line
	5500 6050 5500 6150
Text Label 3800 6850 0    50   ~ 0
RAILPOWER_LED
Wire Wire Line
	3800 6850 4750 6850
Wire Wire Line
	4750 6950 3800 6950
Wire Wire Line
	3800 7050 4750 7050
Wire Wire Line
	4750 6450 3800 6450
Wire Wire Line
	4750 6550 3800 6550
Wire Wire Line
	4750 6650 3800 6650
Wire Wire Line
	4750 6750 3800 6750
Wire Wire Line
	4750 6250 3800 6250
Wire Wire Line
	4750 6350 3800 6350
Text Label 3800 6250 0    50   ~ 0
VR_1
Text Label 3800 6350 0    50   ~ 0
VR_2
Text Label 3800 6450 0    50   ~ 0
DIR2_IN
Text Label 3800 6550 0    50   ~ 0
DIR1_IN
Text Label 3800 6650 0    50   ~ 0
DIR_SCAN_SEL2
Text Label 3800 6750 0    50   ~ 0
DIR_SCAN_SEL1
Text Label 3800 7050 0    50   ~ 0
RAILPOWER_BTN
Text Label 3800 6950 0    50   ~ 0
FAILURE_LED
NoConn ~ 4750 5950
NoConn ~ 4750 6050
$Comp
L power:GND #PWR0104
U 1 1 621E1C16
P 5500 7250
F 0 "#PWR0104" H 5500 7000 50  0001 C CNN
F 1 "GND" H 5505 7077 50  0000 C CNN
F 2 "" H 5500 7250 50  0001 C CNN
F 3 "" H 5500 7250 50  0001 C CNN
	1    5500 7250
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW2
U 1 1 621E23FE
P 3000 2100
F 0 "SW2" H 3000 2485 50  0000 C CNN
F 1 "4" H 3000 2394 50  0000 C CNN
F 2 "footprint:TS-0606" H 3000 2400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3000 2400 50  0001 C CNN
	1    3000 2100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW10
U 1 1 621E3768
P 5000 2100
F 0 "SW10" H 5000 2485 50  0000 C CNN
F 1 "6" H 5000 2394 50  0000 C CNN
F 2 "footprint:TS-0606" H 5000 2400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5000 2400 50  0001 C CNN
	1    5000 2100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW14
U 1 1 621E3E81
P 6000 2100
F 0 "SW14" H 6000 2485 50  0000 C CNN
F 1 "B" H 6000 2394 50  0000 C CNN
F 2 "footprint:TS-0606" H 6000 2400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 6000 2400 50  0001 C CNN
	1    6000 2100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW3
U 1 1 621E5BA4
P 3000 2900
F 0 "SW3" H 3000 3285 50  0000 C CNN
F 1 "7" H 3000 3194 50  0000 C CNN
F 2 "footprint:TS-0606" H 3000 3200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3000 3200 50  0001 C CNN
	1    3000 2900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW7
U 1 1 621E5BAA
P 4000 2900
F 0 "SW7" H 4000 3285 50  0000 C CNN
F 1 "8" H 4000 3194 50  0000 C CNN
F 2 "footprint:TS-0606" H 4000 3200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4000 3200 50  0001 C CNN
	1    4000 2900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW11
U 1 1 621E5BB0
P 5000 2900
F 0 "SW11" H 5000 3285 50  0000 C CNN
F 1 "9" H 5000 3194 50  0000 C CNN
F 2 "footprint:TS-0606" H 5000 3200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5000 3200 50  0001 C CNN
	1    5000 2900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW15
U 1 1 621E5BB6
P 6000 2900
F 0 "SW15" H 6000 3285 50  0000 C CNN
F 1 "SW" H 6000 3194 50  0000 C CNN
F 2 "footprint:TS-0606" H 6000 3200 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 6000 3200 50  0001 C CNN
	1    6000 2900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW4
U 1 1 621E8D10
P 3000 3700
F 0 "SW4" H 3000 4085 50  0000 C CNN
F 1 "*" H 3000 3994 50  0000 C CNN
F 2 "footprint:TS-0606" H 3000 4000 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3000 4000 50  0001 C CNN
	1    3000 3700
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW8
U 1 1 621E8D16
P 4000 3700
F 0 "SW8" H 4000 4085 50  0000 C CNN
F 1 "0" H 4000 3994 50  0000 C CNN
F 2 "footprint:TS-0606" H 4000 4000 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4000 4000 50  0001 C CNN
	1    4000 3700
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW12
U 1 1 621E8D1C
P 5000 3700
F 0 "SW12" H 5000 4085 50  0000 C CNN
F 1 "#" H 5000 3994 50  0000 C CNN
F 2 "footprint:TS-0606" H 5000 4000 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5000 4000 50  0001 C CNN
	1    5000 3700
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW16
U 1 1 621E8D22
P 6000 3700
F 0 "SW16" H 6000 4085 50  0000 C CNN
F 1 "PROG" H 6000 3994 50  0000 C CNN
F 2 "footprint:TS-0606" H 6000 4000 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 6000 4000 50  0001 C CNN
	1    6000 3700
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW1
U 1 1 621EC888
P 3000 1300
F 0 "SW1" H 3000 1685 50  0000 C CNN
F 1 "1" H 3000 1594 50  0000 C CNN
F 2 "footprint:TS-0606" H 3000 1600 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 3000 1600 50  0001 C CNN
	1    3000 1300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW5
U 1 1 621EC88E
P 4000 1300
F 0 "SW5" H 4000 1685 50  0000 C CNN
F 1 "2" H 4000 1594 50  0000 C CNN
F 2 "footprint:TS-0606" H 4000 1600 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4000 1600 50  0001 C CNN
	1    4000 1300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW9
U 1 1 621EC894
P 5000 1300
F 0 "SW9" H 5000 1685 50  0000 C CNN
F 1 "3" H 5000 1594 50  0000 C CNN
F 2 "footprint:TS-0606" H 5000 1600 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 5000 1600 50  0001 C CNN
	1    5000 1300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW13
U 1 1 621EC89A
P 6000 1300
F 0 "SW13" H 6000 1685 50  0000 C CNN
F 1 "A" H 6000 1594 50  0000 C CNN
F 2 "footprint:TS-0606" H 6000 1600 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 6000 1600 50  0001 C CNN
	1    6000 1300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5E SW6
U 1 1 621E311F
P 4000 2100
F 0 "SW6" H 4000 2485 50  0000 C CNN
F 1 "5" H 4000 2394 50  0000 C CNN
F 2 "footprint:TS-0606" H 4000 2400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 4000 2400 50  0001 C CNN
	1    4000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 1200 2650 1200
Wire Wire Line
	2650 1200 2650 1300
Wire Wire Line
	2800 1300 2650 1300
Connection ~ 2650 1300
Wire Wire Line
	2650 1300 2650 1500
Wire Wire Line
	2650 1500 2100 1500
Connection ~ 2650 1500
Wire Wire Line
	3800 1200 3650 1200
Wire Wire Line
	3650 1200 3650 1300
Wire Wire Line
	3650 1300 3800 1300
Wire Wire Line
	3650 1300 3650 1500
Wire Wire Line
	2650 1500 3650 1500
Connection ~ 3650 1300
Wire Wire Line
	4650 1500 4650 1300
Wire Wire Line
	4650 1200 4800 1200
Wire Wire Line
	3650 1500 4650 1500
Connection ~ 3650 1500
Wire Wire Line
	4800 1300 4650 1300
Connection ~ 4650 1300
Wire Wire Line
	4650 1300 4650 1200
Wire Wire Line
	4650 1500 5650 1500
Wire Wire Line
	5650 1500 5650 1300
Wire Wire Line
	5650 1200 5800 1200
Connection ~ 4650 1500
Wire Wire Line
	5800 1300 5650 1300
Connection ~ 5650 1300
Wire Wire Line
	5650 1300 5650 1200
Wire Wire Line
	3200 1300 3350 1300
Wire Wire Line
	3200 1200 3350 1200
Wire Wire Line
	3350 1200 3350 1300
Connection ~ 3350 1300
Wire Wire Line
	4200 1300 4350 1300
Wire Wire Line
	4200 1200 4350 1200
Wire Wire Line
	4350 1200 4350 1300
Connection ~ 4350 1300
Wire Wire Line
	5200 1300 5350 1300
Wire Wire Line
	5200 1200 5350 1200
Wire Wire Line
	5350 1200 5350 1300
Connection ~ 5350 1300
Wire Wire Line
	6200 1300 6350 1300
Wire Wire Line
	6200 1200 6350 1200
Wire Wire Line
	6350 1200 6350 1300
Connection ~ 6350 1300
Wire Wire Line
	3800 2100 3650 2100
Wire Wire Line
	3650 2100 3650 2300
Wire Wire Line
	3650 2300 2650 2300
Wire Wire Line
	3650 2300 4650 2300
Wire Wire Line
	5650 2300 5650 2100
Wire Wire Line
	5650 2100 5800 2100
Connection ~ 3650 2300
Wire Wire Line
	5800 2000 5650 2000
Wire Wire Line
	5650 2000 5650 2100
Connection ~ 5650 2100
Wire Wire Line
	4800 2100 4650 2100
Wire Wire Line
	4650 2100 4650 2300
Connection ~ 4650 2300
Wire Wire Line
	4650 2300 5650 2300
Wire Wire Line
	4800 2000 4650 2000
Wire Wire Line
	4650 2000 4650 2100
Connection ~ 4650 2100
Wire Wire Line
	3800 2000 3650 2000
Wire Wire Line
	3650 2000 3650 2100
Connection ~ 3650 2100
Wire Wire Line
	2800 2100 2650 2100
Wire Wire Line
	2650 2100 2650 2300
Connection ~ 2650 2300
Wire Wire Line
	2650 2300 2100 2300
Wire Wire Line
	2800 2000 2650 2000
Wire Wire Line
	2650 2000 2650 2100
Connection ~ 2650 2100
Wire Wire Line
	3200 2000 3350 2000
Wire Wire Line
	3350 1300 3350 2000
Wire Wire Line
	3200 2100 3350 2100
Wire Wire Line
	3350 2100 3350 2000
Connection ~ 3350 2000
Connection ~ 3350 2100
Wire Wire Line
	5200 2000 5350 2000
Wire Wire Line
	5350 1300 5350 2000
Wire Wire Line
	5200 2100 5350 2100
Wire Wire Line
	5350 2100 5350 2000
Connection ~ 5350 2000
Wire Wire Line
	4200 2000 4350 2000
Wire Wire Line
	4350 1300 4350 2000
Wire Wire Line
	4200 2100 4350 2100
Wire Wire Line
	4350 2100 4350 2000
Connection ~ 4350 2000
Wire Wire Line
	6200 2000 6350 2000
Wire Wire Line
	6350 1300 6350 2000
Wire Wire Line
	6200 2100 6350 2100
Wire Wire Line
	6350 2100 6350 2000
Connection ~ 6350 2000
Wire Wire Line
	2800 2900 2650 2900
Wire Wire Line
	2650 2900 2650 3100
Wire Wire Line
	2650 3100 3650 3100
Wire Wire Line
	5650 3100 5650 2900
Wire Wire Line
	5650 2900 5800 2900
Wire Wire Line
	5800 2800 5650 2800
Wire Wire Line
	5650 2800 5650 2900
Connection ~ 5650 2900
Wire Wire Line
	2650 3100 2100 3100
Connection ~ 2650 3100
Wire Wire Line
	2800 2800 2650 2800
Wire Wire Line
	2650 2800 2650 2900
Connection ~ 2650 2900
Wire Wire Line
	3200 2800 3350 2800
Wire Wire Line
	3350 2100 3350 2800
Wire Wire Line
	3200 2900 3350 2900
Wire Wire Line
	3350 2900 3350 2800
Connection ~ 3350 2800
Wire Wire Line
	3200 3600 3350 3600
Wire Wire Line
	3350 3600 3350 2900
Connection ~ 3350 2900
Wire Wire Line
	3200 3700 3350 3700
Wire Wire Line
	3350 3700 3350 3600
Connection ~ 3350 3600
Wire Wire Line
	4200 2800 4350 2800
Wire Wire Line
	4350 2800 4350 2100
Connection ~ 4350 2100
Wire Wire Line
	4200 2900 4350 2900
Wire Wire Line
	4350 2900 4350 2800
Connection ~ 4350 2800
Wire Wire Line
	4200 3600 4350 3600
Wire Wire Line
	4350 3600 4350 2900
Connection ~ 4350 2900
Wire Wire Line
	4200 3700 4350 3700
Wire Wire Line
	4350 3700 4350 3600
Connection ~ 4350 3600
Wire Wire Line
	3800 2900 3650 2900
Wire Wire Line
	3650 2900 3650 3100
Connection ~ 3650 3100
Wire Wire Line
	3650 3100 4650 3100
Wire Wire Line
	3800 2800 3650 2800
Wire Wire Line
	3650 2800 3650 2900
Connection ~ 3650 2900
Wire Wire Line
	4800 2900 4650 2900
Wire Wire Line
	4650 2900 4650 3100
Connection ~ 4650 3100
Wire Wire Line
	4650 3100 5650 3100
Wire Wire Line
	4800 2800 4650 2800
Wire Wire Line
	4650 2800 4650 2900
Connection ~ 4650 2900
Wire Wire Line
	5200 2800 5350 2800
Wire Wire Line
	5350 2800 5350 2100
Connection ~ 5350 2100
Wire Wire Line
	5200 2900 5350 2900
Wire Wire Line
	5350 2900 5350 2800
Connection ~ 5350 2800
Wire Wire Line
	5200 3600 5350 3600
Wire Wire Line
	5350 3600 5350 2900
Connection ~ 5350 2900
Wire Wire Line
	5200 3700 5350 3700
Wire Wire Line
	5350 3700 5350 3600
Connection ~ 5350 3600
Wire Wire Line
	6200 2800 6350 2800
Wire Wire Line
	6350 2800 6350 2100
Connection ~ 6350 2100
Wire Wire Line
	6200 2900 6350 2900
Wire Wire Line
	6350 2900 6350 2800
Connection ~ 6350 2800
Wire Wire Line
	6200 3600 6350 3600
Wire Wire Line
	6350 3600 6350 2900
Connection ~ 6350 2900
Wire Wire Line
	6200 3700 6350 3700
Wire Wire Line
	6350 3700 6350 3600
Connection ~ 6350 3600
Wire Wire Line
	2800 3600 2650 3600
Wire Wire Line
	2650 3600 2650 3700
Wire Wire Line
	2650 3900 2100 3900
Wire Wire Line
	2650 3900 3650 3900
Wire Wire Line
	5650 3900 5650 3700
Wire Wire Line
	5650 3600 5800 3600
Connection ~ 2650 3900
Wire Wire Line
	5800 3700 5650 3700
Connection ~ 5650 3700
Wire Wire Line
	5650 3700 5650 3600
Wire Wire Line
	4800 3600 4650 3600
Wire Wire Line
	4650 3600 4650 3700
Connection ~ 4650 3900
Wire Wire Line
	4650 3900 5650 3900
Wire Wire Line
	4800 3700 4650 3700
Connection ~ 4650 3700
Wire Wire Line
	4650 3700 4650 3900
Wire Wire Line
	3800 3600 3650 3600
Wire Wire Line
	3650 3600 3650 3700
Connection ~ 3650 3900
Wire Wire Line
	3650 3900 4650 3900
Wire Wire Line
	3800 3700 3650 3700
Connection ~ 3650 3700
Wire Wire Line
	3650 3700 3650 3900
Connection ~ 3350 3700
Connection ~ 4350 3700
Connection ~ 5350 3700
Connection ~ 6350 3700
$Comp
L Device:Q_NMOS_GSD Q1
U 1 1 62400360
P 1900 1400
F 0 "Q1" V 2149 1400 50  0000 C CNN
F 1 "SSM3K7002BFU" V 2240 1400 50  0000 C CNN
F 2 "footprint:SC-70" H 2100 1500 50  0001 C CNN
F 3 "~" H 1900 1400 50  0001 C CNN
	1    1900 1400
	0    1    1    0   
$EndComp
$Comp
L Device:Q_NMOS_GSD Q2
U 1 1 6241CDAB
P 1900 2200
F 0 "Q2" V 2149 2200 50  0000 C CNN
F 1 "SSM3K7002BFU" V 2240 2200 50  0000 C CNN
F 2 "footprint:SC-70" H 2100 2300 50  0001 C CNN
F 3 "~" H 1900 2200 50  0001 C CNN
	1    1900 2200
	0    1    1    0   
$EndComp
$Comp
L Device:Q_NMOS_GSD Q3
U 1 1 6241D609
P 1900 3000
F 0 "Q3" V 2149 3000 50  0000 C CNN
F 1 "SSM3K7002BFU" V 2240 3000 50  0000 C CNN
F 2 "footprint:SC-70" H 2100 3100 50  0001 C CNN
F 3 "~" H 1900 3000 50  0001 C CNN
	1    1900 3000
	0    1    1    0   
$EndComp
Text Label 2250 3900 0    50   ~ 0
ROW4
Text Label 2250 3100 0    50   ~ 0
ROW3
Text Label 2250 2300 0    50   ~ 0
ROW2
Text Label 2250 1500 0    50   ~ 0
ROW1
Wire Wire Line
	3350 3700 3350 4300
Wire Wire Line
	4350 3700 4350 4450
Wire Wire Line
	5350 3700 5350 4600
Wire Wire Line
	6350 3700 6350 4750
Wire Wire Line
	1900 3600 1900 3500
Wire Wire Line
	1700 3900 1400 3900
Connection ~ 1400 3900
Wire Wire Line
	1400 3100 1700 3100
Wire Wire Line
	1400 3100 1400 3900
Wire Wire Line
	1400 3100 1400 2300
Wire Wire Line
	1400 2300 1700 2300
Connection ~ 1400 3100
Wire Wire Line
	1400 2300 1400 1500
Wire Wire Line
	1400 1500 1700 1500
Connection ~ 1400 2300
Wire Wire Line
	1900 2800 1900 2700
Wire Wire Line
	1900 2000 1900 1900
Wire Wire Line
	1900 1200 1900 1100
Text Label 750  1900 0    50   ~ 0
KEY_ROW2_SEL
Text Label 750  2700 0    50   ~ 0
KEY_ROW3_SEL
Text Label 750  3500 0    50   ~ 0
KEY_ROW4_SEL
Text Label 750  4300 0    50   ~ 0
KEY_COL1_IN
Text Label 750  4450 0    50   ~ 0
KEY_COL2_IN
Text Label 750  4600 0    50   ~ 0
KEY_COL3_IN
Text Label 750  4750 0    50   ~ 0
KEY_COL4_IN
Wire Wire Line
	750  4300 3350 4300
Wire Wire Line
	750  4450 4350 4450
Wire Wire Line
	750  3500 1900 3500
Wire Wire Line
	750  4600 5350 4600
Wire Wire Line
	750  4750 6350 4750
Wire Wire Line
	750  2700 1900 2700
Wire Wire Line
	750  1900 1900 1900
Wire Wire Line
	750  1100 1900 1100
Text Label 750  1100 0    50   ~ 0
KEY_ROW1_SEL
$Comp
L Device:Q_NMOS_GSD Q4
U 1 1 6241DAB9
P 1900 3800
F 0 "Q4" V 2149 3800 50  0000 C CNN
F 1 "SSM3K7002BFU" V 2240 3800 50  0000 C CNN
F 2 "footprint:SC-70" H 2100 3900 50  0001 C CNN
F 3 "~" H 1900 3800 50  0001 C CNN
	1    1900 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	1400 3900 1400 4900
$Comp
L power:GND #PWR0105
U 1 1 625752AC
P 1400 4900
F 0 "#PWR0105" H 1400 4650 50  0001 C CNN
F 1 "GND" H 1405 4727 50  0000 C CNN
F 2 "" H 1400 4900 50  0001 C CNN
F 3 "" H 1400 4900 50  0001 C CNN
	1    1400 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT VR1
U 1 1 625795A0
P 7700 1300
F 0 "VR1" H 7630 1346 50  0000 R CNN
F 1 "103" H 7630 1255 50  0000 R CNN
F 2 "footprint:ALPS-SLIDEVOLUME" H 7700 1300 50  0001 C CNN
F 3 "~" H 7700 1300 50  0001 C CNN
	1    7700 1300
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT VR2
U 1 1 6257A015
P 8200 1750
F 0 "VR2" H 8130 1796 50  0000 R CNN
F 1 "103" H 8130 1705 50  0000 R CNN
F 2 "footprint:ALPS-SLIDEVOLUME" H 8200 1750 50  0001 C CNN
F 3 "~" H 8200 1750 50  0001 C CNN
	1    8200 1750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8050 1750 7000 1750
Wire Wire Line
	7550 1300 7000 1300
Wire Wire Line
	7700 1450 7700 2050
Wire Wire Line
	8200 1900 8200 2050
Wire Wire Line
	7700 1150 7700 1000
Wire Wire Line
	8200 1000 8200 1600
$Comp
L power:+3.3V #PWR0106
U 1 1 625EBE55
P 8200 1000
F 0 "#PWR0106" H 8200 850 50  0001 C CNN
F 1 "+3.3V" H 8215 1173 50  0000 C CNN
F 2 "" H 8200 1000 50  0001 C CNN
F 3 "" H 8200 1000 50  0001 C CNN
	1    8200 1000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0107
U 1 1 625EC3CB
P 7700 1000
F 0 "#PWR0107" H 7700 850 50  0001 C CNN
F 1 "+3.3V" H 7715 1173 50  0000 C CNN
F 2 "" H 7700 1000 50  0001 C CNN
F 3 "" H 7700 1000 50  0001 C CNN
	1    7700 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 625EC953
P 7700 2050
F 0 "#PWR0108" H 7700 1800 50  0001 C CNN
F 1 "GND" H 7705 1877 50  0000 C CNN
F 2 "" H 7700 2050 50  0001 C CNN
F 3 "" H 7700 2050 50  0001 C CNN
	1    7700 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 625ECECF
P 8200 2050
F 0 "#PWR0109" H 8200 1800 50  0001 C CNN
F 1 "GND" H 8205 1877 50  0000 C CNN
F 2 "" H 8200 2050 50  0001 C CNN
F 3 "" H 8200 2050 50  0001 C CNN
	1    8200 2050
	1    0    0    -1  
$EndComp
Text Label 7000 1300 0    50   ~ 0
VR_1
Text Label 7000 1750 0    50   ~ 0
VR_2
$Comp
L Switch:SW_SPDT SW18
U 1 1 62600CBF
P 10100 1250
F 0 "SW18" H 10100 1535 50  0000 C CNN
F 1 "Direction1" H 10100 1444 50  0000 C CNN
F 2 "footprint:TOGGLE_SW_1C" H 10100 1250 50  0001 C CNN
F 3 "~" H 10100 1250 50  0001 C CNN
	1    10100 1250
	-1   0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT SW19
U 1 1 6260160F
P 10100 1850
F 0 "SW19" H 10100 2135 50  0000 C CNN
F 1 "Direction2" H 10100 2044 50  0000 C CNN
F 2 "footprint:TOGGLE_SW_1C" H 10100 1850 50  0001 C CNN
F 3 "~" H 10100 1850 50  0001 C CNN
	1    10100 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10300 1250 10700 1250
Wire Wire Line
	10300 1850 10700 1850
Text Label 10700 1850 2    50   ~ 0
DIR2_IN
Text Label 10700 1250 2    50   ~ 0
DIR1_IN
Wire Wire Line
	9900 1150 9700 1150
Wire Wire Line
	9900 1750 9700 1750
Wire Wire Line
	9700 1750 9700 1150
Connection ~ 9700 1150
Wire Wire Line
	9900 1350 9500 1350
Wire Wire Line
	9900 1950 9500 1950
Wire Wire Line
	9500 1950 9500 1350
Connection ~ 9500 1350
Text Label 8850 1150 0    50   ~ 0
DIR_SCAN_SEL1
Text Label 8850 1350 0    50   ~ 0
DIR_SCAN_SEL2
Wire Wire Line
	8850 1350 9500 1350
Wire Wire Line
	8850 1150 9700 1150
$Comp
L Device:LED D1
U 1 1 62720612
P 9850 3450
F 0 "D1" V 9889 3332 50  0000 R CNN
F 1 "LED" V 9798 3332 50  0000 R CNN
F 2 "footprint:LED_3mm" H 9850 3450 50  0001 C CNN
F 3 "~" H 9850 3450 50  0001 C CNN
	1    9850 3450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 627214AF
P 9850 2950
F 0 "R1" H 9920 2996 50  0000 L CNN
F 1 "331" H 9920 2905 50  0000 L CNN
F 2 "footprint:R_1608" V 9780 2950 50  0001 C CNN
F 3 "~" H 9850 2950 50  0001 C CNN
	1    9850 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 3100 9850 3300
$Comp
L Device:R R2
U 1 1 62733B56
P 10450 2950
F 0 "R2" H 10520 2996 50  0000 L CNN
F 1 "331" H 10520 2905 50  0000 L CNN
F 2 "footprint:R_1608" V 10380 2950 50  0001 C CNN
F 3 "~" H 10450 2950 50  0001 C CNN
	1    10450 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 3100 10450 3300
$Comp
L Device:LED D2
U 1 1 62745D2E
P 10450 3450
F 0 "D2" V 10489 3332 50  0000 R CNN
F 1 "LED" V 10398 3332 50  0000 R CNN
F 2 "footprint:LED_3mm" H 10450 3450 50  0001 C CNN
F 3 "~" H 10450 3450 50  0001 C CNN
	1    10450 3450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9850 2800 9850 2650
Wire Wire Line
	10450 2650 10450 2800
Wire Wire Line
	9850 2650 10150 2650
Wire Wire Line
	10150 2650 10150 2500
Wire Wire Line
	10450 2650 10150 2650
Connection ~ 10150 2650
$Comp
L power:+3.3V #PWR0110
U 1 1 627B5C3A
P 10150 2500
F 0 "#PWR0110" H 10150 2350 50  0001 C CNN
F 1 "+3.3V" H 10165 2673 50  0000 C CNN
F 2 "" H 10150 2500 50  0001 C CNN
F 3 "" H 10150 2500 50  0001 C CNN
	1    10150 2500
	1    0    0    -1  
$EndComp
Text Label 9850 4300 1    50   ~ 0
RAILPOWER_LED
Text Label 10450 4300 1    50   ~ 0
FAILURE_LED
Wire Wire Line
	9850 3600 9850 4300
Wire Wire Line
	10450 3600 10450 4300
$Comp
L Switch:SW_MEC_5E SW17
U 1 1 62827A08
P 10100 5600
F 0 "SW17" V 10146 5512 50  0000 R CNN
F 1 "RAIL_POWER" V 10055 5512 50  0000 R CNN
F 2 "footprint:TS-0606" H 10100 5900 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=1371" H 10100 5900 50  0001 C CNN
	1    10100 5600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10000 5800 10000 5950
Wire Wire Line
	10000 5950 10100 5950
Wire Wire Line
	10100 5950 10100 5800
Wire Wire Line
	10100 5950 10100 6250
Connection ~ 10100 5950
$Comp
L power:GND #PWR0111
U 1 1 6284E3BF
P 10100 6250
F 0 "#PWR0111" H 10100 6000 50  0001 C CNN
F 1 "GND" H 10105 6077 50  0000 C CNN
F 2 "" H 10100 6250 50  0001 C CNN
F 3 "" H 10100 6250 50  0001 C CNN
	1    10100 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 5400 10000 5250
Wire Wire Line
	10000 5250 10100 5250
Wire Wire Line
	10100 5250 10100 5400
Connection ~ 10100 5250
Text Label 10100 4500 3    50   ~ 0
RAILPOWER_BTN
Wire Wire Line
	10100 4500 10100 5250
$Comp
L CustomParts:AQM0802A U1
U 1 1 628B1AB5
P 7300 4800
F 0 "U1" H 7358 5565 50  0000 C CNN
F 1 "AQM1602Y" H 7358 5474 50  0000 C CNN
F 2 "footprint:AQM1602Y" H 7300 5550 50  0001 C CNN
F 3 "" H 7300 5550 50  0001 C CNN
	1    7300 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 4700 7900 4700
$Comp
L power:+3.3V #PWR0112
U 1 1 628C6338
P 7900 3750
F 0 "#PWR0112" H 7900 3600 50  0001 C CNN
F 1 "+3.3V" H 7915 3923 50  0000 C CNN
F 2 "" H 7900 3750 50  0001 C CNN
F 3 "" H 7900 3750 50  0001 C CNN
	1    7900 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 4800 7900 4800
Wire Wire Line
	7900 4800 7900 5400
Wire Wire Line
	7600 5400 7900 5400
$Comp
L power:GND #PWR0113
U 1 1 62902735
P 7900 6000
F 0 "#PWR0113" H 7900 5750 50  0001 C CNN
F 1 "GND" H 7905 5827 50  0000 C CNN
F 2 "" H 7900 6000 50  0001 C CNN
F 3 "" H 7900 6000 50  0001 C CNN
	1    7900 6000
	1    0    0    -1  
$EndComp
Text Label 8800 5100 2    50   ~ 0
~LCD_RST
Text Label 8800 5000 2    50   ~ 0
I2C1_SCL
Text Label 8800 4900 2    50   ~ 0
I2C1_SDA
$Comp
L Device:C C1
U 1 1 62940E16
P 8150 4150
F 0 "C1" H 8265 4196 50  0000 L CNN
F 1 "105" H 8265 4105 50  0000 L CNN
F 2 "footprint:C_1005" H 8188 4000 50  0001 C CNN
F 3 "~" H 8150 4150 50  0001 C CNN
	1    8150 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 3750 7900 3800
Wire Wire Line
	7600 4400 8150 4400
Wire Wire Line
	8150 4400 8150 4300
Wire Wire Line
	8150 4000 8150 3800
Wire Wire Line
	8150 3800 7900 3800
Connection ~ 7900 3800
Wire Wire Line
	7900 3800 7900 4700
$Comp
L Device:C C2
U 1 1 6297FAC2
P 8550 4500
F 0 "C2" V 8298 4500 50  0000 C CNN
F 1 "105" V 8389 4500 50  0000 C CNN
F 2 "footprint:C_1005" H 8588 4350 50  0001 C CNN
F 3 "~" H 8550 4500 50  0001 C CNN
	1    8550 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	7600 4500 8400 4500
Wire Wire Line
	7600 4600 8350 4600
Wire Wire Line
	8350 4600 8350 4750
Wire Wire Line
	8350 4750 8850 4750
Wire Wire Line
	8850 4750 8850 4500
Wire Wire Line
	8850 4500 8700 4500
Wire Wire Line
	7900 4700 8150 4700
Connection ~ 7900 4700
$Comp
L Device:C C3
U 1 1 629C119D
P 8150 5600
F 0 "C3" H 8265 5646 50  0000 L CNN
F 1 "105" H 8265 5555 50  0000 L CNN
F 2 "footprint:C_1005" H 8188 5450 50  0001 C CNN
F 3 "~" H 8150 5600 50  0001 C CNN
	1    8150 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 5850 8150 5850
Wire Wire Line
	8150 5850 8150 5750
Wire Wire Line
	7900 5850 7900 6000
Connection ~ 7900 5850
Wire Wire Line
	7900 5400 7900 5850
Connection ~ 7900 5400
Wire Wire Line
	8150 4700 8150 5450
Wire Wire Line
	7600 5300 9000 5300
Wire Wire Line
	9000 5300 9000 4300
$Comp
L Device:R R3
U 1 1 62A4778F
P 9000 4150
F 0 "R3" H 9070 4196 50  0000 L CNN
F 1 "101" H 9070 4105 50  0000 L CNN
F 2 "footprint:R_1608" V 8930 4150 50  0001 C CNN
F 3 "~" H 9000 4150 50  0001 C CNN
	1    9000 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4000 9000 3800
Wire Wire Line
	9000 3800 8150 3800
Connection ~ 8150 3800
Wire Wire Line
	7600 5100 8800 5100
Wire Wire Line
	7600 5000 8800 5000
Wire Wire Line
	7600 4900 8800 4900
$Comp
L power:+3.3V #PWR0114
U 1 1 62B7FA66
P 4500 5550
F 0 "#PWR0114" H 4500 5400 50  0001 C CNN
F 1 "+3.3V" H 4515 5723 50  0000 C CNN
F 2 "" H 4500 5550 50  0001 C CNN
F 3 "" H 4500 5550 50  0001 C CNN
	1    4500 5550
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 621DD551
P 6150 6750
F 0 "H1" H 6250 6796 50  0000 L CNN
F 1 "Hole_3.2mm" H 6250 6705 50  0000 L CNN
F 2 "footprint:HOLE_3.2mm" H 6150 6750 50  0001 C CNN
F 3 "~" H 6150 6750 50  0001 C CNN
	1    6150 6750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 621E0D43
P 6150 7000
F 0 "H2" H 6250 7046 50  0000 L CNN
F 1 "Hole_3.2mm" H 6250 6955 50  0000 L CNN
F 2 "footprint:HOLE_3.2mm" H 6150 7000 50  0001 C CNN
F 3 "~" H 6150 7000 50  0001 C CNN
	1    6150 7000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 621E1104
P 6150 7250
F 0 "H3" H 6250 7296 50  0000 L CNN
F 1 "Hole_3.2mm" H 6250 7205 50  0000 L CNN
F 2 "footprint:HOLE_3.2mm" H 6150 7250 50  0001 C CNN
F 3 "~" H 6150 7250 50  0001 C CNN
	1    6150 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3700 2650 3700
Connection ~ 2650 3700
Wire Wire Line
	2650 3700 2650 3900
Wire Wire Line
	4750 6150 4500 6150
Wire Wire Line
	4500 6150 4500 5550
$EndSCHEMATC
