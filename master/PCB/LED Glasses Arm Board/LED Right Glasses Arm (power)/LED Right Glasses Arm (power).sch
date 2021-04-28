EESchema Schematic File Version 4
LIBS:LED Right Glasses Arm (power)-cache
EELAYER 26 0
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
Text Notes 4350 2950 0    50   ~ 0
Power Regulator Circuit\n
Text Label 1050 4050 0    50   ~ 0
VIN_2.5-12V
Wire Wire Line
	4350 4250 4000 4250
$Comp
L power:GND #PWR0101
U 1 1 5F4B6F4F
P 4300 4500
F 0 "#PWR0101" H 4300 4250 50  0001 C CNN
F 1 "GND" H 4305 4327 50  0000 C CNN
F 2 "" H 4300 4500 50  0001 C CNN
F 3 "" H 4300 4500 50  0001 C CNN
	1    4300 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5F4B6F73
P 4000 4450
F 0 "#PWR0102" H 4000 4200 50  0001 C CNN
F 1 "GND" H 4005 4277 50  0000 C CNN
F 2 "" H 4000 4450 50  0001 C CNN
F 3 "" H 4000 4450 50  0001 C CNN
	1    4000 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5F4B6FC9
P 4650 4850
F 0 "#PWR0104" H 4650 4600 50  0001 C CNN
F 1 "GND" H 4655 4677 50  0000 C CNN
F 2 "" H 4650 4850 50  0001 C CNN
F 3 "" H 4650 4850 50  0001 C CNN
	1    4650 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5F4B8E20
P 4000 4350
F 0 "C2" H 4092 4396 50  0000 L CNN
F 1 "0.1u" H 4092 4305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4000 4350 50  0001 C CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL05A104KA5NNNC.pdf" H 4000 4350 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL05A104KA5NNNC/1276-1043-1-ND/3889129" H 4000 4350 50  0001 C CNN "Digikey Link"
	1    4000 4350
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Switching:TPS63060 U1
U 1 1 5F4B6990
P 4750 4250
F 0 "U1" H 4750 4917 50  0000 C CNN
F 1 "TPS63061DSCT" H 4750 4826 50  0000 C CNN
F 2 "Package_SON:Texas_S-PWSON-N10" H 4750 3600 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tps63061.pdf?HQS=dis-dk-null-digikeymode-dsf-pf-null-wwe&ts=1618215570038&ref_url=https%253A%252F%252Fwww.digikey.com.au%252F" H 4750 4250 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/texas-instruments/TPS63061DSCT/296-30179-1-ND/2812734" H 4750 4250 50  0001 C CNN "Digikey Link"
	1    4750 4250
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L1
U 1 1 5F4B9710
P 4750 3300
F 0 "L1" H 4750 3515 50  0000 C CNN
F 1 "1u" H 4750 3424 50  0000 C CNN
F 2 "sft:SRN4018TA" H 4750 3300 50  0001 C CNN
F 3 "" H 4750 3300 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/taiyo-yuden/NRS5020T1R0NMGJ/587-2403-1-ND/2230269" H 4750 3300 50  0001 C CNN "Digikey Link"
	1    4750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3850 4350 3300
Wire Wire Line
	4350 3300 4500 3300
Wire Wire Line
	5000 3300 5150 3300
Wire Wire Line
	5150 3300 5150 3850
$Comp
L power:GND #PWR0108
U 1 1 5F4BEA89
P 5450 4350
F 0 "#PWR0108" H 5450 4100 50  0001 C CNN
F 1 "GND" H 5455 4177 50  0000 C CNN
F 2 "" H 5450 4350 50  0001 C CNN
F 3 "" H 5450 4350 50  0001 C CNN
	1    5450 4350
	1    0    0    -1  
$EndComp
Text Label 6400 4050 0    50   ~ 0
5V0
$Comp
L Switch:SW_SPST SW1
U 1 1 5F4C134D
P 3600 4250
F 0 "SW1" V 3550 4100 50  0000 C CNN
F 1 "EN" V 3650 4100 50  0000 C CNN
F 2 "digikey-footprints:Switch_Slide_JS202011SCQN" H 3600 4250 50  0001 C CNN
F 3 "https://www.ckswitches.com/media/1422/js.pdf" H 3600 4250 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/c-k/JS202011SCQN/401-2002-1-ND/1640098" H 3600 4250 50  0001 C CNN "Digikey Link"
	1    3600 4250
	0    1    1    0   
