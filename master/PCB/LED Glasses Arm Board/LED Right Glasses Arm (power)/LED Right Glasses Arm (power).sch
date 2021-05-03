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
Text Notes 5300 2450 0    50   ~ 0
Power Regulator Circuit\n
$Comp
L power:GND #PWR0108
U 1 1 5F4B6F73
P 5150 3950
F 0 "#PWR0108" H 5150 3700 50  0001 C CNN
F 1 "GND" H 5155 3777 50  0000 C CNN
F 2 "" H 5150 3950 50  0001 C CNN
F 3 "" H 5150 3950 50  0001 C CNN
	1    5150 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C105
U 1 1 5F4B8E20
P 5150 3850
F 0 "C105" H 5242 3896 50  0000 L CNN
F 1 "100N_15V" H 5242 3805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5150 3850 50  0001 C CNN
F 3 "" H 5150 3850 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/kemet/C0805C104M3RACTU/399-8000-1-ND/3471723" H 5150 3850 50  0001 C CNN "Digikey Link"
F 5 "0.1µF ±20% 25V X7R 0805 (2012 Metric)" H 0   0   50  0001 C CNN "Description"
F 6 "399-8000-1-ND" H 0   0   50  0001 C CNN "Digi-Key Part Number"
	1    5150 3850
	-1   0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L101
U 1 1 5F4B9710
P 5750 2950
F 0 "L101" H 5750 3165 50  0000 C CNN
F 1 "1U" H 5750 3074 50  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_NR-50xx_HandSoldering" H 5750 2950 50  0001 C CNN
F 3 "https://www.yuden.co.jp/productdata/catalog/wound04_e.pdf" H 5750 2950 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/taiyo-yuden/NRS5020T1R0NMGJ/587-2403-1-ND/2230269" H 5750 2950 50  0001 C CNN "Digikey Link"
F 5 "FIXED IND 1UH 3.6A 25.2 MOHM SMD" H 0   0   50  0001 C CNN "Description"
F 6 "587-2403-1-ND" H 0   0   50  0001 C CNN "Digi-Key Part Number"
	1    5750 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5F4BEA89
P 6350 3750
F 0 "#PWR0105" H 6350 3500 50  0001 C CNN
F 1 "GND" H 6355 3577 50  0000 C CNN
F 2 "" H 6350 3750 50  0001 C CNN
F 3 "" H 6350 3750 50  0001 C CNN
	1    6350 3750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW101
U 1 1 5F4C134D
P 3950 3750
F 0 "SW101" V 3900 3650 50  0000 R CNN
F 1 "EN" V 4000 3650 50  0000 R CNN
F 2 "digikey-footprints:Switch_Slide_JS202011SCQN" H 3950 3750 50  0001 C CNN
F 3 "https://www.ckswitches.com/media/1422/js.pdf" H 3950 3750 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/c-k/JS202011SCQN/401-2002-1-ND/1640098" H 3950 3750 50  0001 C CNN "Digikey Link"
F 5 "SWITCH SLIDE DPDT 300MA 6V" H -250 0   50  0001 C CNN "Description"
F 6 "401-2002-1-ND" H -250 0   50  0001 C CNN "Digi-Key Part Number"
	1    3950 3750
	0    1    1    0   
