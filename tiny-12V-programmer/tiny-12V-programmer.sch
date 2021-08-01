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
Wire Wire Line
	4200 5550 4000 5550
Wire Wire Line
	3750 5250 4000 5250
Wire Wire Line
	3750 4950 4000 4950
$Comp
L power:GND #PWR0102
U 1 1 5FCBE2B1
P 4000 4300
F 0 "#PWR0102" H 4000 4050 50  0001 C CNN
F 1 "GND" H 4005 4127 50  0000 C CNN
F 2 "" H 4000 4300 50  0001 C CNN
F 3 "" H 4000 4300 50  0001 C CNN
	1    4000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 4300 4000 4200
$Comp
L power:+5V #PWR0103
U 1 1 5FCBEC40
P 4200 5250
F 0 "#PWR0103" H 4200 5100 50  0001 C CNN
F 1 "+5V" H 4300 5300 50  0000 C CNN
F 2 "" H 4200 5250 50  0001 C CNN
F 3 "" H 4200 5250 50  0001 C CNN
	1    4200 5250
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR0104
U 1 1 5FCBF443
P 4200 4950
F 0 "#PWR0104" H 4200 4800 50  0001 C CNN
F 1 "+12V" H 4350 5000 50  0000 C CNN
F 2 "" H 4200 4950 50  0001 C CNN
F 3 "" H 4200 4950 50  0001 C CNN
	1    4200 4950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 5FCBFDCD
P 4000 2900
F 0 "#PWR0105" H 4000 2750 50  0001 C CNN
F 1 "+5V" H 4015 3073 50  0000 C CNN
F 2 "" H 4000 2900 50  0001 C CNN
F 3 "" H 4000 2900 50  0001 C CNN
	1    4000 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2900 4000 3000
$Comp
L power:+5V #PWR0106
U 1 1 5FCC032A
P 4700 4750
F 0 "#PWR0106" H 4700 4600 50  0001 C CNN
F 1 "+5V" H 4715 4923 50  0000 C CNN
F 2 "" H 4700 4750 50  0001 C CNN
F 3 "" H 4700 4750 50  0001 C CNN
	1    4700 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5FCC0E5D
P 4700 5000
F 0 "R5" H 4770 5046 50  0000 L CNN
F 1 "1k" H 4770 4955 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4630 5000 50  0001 C CNN
F 3 "~" H 4700 5000 50  0001 C CNN
	1    4700 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5FCC1576
P 4700 5400
F 0 "D1" V 4739 5282 50  0000 R CNN
F 1 "LED" V 4648 5282 50  0000 R CNN
F 2 "Diode_SMD:D_SOT-23_ANK" H 4700 5400 50  0001 C CNN
F 3 "~" H 4700 5400 50  0001 C CNN
	1    4700 5400
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5FCC249B
P 4700 5650
F 0 "#PWR0107" H 4700 5400 50  0001 C CNN
F 1 "GND" H 4705 5477 50  0000 C CNN
F 2 "" H 4700 5650 50  0001 C CNN
F 3 "" H 4700 5650 50  0001 C CNN
	1    4700 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 5650 4700 5550
Wire Wire Line
	4700 4850 4700 4750
Wire Wire Line
	4700 5250 4700 5150
$Comp
L tiny:SW_TPDT_x3 SW2
U 1 1 5FCC4B87
P 5400 3100
F 0 "SW2" H 5400 3385 50  0000 C CNN
F 1 "SW_TPDT_x3" H 5400 3294 50  0000 C CNN
F 2 "tiny:SW_TPDT" H 5400 3100 50  0001 C CNN
F 3 "~" H 5400 3100 50  0001 C CNN
	1    5400 3100
	1    0    0    -1  
$EndComp
$Comp
L tiny:SW_TPDT_x3 SW2
U 2 1 5FCC5289
P 5400 3600
F 0 "SW2" H 5400 3885 50  0000 C CNN
F 1 "SW_TPDT_x3" H 5400 3794 50  0000 C CNN
F 2 "tiny:SW_TPDT" H 5400 3600 50  0001 C CNN
F 3 "~" H 5400 3600 50  0001 C CNN
	2    5400 3600
	1    0    0    -1  
$EndComp
$Comp
L tiny:SW_TPDT_x3 SW2
U 3 1 5FCC6040
P 5400 4200
F 0 "SW2" H 5400 4485 50  0000 C CNN
F 1 "SW_TPDT_x3" H 5400 4394 50  0000 C CNN
F 2 "tiny:SW_TPDT" H 5400 4200 50  0001 C CNN
F 3 "~" H 5400 4200 50  0001 C CNN
	3    5400 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3300 4950 3300