$EndComp
$Comp
L Device:LED D3
U 1 1 5F4C82CF
P 6400 4600
F 0 "D3" H 6392 4816 50  0000 C CNN
F 1 "P_OUT" H 6392 4725 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 6400 4600 50  0001 C CNN
F 3 "https://optoelectronics.liteon.com/upload/download/DS22-2000-226/LTST-S270KGKT.pdf" H 6400 4600 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/lite-on-inc/LTST-S270KGKT/160-1478-1-ND/386892" H 6400 4600 50  0001 C CNN "Digikey Link"
	1    6400 4600
	0    1    -1   0   
$EndComp
$Comp
L Device:R_Small R6
U 1 1 5F4C82D6
P 6400 5050
F 0 "R6" H 6468 5096 50  0000 L CNN
F 1 "2k2" H 6468 5005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6400 5050 50  0001 C CNN
F 3 "https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=1773204-3&DocType=DS&DocLang=English" H 6400 5050 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/te-connectivity-passive-product/CRGCQ0603J2K2/A130093CT-ND/8577925" H 6400 5050 50  0001 C CNN "Digikey Link"
	1    6400 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 4750 6400 4950
Wire Wire Line
	6400 5150 6400 5650
Wire Wire Line
	6400 4050 6400 4450
$Comp
L power:GND #PWR0109
U 1 1 5F4CC68F
P 6400 5650
F 0 "#PWR0109" H 6400 5400 50  0001 C CNN
F 1 "GND" H 6405 5477 50  0000 C CNN
F 2 "" H 6400 5650 50  0001 C CNN
F 3 "" H 6400 5650 50  0001 C CNN
	1    6400 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Zener_Small D1
U 1 1 5F7F1725
P 2100 4300
F 0 "D1" V 2054 4368 50  0000 L CNN
F 1 "6V" V 2145 4368 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123" V 2100 4300 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85774/mmsz5225.pdf" V 2100 4300 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/vishay-general-semiconductor-diodes-division/MMSZ5233B-E3-08/112-MMSZ5233B-E3-08CT-ND/8565146" V 2100 4300 50  0001 C CNN "Digikey Link"
	1    2100 4300
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R1
U 1 1 5F7F24DD
P 1900 4600
F 0 "R1" H 1968 4646 50  0000 L CNN
F 1 "100k" H 1968 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1900 4600 50  0001 C CNN
F 3 "https://www.susumu.co.jp/common/pdf/n_catalog_partition05_en.pdf" H 1900 4600 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/susumu/RR0816P-104-D/RR08P100KDCT-ND/432772" H 1900 4600 50  0001 C CNN "Digikey Link"
	1    1900 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5F7F25B0
P 1900 4950
F 0 "#PWR0110" H 1900 4700 50  0001 C CNN
F 1 "GND" H 1905 4777 50  0000 C CNN
F 2 "" H 1900 4950 50  0001 C CNN
F 3 "" H 1900 4950 50  0001 C CNN
	1    1900 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4700 1900 4950
Wire Wire Line
	1900 4500 2100 4500
$Comp
L Transistor_FET:TP0610T Q1
U 1 1 5F7F3597
P 1900 4150
F 0 "Q1" H 2106 4196 50  0000 L CNN
F 1 "TP0610T" H 2106 4105 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2100 4075 50  0001 L CIN
F 3 "https://www.vishay.com/docs/72014/72014.pdf" H 1900 4150 50  0001 L CNN
F 4 "https://www.digikey.com.au/product-detail/en/vishay-siliconix/SI2315BDS-T1-GE3/SI2315BDS-T1-GE3CT-ND/6595280" H 1900 4150 50  0001 C CNN "Digikey Link"
	1    1900 4150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2100 4050 2350 4050
Wire Wire Line
	1900 4350 1900 4500
Wire Wire Line
	4750 4850 4650 4850