$EndComp
$Comp
L Device:LED D102
U 1 1 5F4C82CF
P 7150 4900
F 0 "D102" V 7200 4750 50  0000 R CNN
F 1 "P_GOOD" V 7100 4750 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 7150 4900 50  0001 C CNN
F 3 "https://optoelectronics.liteon.com/upload/download/DS22-2000-226/LTST-S270KGKT.pdf" H 7150 4900 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/w%C3%BCrth-elektronik/150080VS75000/732-4986-1-ND/4489922" H 7150 4900 50  0001 C CNN "Digikey Link"
F 5 "LED GREEN CLEAR 0805 SMD" H 0   0   50  0001 C CNN "Description"
F 6 "732-4986-1-ND" H 0   0   50  0001 C CNN "Digi-Key Part Number"
	1    7150 4900
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R104
U 1 1 5F4C82D6
P 7150 5150
F 0 "R104" H 7300 5200 50  0000 L CNN
F 1 "270" H 7300 5100 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7150 5150 50  0001 C CNN
F 3 "" H 7150 5150 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/susumu/RR1220P-271-D/RR12P270DCT-ND/432816" H 7150 5150 50  0001 C CNN "Digikey Link"
F 5 "RES SMD 270 OHM 0.5% 1/10W 0805" H 0   0   50  0001 C CNN "Description"
F 6 "RR12P270DCT-ND" H 0   0   50  0001 C CNN "Digi-Key Part Number"
	1    7150 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5F4CC68F
P 7150 5250
F 0 "#PWR0115" H 7150 5000 50  0001 C CNN
F 1 "GND" H 7155 5077 50  0000 C CNN
F 2 "" H 7150 5250 50  0001 C CNN
F 3 "" H 7150 5250 50  0001 C CNN
	1    7150 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R101
U 1 1 5F7F24DD
P 2950 3950
F 0 "R101" H 3050 3950 50  0000 L CNN
F 1 "100K" H 3050 3850 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 2950 3950 50  0001 C CNN
F 3 "" H 2950 3950 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/susumu/RR1220P-104-D/RR12P100KDCT-ND/432878" H 2950 3950 50  0001 C CNN "Digikey Link"
F 5 "RES SMD 100K OHM 0.5% 1/10W 0805" H -200 0   50  0001 C CNN "Description"
F 6 "RR12P100KDCT-ND" H -200 0   50  0001 C CNN "Digi-Key Part Number"
	1    2950 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5F7F25B0
P 2950 4050
F 0 "#PWR0110" H 2950 3800 50  0001 C CNN
F 1 "GND" H 2955 3877 50  0000 C CNN
F 2 "" H 2950 4050 50  0001 C CNN
F 3 "" H 2950 4050 50  0001 C CNN
	1    2950 4050
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:TP0610T Q101
U 1 1 5F7F3597
P 2950 3650
F 0 "Q101" V 3300 3800 50  0000 R CNN
F 1 "TP0610T" V 3200 3800 50  0000 R CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3150 3575 50  0001 L CIN
F 3 "https://www.vishay.com/docs/72014/72014.pdf" H 2950 3650 50  0001 L CNN
F 4 "https://www.digikey.com.au/product-detail/en/vishay-siliconix/SI2315BDS-T1-GE3/SI2315BDS-T1-GE3CT-ND/6595280" H 2950 3650 50  0001 C CNN "Digikey Link"
F 5 "MOSFET P-CH 12V 3A SOT23-3" H -200 0   50  0001 C CNN "Description"
F 6 "SI2315BDS-T1-GE3CT-ND" H -200 0   50  0001 C CNN "Digi-Key Part Number"
	1    2950 3650
	0    -1   -1   0   
$EndComp
$Comp
L Device:C_Small C101
U 1 1 604D825D
P 4550 3650
F 0 "C101" H 4450 3700 50  0000 R CNN
F 1 "22U_25V" H 4450 3600 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4550 3650 50  0001 C CNN
F 3 "" H 4550 3650 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 4550 3650 50  0001 C CNN "Digikey Link"
F 5 "22UF ±20% 25V X5R 0805 (2012 Metric)" H 50  0   50  0001 C CNN "Description"
F 6 "1276-CL21A226MAYNNNECT-ND" H 50  0   50  0001 C CNN "Digi-Key Part Number"
	1    4550 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C102