Wire Wire Line
	4950 3300 4950 3100
Wire Wire Line
	4950 3100 5200 3100
Wire Wire Line
	4600 3400 4950 3400
Wire Wire Line
	4950 3400 4950 3600
Wire Wire Line
	4950 3600 5200 3600
Wire Wire Line
	4600 3600 4750 3600
Wire Wire Line
	4750 3600 4750 4200
Wire Wire Line
	4750 4200 5200 4200
Wire Wire Line
	4600 3500 4850 3500
Wire Wire Line
	4850 3500 4850 3900
Wire Wire Line
	4850 3900 5950 3900
Wire Wire Line
	5600 3000 5900 3000
$Comp
L power:GND #PWR0108
U 1 1 5FCD3726
P 7500 5550
F 0 "#PWR0108" H 7500 5300 50  0001 C CNN
F 1 "GND" H 7505 5377 50  0000 C CNN
F 2 "" H 7500 5550 50  0001 C CNN
F 3 "" H 7500 5550 50  0001 C CNN
	1    7500 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5550 7500 5450
Wire Wire Line
	7500 5450 7650 5450
Wire Wire Line
	7500 5450 7500 5350
Wire Wire Line
	7500 5350 7650 5350
Connection ~ 7500 5450
$Comp
L Connector:AVR-ISP-6 J4
U 1 1 5FCE68E9
P 7100 3400
F 0 "J4" H 6770 3496 50  0000 R CNN
F 1 "ISP" H 6770 3405 50  0000 R CNN
F 2 "Connector_IDC:IDC-Header_2x03_P2.54mm_Vertical" V 6850 3450 50  0001 C CNN
F 3 " ~" H 5825 2850 50  0001 C CNN
	1    7100 3400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6700 3200 5600 3200
Wire Wire Line
	6700 3400 5650 3400
Wire Wire Line
	5650 3400 5650 3700
Wire Wire Line
	5650 3700 5600 3700
$Comp
L power:GND #PWR0109
U 1 1 5FCEC673
P 7200 3900
F 0 "#PWR0109" H 7200 3650 50  0001 C CNN
F 1 "GND" H 7205 3727 50  0000 C CNN
F 2 "" H 7200 3900 50  0001 C CNN
F 3 "" H 7200 3900 50  0001 C CNN
	1    7200 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 3900 7200 3800
$Comp
L power:+5V #PWR0110
U 1 1 5FCED870
P 7200 2800
F 0 "#PWR0110" H 7200 2650 50  0001 C CNN
F 1 "+5V" H 7050 2850 50  0000 C CNN
F 2 "" H 7200 2800 50  0001 C CNN
F 3 "" H 7200 2800 50  0001 C CNN
	1    7200 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 2800 7200 2900
$Comp
L Isolator:6N135 U1
U 1 1 5FCEF19A
P 6800 2100
F 0 "U1" H 6800 2525 50  0000 C CNN
F 1 "6N135" H 6800 2434 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 6600 1800 50  0001 L CIN
F 3 "https://optoelectronics.liteon.com/upload/download/DS70-2008-0032/6N135-L%206N136-L%20series.pdf" H 6800 2100 50  0001 L CNN
	1    6800 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2200 6400 2200
Wire Wire Line
	6400 2200 6400 3500
Wire Wire Line
	6400 3500 6550 3500
$Comp
L Device:R R1
U 1 1 5FCF2DF1
P 6400 1750
F 0 "R1" H 6470 1796 50  0000 L CNN
F 1 "220" H 6470 1705 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6330 1750 50  0001 C CNN
F 3 "~" H 6400 1750 50  0001 C CNN
	1    6400 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2000 6400 2000
Wire Wire Line
	6400 2000 6400 1900
$Comp
L power:+5V #PWR0111
U 1 1 5FCF6E0A
P 6400 1500
F 0 "#PWR0111" H 6400 1350 50  0001 C CNN
F 1 "+5V" H 6415 1673 50  0000 C CNN
F 2 "" H 6400 1500 50  0001 C CNN
F 3 "" H 6400 1500 50  0001 C CNN
	1    6400 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1500 6400 1600
$Comp
L power:+12V #PWR0112
U 1 1 5FCF8326
P 7450 1700
F 0 "#PWR0112" H 7450 1550 50  0001 C CNN
F 1 "+12V" H 7465 1873 50  0000 C CNN
F 2 "" H 7450 1700 50  0001 C CNN
F 3 "" H 7450 1700 50  0001 C CNN
	1    7450 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5FCF93CF