Connection ~ 4650 4850
$Comp
L Device:C_Small C5
U 1 1 604D825D
P 2800 4450
F 0 "C5" H 2892 4496 50  0000 L CNN
F 1 "22u" H 2892 4405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2800 4450 50  0001 C CNN
F 3 "" H 2800 4450 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 2800 4450 50  0001 C CNN "Digikey Link"
	1    2800 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 4050 1700 4050
$Comp
L Device:C_Small C6
U 1 1 604E0D34
P 5450 4250
F 0 "C6" H 5542 4296 50  0000 L CNN
F 1 "22u" H 5542 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5450 4250 50  0001 C CNN
F 3 "" H 5450 4250 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 5450 4250 50  0001 C CNN "Digikey Link"
	1    5450 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4050 5450 4150
Wire Wire Line
	5450 4050 5750 4050
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 604D70E4
P 850 4050
F 0 "J1" H 956 4228 50  0000 C CNN
F 1 "PWR IN" H 956 4137 50  0000 C CNN
F 2 "Connector_JST:JST_PH_S2B-PH-SM4-TB_1x02-1MP_P2.00mm_Horizontal" H 850 4050 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1699381.pdf" H 850 4050 50  0001 C CNN
F 4 "https://au.element14.com/jst-japan-solderless-terminals/s2b-ph-sm4-tb-lf-sn/connector-header-smt-r-a-2mm-2way/dp/9492615" H 850 4050 50  0001 C CNN "Link"
F 5 "https://www.digikey.com.au/product-detail/en/jst-sales-america-inc/S2B-PH-SM4-TB-LF-SN/455-1749-2-ND/926655" H 850 4050 50  0001 C CNN "Digikey Link"
	1    850  4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 4150 1300 4150
Wire Wire Line
	1300 4150 1300 5000
$Comp
L power:GND #PWR0105
U 1 1 604D7EA7
P 1300 5000
F 0 "#PWR0105" H 1300 4750 50  0001 C CNN
F 1 "GND" H 1305 4827 50  0000 C CNN
F 2 "" H 1300 5000 50  0001 C CNN
F 3 "" H 1300 5000 50  0001 C CNN
	1    1300 5000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J2
U 1 1 604D92F9
P 6800 4050
F 0 "J2" H 6773 3980 50  0000 R CNN
F 1 "PWR OUT +" H 6773 4071 50  0000 R CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x01_P1.00mm_Vertical_SMD_CUSTOM" H 6800 4050 50  0001 C CNN
F 3 "~" H 6800 4050 50  0001 C CNN
	1    6800 4050
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J3
U 1 1 604DC290
P 6800 4200
F 0 "J3" H 6773 4130 50  0000 R CNN
F 1 "PWR OUT -" H 6773 4221 50  0000 R CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x01_P1.00mm_Vertical_SMD_CUSTOM" H 6800 4200 50  0001 C CNN
F 3 "~" H 6800 4200 50  0001 C CNN
	1    6800 4200
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5F4B6F97
P 2800 4750
F 0 "#PWR0103" H 2800 4500 50  0001 C CNN
F 1 "GND" H 2805 4577 50  0000 C CNN
F 2 "" H 2800 4750 50  0001 C CNN
F 3 "" H 2800 4750 50  0001 C CNN
	1    2800 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4550 2800 4750
Wire Wire Line
	2800 4050 2800 4350
$Comp
L Connector:TestPoint TP3
U 1 1 60740989
P 6050 4050
F 0 "TP3" H 6108 4170 50  0000 L CNN
F 1 "VOUT" H 6108 4079 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 6250 4050 50  0001 C CNN
F 3 "~" H 6250 4050 50  0001 C CNN
	1    6050 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP1
U 1 1 60741617
P 2350 4000
F 0 "TP1" H 2408 4120 50  0000 L CNN
F 1 "VIN" H 2408 4029 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 2550 4000 50  0001 C CNN
F 3 "~" H 2550 4000 50  0001 C CNN
	1    2350 4000
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP2
U 1 1 60742752
P 1300 4150
F 0 "TP2" V 1300 4350 50  0000 L CNN
F 1 "GND" V 1400 4350 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 1500 4150 50  0001 C CNN
F 3 "~" H 1500 4150 50  0001 C CNN
	1    1300 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 4500 2100 4400