U 1 1 604E0D34
P 6350 3650
F 0 "C102" H 6442 3696 50  0000 L CNN
F 1 "22U_25V" H 6442 3605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6350 3650 50  0001 C CNN
F 3 "" H 6350 3650 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 6350 3650 50  0001 C CNN "Digikey Link"
F 5 "22UF ±20% 25V X5R 0805 (2012 Metric)" H -300 0   50  0001 C CNN "Description"
F 6 "1276-CL21A226MAYNNNECT-ND" H -300 0   50  0001 C CNN "Digi-Key Part Number"
	1    6350 3650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J101
U 1 1 604D70E4
P 950 3550
F 0 "J101" H 950 3800 50  0000 L CNN
F 1 "PWR_IN" H 950 3700 50  0000 L CNN
F 2 "Connector_JST:JST_PH_S2B-PH-SM4-TB_1x02-1MP_P2.00mm_Horizontal" H 950 3550 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1699381.pdf" H 950 3550 50  0001 C CNN
F 4 "https://au.element14.com/jst-japan-solderless-terminals/s2b-ph-sm4-tb-lf-sn/connector-header-smt-r-a-2mm-2way/dp/9492615" H 950 3550 50  0001 C CNN "Digikey Link"
	1    950  3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 604D7EA7
P 1150 3650
F 0 "#PWR0102" H 1150 3400 50  0001 C CNN
F 1 "GND" H 1155 3477 50  0000 C CNN
F 2 "" H 1150 3650 50  0001 C CNN
F 3 "" H 1150 3650 50  0001 C CNN
	1    1150 3650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J102
U 1 1 604D92F9
P 8600 3550
F 0 "J102" H 8573 3480 50  0000 R CNN
F 1 "PWR_OUT_P" H 8573 3571 50  0000 R CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x01_P1.00mm_Vertical_SMD_CUSTOM" H 8600 3550 50  0001 C CNN
F 3 "" H 8600 3550 50  0001 C CNN
	1    8600 3550
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J103
U 1 1 604DC290
P 8600 3700
F 0 "J103" H 8573 3630 50  0000 R CNN
F 1 "PWR_OUT_N" H 8573 3721 50  0000 R CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x01_P1.00mm_Vertical_SMD_CUSTOM" H 8600 3700 50  0001 C CNN
F 3 "" H 8600 3700 50  0001 C CNN
	1    8600 3700
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R102
U 1 1 6087A022
P 3950 4050
F 0 "R102" H 3850 4100 50  0000 R CNN
F 1 "100K" H 3850 4000 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 3950 4050 50  0001 C CNN
F 3 "" H 3950 4050 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/susumu/RR1220P-104-D/RR12P100KDCT-ND/432878" H 3950 4050 50  0001 C CNN "Digikey Link"
F 5 "RES SMD 100K OHM 0.5% 1/10W 0805" H -250 0   50  0001 C CNN "Description"
F 6 "RR12P100KDCT-ND" H -250 0   50  0001 C CNN "Digi-Key Part Number"
	1    3950 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 6087AECD
P 3950 4150
F 0 "#PWR0111" H 3950 3900 50  0001 C CNN
F 1 "GND" H 3955 3977 50  0000 C CNN
F 2 "" H 3950 4150 50  0001 C CNN
F 3 "" H 3950 4150 50  0001 C CNN
	1    3950 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C103
U 1 1 6088DE45
P 6900 3650
F 0 "C103" H 6992 3696 50  0000 L CNN
F 1 "22U_25V" H 6992 3605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6900 3650 50  0001 C CNN
F 3 "" H 6900 3650 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 6900 3650 50  0001 C CNN "Digikey Link"
F 5 "22UF ±20% 25V X5R 0805 (2012 Metric)" H -300 0   50  0001 C CNN "Description"
F 6 "1276-CL21A226MAYNNNECT-ND" H -300 0   50  0001 C CNN "Digi-Key Part Number"
	1    6900 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C104