P 7450 1950
F 0 "R2" H 7520 1996 50  0000 L CNN
F 1 "10k" H 7520 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 7380 1950 50  0001 C CNN
F 3 "~" H 7450 1950 50  0001 C CNN
	1    7450 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2100 7450 2200
Wire Wire Line
	7450 2200 7100 2200
NoConn ~ 7100 2000
$Comp
L power:GND #PWR0113
U 1 1 5FCFEAB2
P 7200 2400
F 0 "#PWR0113" H 7200 2150 50  0001 C CNN
F 1 "GND" H 7205 2227 50  0000 C CNN
F 2 "" H 7200 2400 50  0001 C CNN
F 3 "" H 7200 2400 50  0001 C CNN
	1    7200 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 2400 7200 2300
Wire Wire Line
	7200 2300 7100 2300
Wire Wire Line
	7450 2200 7600 2200
Connection ~ 7450 2200
$Comp
L power:+12V #PWR0114
U 1 1 5FD02821
P 7900 1900
F 0 "#PWR0114" H 7900 1750 50  0001 C CNN
F 1 "+12V" H 7915 2073 50  0000 C CNN
F 2 "" H 7900 1900 50  0001 C CNN
F 3 "" H 7900 1900 50  0001 C CNN
	1    7900 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 1900 7900 2000
$Comp
L Device:R R3
U 1 1 5FD048C6
P 7900 2750
F 0 "R3" H 7970 2796 50  0000 L CNN
F 1 "1k" H 7970 2705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7830 2750 50  0001 C CNN
F 3 "~" H 7900 2750 50  0001 C CNN
	1    7900 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5FD050B8
P 7900 3000
F 0 "#PWR0115" H 7900 2750 50  0001 C CNN
F 1 "GND" H 7905 2827 50  0000 C CNN
F 2 "" H 7900 3000 50  0001 C CNN
F 3 "" H 7900 3000 50  0001 C CNN
	1    7900 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 3000 7900 2900
Wire Wire Line
	7900 2600 7900 2500
Wire Wire Line
	7900 2500 7650 2500
Wire Wire Line
	7650 2500 7650 4300
Wire Wire Line
	7650 4300 5600 4300
Connection ~ 7900 2500
Wire Wire Line
	7900 2500 7900 2400
$Comp
L Device:R R4
U 1 1 5FD0884C
P 6550 2950
F 0 "R4" H 6620 2996 50  0000 L CNN
F 1 "10k" H 6620 2905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6480 2950 50  0001 C CNN
F 3 "~" H 6550 2950 50  0001 C CNN
	1    6550 2950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0116
U 1 1 5FD09989
P 6550 2700
F 0 "#PWR0116" H 6550 2550 50  0001 C CNN
F 1 "+5V" H 6565 2873 50  0000 C CNN
F 2 "" H 6550 2700 50  0001 C CNN
F 3 "" H 6550 2700 50  0001 C CNN
	1    6550 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3100 6550 3500
Connection ~ 6550 3500
Wire Wire Line
	6550 3500 6700 3500
Wire Wire Line
	6550 2700 6550 2800
NoConn ~ 6700 3300
$Comp
L Device:Q_PNP_BCE Q1
U 1 1 5FD16806
P 7800 2200
F 0 "Q1" H 7991 2154 50  0000 L CNN
F 1 "2SA968" H 7991 2245 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 8000 2300 50  0001 C CNN
F 3 "~" H 7800 2200 50  0001 C CNN
	1    7800 2200
	1    0    0    1   
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 5FD187A6
P 4200 2000
F 0 "SW1" H 4200 2285 50  0000 C CNN
F 1 "SW_Push" H 4200 2194 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 4200 2200 50  0001 C CNN
F 3 "~" H 4200 2200 50  0001 C CNN
	1    4200 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2000 4000 2000
Wire Wire Line
	4400 2000 4500 2000
$Comp
L Connector:Conn_01x02_Female J1
U 1 1 5FD22337
P 4700 2000
F 0 "J1" H 4728 1976 50  0000 L CNN
F 1 "Conn_01x02_Female" H 4728 1885 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 4700 2000 50  0001 C CNN
F 3 "~" H 4700 2000 50  0001 C CNN
	1    4700 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2000 3900 2100
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 5FD234A3
P 3700 2100
F 0 "J2" H 3592 1775 50  0000 C CNN
F 1 "Conn_01x02_Female" H 3592 1866 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 3700 2100 50  0001 C CNN
F 3 "~" H 3700 2100 50  0001 C CNN
	1    3700 2100
	-1   0    0    1   