Connection ~ 2100 4050
Wire Wire Line
	2100 4200 2100 4050
Wire Wire Line
	2350 4000 2350 4050
Connection ~ 2350 4050
Wire Wire Line
	5150 4050 5250 4050
Wire Wire Line
	5150 4250 5250 4250
Wire Wire Line
	5250 4250 5250 4050
Wire Wire Line
	2350 4050 2800 4050
Connection ~ 2800 4050
Wire Wire Line
	2800 4050 3600 4050
$Comp
L Device:R_Small R2
U 1 1 6087A022
P 3600 4650
F 0 "R2" H 3450 4700 50  0000 L CNN
F 1 "100k" H 3300 4600 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 3600 4650 50  0001 C CNN
F 3 "https://www.susumu.co.jp/common/pdf/n_catalog_partition05_en.pdf" H 3600 4650 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/susumu/RR0816P-104-D/RR08P100KDCT-ND/432772" H 3600 4650 50  0001 C CNN "Digikey Link"
	1    3600 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6087AECD
P 3600 4750
F 0 "#PWR?" H 3600 4500 50  0001 C CNN
F 1 "GND" H 3605 4577 50  0000 C CNN
F 2 "" H 3600 4750 50  0001 C CNN
F 3 "" H 3600 4750 50  0001 C CNN
	1    3600 4750
	1    0    0    -1  
$EndComp
Connection ~ 3600 4050
Wire Wire Line
	3600 4050 4350 4050
Wire Wire Line
	3600 4500 3700 4500
Wire Wire Line
	3700 4500 3700 4150
Wire Wire Line
	3700 4150 4350 4150
Wire Wire Line
	4350 4450 4300 4450
Wire Wire Line
	4300 4450 4300 4500
Connection ~ 3600 4500
Wire Wire Line
	3600 4500 3600 4550
Wire Wire Line
	3600 4450 3600 4500
Connection ~ 1300 4150
$Comp
L Device:C_Small C?
U 1 1 6088DE45
P 5750 4250
F 0 "C?" H 5842 4296 50  0000 L CNN
F 1 "22u" H 5842 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5750 4250 50  0001 C CNN
F 3 "" H 5750 4250 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 5750 4250 50  0001 C CNN "Digikey Link"
	1    5750 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6088ECFA
P 6050 4250
F 0 "C?" H 6142 4296 50  0000 L CNN
F 1 "22u" H 6142 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 6050 4250 50  0001 C CNN
F 3 "" H 6050 4250 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL21A226MAYNNNE/1276-CL21A226MAYNNNECT-ND/12698679" H 6050 4250 50  0001 C CNN "Digikey Link"
	1    6050 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60894417
P 5750 4350
F 0 "#PWR?" H 5750 4100 50  0001 C CNN
F 1 "GND" H 5755 4177 50  0000 C CNN
F 2 "" H 5750 4350 50  0001 C CNN
F 3 "" H 5750 4350 50  0001 C CNN
	1    5750 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60895236
P 6050 4350
F 0 "#PWR?" H 6050 4100 50  0001 C CNN
F 1 "GND" H 6055 4177 50  0000 C CNN
F 2 "" H 6050 4350 50  0001 C CNN
F 3 "" H 6050 4350 50  0001 C CNN
	1    6050 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4150 5750 4050
Connection ~ 5750 4050
Wire Wire Line
	5750 4050 6050 4050
Wire Wire Line
	6050 4050 6050 4150
Connection ~ 6050 4050
Wire Wire Line
	6050 4050 6400 4050
Wire Wire Line
	5250 4050 5450 4050
Connection ~ 5250 4050
Connection ~ 5450 4050
$Comp
L power:GND #PWR?
U 1 1 608A38C1
P 6600 4200
F 0 "#PWR?" H 6600 3950 50  0001 C CNN
F 1 "GND" H 6605 4027 50  0000 C CNN
F 2 "" H 6600 4200 50  0001 C CNN
F 3 "" H 6600 4200 50  0001 C CNN
	1    6600 4200
	1    0    0    -1  
$EndComp
Connection ~ 6400 4050
Wire Wire Line
	6400 4050 6600 4050
$EndSCHEMATC