U 1 1 6088ECFA
P 7450 3650
F 0 "C104" H 7542 3696 50  0000 L CNN
F 1 "22U_25V" H 7542 3605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7450 3650 50  0001 C CNN
F 3 "" H 7450 3650 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 7450 3650 50  0001 C CNN "Digikey Link"
F 5 "22UF ±20% 25V X5R 0805 (2012 Metric)" H -300 0   50  0001 C CNN "Description"
F 6 "1276-CL21A226MAYNNNECT-ND" H -300 0   50  0001 C CNN "Digi-Key Part Number"
	1    7450 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 60894417
P 6900 3750
F 0 "#PWR0106" H 6900 3500 50  0001 C CNN
F 1 "GND" H 6905 3577 50  0000 C CNN
F 2 "" H 6900 3750 50  0001 C CNN
F 3 "" H 6900 3750 50  0001 C CNN
	1    6900 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 60895236
P 7450 3750
F 0 "#PWR0107" H 7450 3500 50  0001 C CNN
F 1 "GND" H 7455 3577 50  0000 C CNN
F 2 "" H 7450 3750 50  0001 C CNN
F 3 "" H 7450 3750 50  0001 C CNN
	1    7450 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 608A38C1
P 8400 3700
F 0 "#PWR0103" H 8400 3450 50  0001 C CNN
F 1 "GND" H 8405 3527 50  0000 C CNN
F 2 "" H 8400 3700 50  0001 C CNN
F 3 "" H 8400 3700 50  0001 C CNN
	1    8400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2950 6150 2950
Wire Wire Line
	6150 2950 6150 3350
$Comp
L Device:Q_PMOS_SDG Q102
U 1 1 608D6E29
P 7050 4550
F 0 "Q102" H 7300 4500 50  0000 L CNN
F 1 "TP0610T" H 7300 4600 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7250 4650 50  0001 C CNN
F 3 "https://www.vishay.com/docs/72014/72014.pdf" H 7050 4550 50  0001 C CNN
F 4 "MOSFET P-CH 12V 3A SOT23-3" H 0   0   50  0001 C CNN "Description"
F 5 "SI2315BDS-T1-GE3CT-ND" H 0   0   50  0001 C CNN "Digi-Key Part Number"
F 6 "https://www.digikey.com.au/product-detail/en/vishay-siliconix/SI2315BDS-T1-GE3/SI2315BDS-T1-GE3CT-ND/6595280" H 0   0   50  0001 C CNN "Digikey Link"
	1    7050 4550
	1    0    0    1   
$EndComp
$Comp
L Device:R_Small R103
U 1 1 608D9063
P 6850 4400
F 0 "R103" H 6750 4500 50  0000 R CNN
F 1 "1M" H 6750 4400 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 6850 4400 50  0001 C CNN
F 3 "" H 6850 4400 50  0001 C CNN
F 4 "RES 1M OHM 0.5% 1/8W 0805" H 0   0   50  0001 C CNN "Description"
F 5 "P123796CT-ND" H 0   0   50  0001 C CNN "Digi-Key Part Number"
F 6 "https://www.digikey.com.au/product-detail/en/panasonic-electronic-components/ERA-6AED105V/P123796CT-ND/9467725" H 0   0   50  0001 C CNN "Digikey Link"
	1    6850 4400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0112
U 1 1 608DF730
P 7150 4300
F 0 "#PWR0112" H 7150 4150 50  0001 C CNN
F 1 "+5V" H 7165 4473 50  0000 C CNN
F 2 "" H 7150 4300 50  0001 C CNN
F 3 "" H 7150 4300 50  0001 C CNN
	1    7150 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 4300 7150 4350
Text Label 6750 4550 2    50   ~ 0
PG
Text Label 6150 4050 0    50   ~ 0
PG
Wire Wire Line
	6850 4550 6850 4500
Wire Wire Line
	6850 4300 7150 4300
Connection ~ 7150 4300
$Comp
L power:+5V #PWR0101
U 1 1 608F31A6
P 7450 3550
F 0 "#PWR0101" H 7450 3400 50  0001 C CNN
F 1 "+5V" H 7465 3723 50  0000 C CNN
F 2 "" H 7450 3550 50  0001 C CNN
F 3 "" H 7450 3550 50  0001 C CNN
	1    7450 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 608F59D3