$EndComp
Connection ~ 3900 2000
Wire Wire Line
	4500 2100 4500 2000
Connection ~ 4500 2000
$Comp
L Connector:Conn_01x02_Female J3
U 1 1 5FD25E26
P 6100 2900
F 0 "J3" H 6128 2876 50  0000 L CNN
F 1 "PB0" H 6128 2785 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 6100 2900 50  0001 C CNN
F 3 "~" H 6100 2900 50  0001 C CNN
	1    6100 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3000 5900 2900
Connection ~ 5900 3000
Wire Wire Line
	5600 3500 5900 3500
$Comp
L Connector:Conn_01x02_Female J5
U 1 1 5FD29059
P 6100 3500
F 0 "J5" H 6128 3476 50  0000 L CNN
F 1 "PB1" H 6128 3385 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 6100 3500 50  0001 C CNN
F 3 "~" H 6100 3500 50  0001 C CNN
	1    6100 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3500 5900 3600
Connection ~ 5900 3500
$Comp
L Connector:Conn_01x02_Female J6
U 1 1 5FD2C3C7
P 6150 3800
F 0 "J6" H 6178 3776 50  0000 L CNN
F 1 "PB2" H 6178 3685 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 6150 3800 50  0001 C CNN
F 3 "~" H 6150 3800 50  0001 C CNN
	1    6150 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3900 5950 3800
Connection ~ 5950 3900
$Comp
L Connector:Conn_01x02_Female J7
U 1 1 5FD2E2EF
P 6150 4100
F 0 "J7" H 6178 4076 50  0000 L CNN
F 1 "PB3" H 6178 3985 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 6150 4100 50  0001 C CNN
F 3 "~" H 6150 4100 50  0001 C CNN
	1    6150 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4100 5950 4100
Wire Wire Line
	5950 4100 5950 4200
Connection ~ 5950 4100
$Comp
L Connector:Conn_01x08_Female J10
U 1 1 5FD3413A
P 7850 5050
F 0 "J10" H 7878 5026 50  0000 L CNN
F 1 "GND" H 7878 4935 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 7850 5050 50  0001 C CNN
F 3 "~" H 7850 5050 50  0001 C CNN
	1    7850 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5350 7500 5250
Wire Wire Line
	7500 5250 7650 5250
Connection ~ 7500 5350
Wire Wire Line
	7500 5250 7500 5150
Wire Wire Line
	7500 5150 7650 5150
Connection ~ 7500 5250
Wire Wire Line
	7500 5150 7500 5050
Wire Wire Line
	7500 5050 7650 5050
Connection ~ 7500 5150
Wire Wire Line
	7500 5050 7500 4950
Wire Wire Line
	7500 4950 7650 4950
Connection ~ 7500 5050
Wire Wire Line
	7500 4950 7500 4850
Connection ~ 7500 4950
Wire Wire Line
	7500 4850 7650 4850
Wire Wire Line
	7650 4750 7500 4750
Wire Wire Line
	7500 4750 7500 4850
Connection ~ 7500 4850
Wire Wire Line
	6450 5450 6600 5450
Wire Wire Line
	6450 5450 6450 5350
Wire Wire Line
	6450 5350 6600 5350
$Comp
L Connector:Conn_01x08_Female J9
U 1 1 5FD4293E
P 6800 5050
F 0 "J9" H 6828 5026 50  0000 L CNN
F 1 "VCC" H 6828 4935 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 6800 5050 50  0001 C CNN
F 3 "~" H 6800 5050 50  0001 C CNN
	1    6800 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 5350 6450 5250
Wire Wire Line
	6450 5250 6600 5250
Connection ~ 6450 5350
Wire Wire Line
	6450 5250 6450 5150
Wire Wire Line
	6450 5150 6600 5150
Connection ~ 6450 5250
Wire Wire Line
	6450 5150 6450 5050
Wire Wire Line
	6450 5050 6600 5050
Connection ~ 6450 5150
Wire Wire Line
	6450 5050 6450 4950
Wire Wire Line
	6450 4950 6600 4950
Connection ~ 6450 5050
Wire Wire Line
	6450 4950 6450 4850
Connection ~ 6450 4950
Wire Wire Line
	6450 4850 6600 4850
Wire Wire Line
	6600 4750 6450 4750
Wire Wire Line
	6450 4750 6450 4850