P 5750 4350
F 0 "#PWR0113" H 5750 4100 50  0001 C CNN
F 1 "GND" H 5755 4177 50  0000 C CNN
F 2 "" H 5750 4350 50  0001 C CNN
F 3 "" H 5750 4350 50  0001 C CNN
	1    5750 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4350 5750 4350
$Comp
L power:GND #PWR0109
U 1 1 608F9F47
P 5350 3950
F 0 "#PWR0109" H 5350 3700 50  0001 C CNN
F 1 "GND" H 5355 3777 50  0000 C CNN
F 2 "" H 5350 3950 50  0001 C CNN
F 3 "" H 5350 3950 50  0001 C CNN
	1    5350 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3750 6150 3550
Wire Wire Line
	5350 3750 5150 3750
Text Label 5350 3650 2    50   ~ 0
EN
Text Label 4050 3950 0    50   ~ 0
EN
Wire Wire Line
	4050 3950 3950 3950
Connection ~ 3950 3950
Connection ~ 2950 3850
Text Label 5350 3550 2    50   ~ 0
VCC_IN_SAFE
Text Label 6150 3550 0    50   ~ 0
VCC_5V0
Text Label 5350 3750 2    50   ~ 0
VAUX
Wire Wire Line
	5350 2950 5500 2950
Connection ~ 6150 3550
Connection ~ 5750 4350
Wire Wire Line
	5350 3350 5350 2950
$Comp
L Regulator_Switching:TPS63060 U101
U 1 1 5F4B6990
P 5750 3750
F 0 "U101" H 5750 4417 50  0000 C CNN
F 1 "TPS63061DSCT" H 5750 4326 50  0000 C CNN
F 2 "Package_SON:Texas_S-PWSON-N10" H 5750 3100 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tps63061.pdf" H 5750 3750 50  0001 C CNN
F 4 "296-30179-1-ND" H 5750 3750 50  0001 C CNN "Digi-Key Part Number"
F 5 "IC REG BCK BST 5V 2A 10WSON" H 5750 3750 50  0001 C CNN "Description"
F 6 "https://www.digikey.com.au/product-detail/en/texas-instruments/TPS63061DSCT/296-30179-1-ND/2812734" H 5750 3750 50  0001 C CNN "Digikey Link"
	1    5750 3750
	1    0    0    -1  
$EndComp
Text Label 1300 3550 0    50   ~ 0
VCC_IN
Wire Wire Line
	1150 3550 1800 3550
$Comp
L power:GND #PWR0104
U 1 1 609166D8
P 4550 3750
F 0 "#PWR0104" H 4550 3500 50  0001 C CNN
F 1 "GND" H 4555 3577 50  0000 C CNN
F 2 "" H 4550 3750 50  0001 C CNN
F 3 "" H 4550 3750 50  0001 C CNN
	1    4550 3750
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP104
U 1 1 609167CE
P 5400 5700
F 0 "TP104" H 5450 5850 50  0000 L CNN
F 1 "TP_PG" H 5450 5750 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 5600 5700 50  0001 C CNN
F 3 "" H 5600 5700 50  0001 C CNN
	1    5400 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP102
U 1 1 609173DD
P 5400 5250
F 0 "TP102" H 5450 5400 50  0000 L CNN
F 1 "TP_5V0" H 5450 5300 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 5600 5250 50  0001 C CNN
F 3 "" H 5600 5250 50  0001 C CNN
	1    5400 5250
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP105
U 1 1 6091762A
P 4650 6050
F 0 "TP105" H 4700 6200 50  0000 L CNN
F 1 "TP_POL" H 4700 6100 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 4850 6050 50  0001 C CNN
F 3 "" H 4850 6050 50  0001 C CNN
	1    4650 6050
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP101
U 1 1 60917D71
P 4650 5250
F 0 "TP101" H 4700 5400 50  0000 L CNN
F 1 "TP_GND" H 4700 5300 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 4850 5250 50  0001 C CNN
F 3 "" H 4850 5250 50  0001 C CNN
	1    4650 5250
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP103
U 1 1 60917E6C
P 4650 5700
F 0 "TP103" H 4700 5850 50  0000 L CNN
F 1 "TP_IN_SAFE" H 4700 5750 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 4850 5700 50  0001 C CNN
F 3 "" H 4850 5700 50  0001 C CNN
	1    4650 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 4550 6850 4550