Connection ~ 6450 4850
$Comp
L power:+5V #PWR0117
U 1 1 5FD45A72
P 6450 4750
F 0 "#PWR0117" H 6450 4600 50  0001 C CNN
F 1 "+5V" H 6465 4923 50  0000 C CNN
F 2 "" H 6450 4750 50  0001 C CNN
F 3 "" H 6450 4750 50  0001 C CNN
	1    6450 4750
	1    0    0    -1  
$EndComp
Connection ~ 6450 4750
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5FD57B11
P 4000 5250
F 0 "#FLG0101" H 4000 5325 50  0001 C CNN
F 1 "PWR_FLAG" H 4000 5423 50  0000 C CNN
F 2 "" H 4000 5250 50  0001 C CNN
F 3 "~" H 4000 5250 50  0001 C CNN
	1    4000 5250
	1    0    0    -1  
$EndComp
Connection ~ 4000 5250
Wire Wire Line
	4000 5250 4200 5250
Wire Wire Line
	4200 5650 4200 5550
$Comp
L power:GND #PWR0101
U 1 1 5FCBB2E1
P 4200 5650
F 0 "#PWR0101" H 4200 5400 50  0001 C CNN
F 1 "GND" H 4205 5477 50  0000 C CNN
F 2 "" H 4200 5650 50  0001 C CNN
F 3 "" H 4200 5650 50  0001 C CNN
	1    4200 5650
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5FD61C83
P 4000 5550
F 0 "#FLG0102" H 4000 5625 50  0001 C CNN
F 1 "PWR_FLAG" H 4000 5723 50  0000 C CNN
F 2 "" H 4000 5550 50  0001 C CNN
F 3 "~" H 4000 5550 50  0001 C CNN
	1    4000 5550
	1    0    0    -1  
$EndComp
Connection ~ 4000 5550
Wire Wire Line
	4000 5550 3750 5550
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5FD64F73
P 4000 4950
F 0 "#FLG0103" H 4000 5025 50  0001 C CNN
F 1 "PWR_FLAG" H 4000 5123 50  0000 C CNN
F 2 "" H 4000 4950 50  0001 C CNN
F 3 "~" H 4000 4950 50  0001 C CNN
	1    4000 4950
	1    0    0    -1  
$EndComp
Connection ~ 4000 4950
Wire Wire Line
	4000 4950 4200 4950
Wire Wire Line
	7450 1800 7450 1700
Wire Wire Line
	7100 1900 7200 1900
Wire Wire Line
	7200 1900 7200 1700
$Comp
L power:+12V #PWR0118
U 1 1 5FD6B270
P 7200 1700
F 0 "#PWR0118" H 7200 1550 50  0001 C CNN
F 1 "+12V" H 7215 1873 50  0000 C CNN
F 2 "" H 7200 1700 50  0001 C CNN
F 3 "" H 7200 1700 50  0001 C CNN
	1    7200 1700
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny10-TS U2
U 1 1 5FCBC18B
P 4000 3600
F 0 "U2" H 3470 3646 50  0000 R CNN
F 1 "ATtiny10" H 3470 3555 50  0000 R CNN
F 2 "tiny:pin-socket-02x03" H 4000 3600 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8127-AVR-8-bit-Microcontroller-ATtiny4-ATtiny5-ATtiny9-ATtiny10_Datasheet.pdf" H 4000 3600 50  0001 C CNN
	1    4000 3600
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5FDDE3FD
P 5450 5500
F 0 "H1" H 5550 5549 50  0000 L CNN
F 1 "MountingHole_Pad" H 5550 5458 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 5450 5500 50  0001 C CNN
F 3 "~" H 5450 5500 50  0001 C CNN
	1    5450 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5FDE6F44
P 5450 5650
F 0 "#PWR0119" H 5450 5400 50  0001 C CNN
F 1 "GND" H 5455 5477 50  0000 C CNN
F 2 "" H 5450 5650 50  0001 C CNN
F 3 "" H 5450 5650 50  0001 C CNN
	1    5450 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 5650 5450 5600
$Comp
L Connector:Conn_01x03_Female J8
U 1 1 5FE37698
P 3550 5250
F 0 "J8" H 3442 4925 50  0000 C CNN
F 1 "Conn_01x03_Female" H 3442 5016 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3550 5250 50  0001 C CNN
F 3 "~" H 3550 5250 50  0001 C CNN
	1    3550 5250
	-1   0    0    1   
$EndComp
Wire Wire Line
	3750 5150 3750 4950
Wire Wire Line
	3750 5350 3750 5550
Text Notes 8250 1950 0    50   ~ 0
TODO: smaller PNP\nTODO: 5V regulator\nTODO: LED for programming\nTODO: turn TPI connector (J4) 180
$EndSCHEMATC