Connection ~ 6850 4550
Text Label 2850 3850 2    50   ~ 0
POL_MOS_CTRL
Wire Wire Line
	2850 3850 2950 3850
Text Label 5400 5700 2    50   ~ 0
PG
Text Label 4650 5700 2    50   ~ 0
VCC_IN_SAFE
Text Label 4650 6050 2    50   ~ 0
POL_MOS_CTRL
Text Label 5400 5250 2    50   ~ 0
VCC_5V0
$Comp
L power:GND #PWR0114
U 1 1 609199C0
P 4650 5250
F 0 "#PWR0114" H 4650 5000 50  0001 C CNN
F 1 "GND" H 4655 5077 50  0000 C CNN
F 2 "" H 4650 5250 50  0001 C CNN
F 3 "" H 4650 5250 50  0001 C CNN
	1    4650 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 3550 3250 3550
$Comp
L Device:D_TVS D101
U 1 1 6091C99C
P 3250 3700
F 0 "D101" V 3204 3779 50  0000 L CNN
F 1 "D_TVS" V 3295 3779 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-323" H 3250 3700 50  0001 C CNN
F 3 "" H 3250 3700 50  0001 C CNN
F 4 "TVS DIODE 5V 7.2V SOD323" H -200 0   50  0001 C CNN "Description"
F 5 "DESD5V0U1BA-7DICT-ND" H -200 0   50  0001 C CNN "Digi-Key Part Number"
F 6 "https://www.digikey.com.au/product-detail/en/diodes-incorporated/DESD5V0U1BA-7/DESD5V0U1BA-7DICT-ND/5371911" H -200 0   50  0001 C CNN "Digikey Link"
	1    3250 3700
	0    1    1    0   
$EndComp
Connection ~ 3250 3550
Wire Wire Line
	3250 3850 2950 3850
$Comp
L Device:Fuse_Small F101
U 1 1 6091E05E
P 1900 3550
F 0 "F101" H 1900 3735 50  0000 C CNN
F 1 "FUSE 2A" H 1900 3644 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 1900 3550 50  0001 C CNN
F 3 "" H 1900 3550 50  0001 C CNN
F 4 "FUSE 2.0A 125VAC FAST 1206" H -750 0   50  0001 C CNN "Description"
F 5 "507-1881-1-ND" H -750 0   50  0001 C CNN "Digi-Key Part Number"
F 6 "https://www.digikey.com.au/product-detail/en/bel-fuse-inc/C1F-2/507-1881-1-ND/4968159" H -750 0   50  0001 C CNN "Digikey Link"
	1    1900 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3550 3950 3550
Connection ~ 6350 3550
Wire Wire Line
	6350 3550 6150 3550
Connection ~ 6900 3550
Connection ~ 7450 3550
Wire Wire Line
	6900 3550 7450 3550
Wire Wire Line
	6350 3550 6900 3550
Wire Wire Line
	7450 3550 8400 3550
Connection ~ 4550 3550
Wire Wire Line
	4550 3550 5350 3550
Connection ~ 3950 3550
Wire Wire Line
	3950 3550 4550 3550
Text Label 2150 3550 0    50   ~ 0
VCC_IN_FUSED
Wire Wire Line
	2000 3550 2750 3550
Text Label 5350 3100 2    50   ~ 0
L1
Text Label 6150 3100 0    50   ~ 0
L2
$EndSCHEMATC
