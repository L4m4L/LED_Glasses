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
	4350 4450 4200 4450
Wire Wire Line
	4200 4450 4200 4850
Wire Wire Line
	4350 4250 3800 4250
$Comp
L power:GND #PWR0101
U 1 1 5F4B6F4F
P 4200 4850
F 0 "#PWR0101" H 4200 4600 50  0001 C CNN
F 1 "GND" H 4205 4677 50  0000 C CNN
F 2 "" H 4200 4850 50  0001 C CNN
F 3 "" H 4200 4850 50  0001 C CNN
	1    4200 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5F4B6F73
P 3800 4850
F 0 "#PWR0102" H 3800 4600 50  0001 C CNN
F 1 "GND" H 3805 4677 50  0000 C CNN
F 2 "" H 3800 4850 50  0001 C CNN
F 3 "" H 3800 4850 50  0001 C CNN
	1    3800 4850
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
P 3800 4550
F 0 "C2" H 3892 4596 50  0000 L CNN
F 1 "0.1u" H 3892 4505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3800 4550 50  0001 C CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL05A104KA5NNNC.pdf" H 3800 4550 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL05A104KA5NNNC/1276-1043-1-ND/3889129" H 3800 4550 50  0001 C CNN "Link"
	1    3800 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4250 3800 4450
Wire Wire Line
	3800 4650 3800 4850
$Comp
L Regulator_Switching:TPS63060 U1
U 1 1 5F4B6990
P 4750 4250
F 0 "U1" H 4750 4917 50  0000 C CNN
F 1 "TPS63060" H 4750 4826 50  0000 C CNN
F 2 "Package_SON:Texas_S-PWSON-N10" H 4750 3600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tps63060.pdf" H 4750 4250 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/texas-instruments/TPS63060DSCR/296-30204-1-ND/2834996" H 4750 4250 50  0001 C CNN "Digikey Link"
	1    4750 4250
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L1
U 1 1 5F4B9710
P 4750 3300
F 0 "L1" H 4750 3515 50  0000 C CNN
F 1 "1uF" H 4750 3424 50  0000 C CNN
F 2 "sft:SRN4018TA" H 4750 3300 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/SRN4018TA.pdf" H 4750 3300 50  0001 C CNN
F 4 "XFL4020-102MEC" H 4750 3300 50  0001 C CNN "Old product"
F 5 "https://www.digikey.com/en/products/detail/bourns-inc/SRN4018TA-1R0M/6565079" H 4750 3300 50  0001 C CNN "Digikey"
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
Wire Wire Line
	5150 4800 6350 4800
Wire Wire Line
	5150 4050 5250 4050
$Comp
L Device:R_Small_US R3
U 1 1 5F4BA20C
P 5250 4150
F 0 "R3" H 5318 4196 50  0000 L CNN
F 1 "1M" H 5318 4105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5250 4150 50  0001 C CNN
F 3 "https://www.yageo.com/upload/media/product/productsearch/datasheet/rchip/PYu-RT_1-to-0.01_RoHS_L_12.pdf" H 5250 4150 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/yageo/RT0603BRD071ML/YAG4498CT-ND/6616654" H 5250 4150 50  0001 C CNN "Digikey Link"
	1    5250 4150
	1    0    0    -1  
$EndComp
Connection ~ 5250 4050
Wire Wire Line
	5250 4050 6350 4050
Wire Wire Line
	5150 4250 5250 4250
$Comp
L Device:R_Small_US R4
U 1 1 5F4BA598
P 5250 4350
F 0 "R4" H 5318 4396 50  0000 L CNN
F 1 "111k" H 5318 4305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 5250 4350 50  0001 C CNN
F 3 "https://www.vishay.com/docs/60026/ptn.pdf" H 5250 4350 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/vishay-thin-film/PTN0805E1113BST1/764-1475-6-ND/7103531" H 5250 4350 50  0001 C CNN "Digikey Link"
	1    5250 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4550 5150 4800
$Comp
L power:GND #PWR0106
U 1 1 5F4BAEF0
P 5250 4450
F 0 "#PWR0106" H 5250 4200 50  0001 C CNN
F 1 "GND" H 5255 4277 50  0000 C CNN
F 2 "" H 5250 4450 50  0001 C CNN
F 3 "" H 5250 4450 50  0001 C CNN
	1    5250 4450
	1    0    0    -1  
$EndComp
Connection ~ 5250 4250
$Comp
L Device:C_Small C3
U 1 1 5F4BB248
P 5950 4350
F 0 "C3" H 6042 4396 50  0000 L CNN
F 1 "10pF" H 6042 4305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5950 4350 50  0001 C CNN
F 3 "https://datasheets.avx.com/C0GNP0-Dielectric.pdf" H 5950 4350 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/avx-corporation/06035A100JAT2A/478-1163-1-ND/564195" H 5950 4350 50  0001 C CNN "Link"
	1    5950 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 4250 5950 4250
$Comp
L power:GND #PWR0107
U 1 1 5F4BBACE
P 5950 4450
F 0 "#PWR0107" H 5950 4200 50  0001 C CNN
F 1 "GND" H 5955 4277 50  0000 C CNN
F 2 "" H 5950 4450 50  0001 C CNN
F 3 "" H 5950 4450 50  0001 C CNN
	1    5950 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R5
U 1 1 5F4BBDC6
P 6350 4400
F 0 "R5" H 6418 4446 50  0000 L CNN
F 1 "1M" H 6418 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6350 4400 50  0001 C CNN
F 3 "https://www.yageo.com/upload/media/product/productsearch/datasheet/rchip/PYu-RT_1-to-0.01_RoHS_L_12.pdf" H 6350 4400 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/yageo/RT0603BRD071ML/YAG4498CT-ND/6616654" H 6350 4400 50  0001 C CNN "Digikey Link"
	1    6350 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 4050 6350 4300
Wire Wire Line
	6350 4500 6350 4800
Connection ~ 6350 4050
$Comp
L power:GND #PWR0108
U 1 1 5F4BEA89
P 7100 4750
F 0 "#PWR0108" H 7100 4500 50  0001 C CNN
F 1 "GND" H 7105 4577 50  0000 C CNN
F 2 "" H 7100 4750 50  0001 C CNN
F 3 "" H 7100 4750 50  0001 C CNN
	1    7100 4750
	1    0    0    -1  
$EndComp
Text Label 8550 4050 0    50   ~ 0
VOUT_5V
Connection ~ 6350 4800
Text Label 6350 5000 0    50   ~ 0
PG
Wire Wire Line
	4350 4150 3700 4150
Wire Wire Line
	3700 4150 3700 5400
Wire Wire Line
	2850 5400 2850 4050
Connection ~ 2850 4050
$Comp
L Switch:SW_SPST SW1
U 1 1 5F4C134D
P 3350 5400
F 0 "SW1" H 3350 5635 50  0000 C CNN
F 1 "SW_SPST" H 3350 5544 50  0000 C CNN
F 2 "digikey-footprints:Switch_Slide_JS202011SCQN" H 3350 5400 50  0001 C CNN
F 3 "https://www.ckswitches.com/media/1422/js.pdf" H 3350 5400 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/c-k/JS202011SCQN/401-2002-1-ND/1640098" H 3350 5400 50  0001 C CNN "Digikey Link"
	1    3350 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 5400 3150 5400
Wire Wire Line
	3550 5400 3700 5400
$Comp
L Device:LED D3
U 1 1 5F4C82CF
P 8050 4600
F 0 "D3" H 8042 4816 50  0000 C CNN
F 1 "LED Green" H 8042 4725 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 8050 4600 50  0001 C CNN
F 3 "https://optoelectronics.liteon.com/upload/download/DS22-2000-226/LTST-S270KGKT.pdf" H 8050 4600 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/lite-on-inc/LTST-S270KGKT/160-1478-1-ND/386892" H 8050 4600 50  0001 C CNN "Digikey Link"
	1    8050 4600
	0    1    -1   0   
$EndComp
$Comp
L Device:R_Small_US R6
U 1 1 5F4C82D6
P 8050 5050
F 0 "R6" H 8118 5096 50  0000 L CNN
F 1 "2k2" H 8118 5005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 8050 5050 50  0001 C CNN
F 3 "https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=1773204-3&DocType=DS&DocLang=English" H 8050 5050 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/te-connectivity-passive-product/CRGCQ0603J2K2/A130093CT-ND/8577925" H 8050 5050 50  0001 C CNN "Digikey Link"
	1    8050 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 4750 8050 4950
Wire Wire Line
	8050 5150 8050 5650
Wire Wire Line
	8050 4050 8050 4450
Connection ~ 8050 4050
$Comp
L power:GND #PWR0109
U 1 1 5F4CC68F
P 8050 5650
F 0 "#PWR0109" H 8050 5400 50  0001 C CNN
F 1 "GND" H 8055 5477 50  0000 C CNN
F 2 "" H 8050 5650 50  0001 C CNN
F 3 "" H 8050 5650 50  0001 C CNN
	1    8050 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5F4CC9A9
P 3850 5400
F 0 "D2" H 3842 5616 50  0000 C CNN
F 1 "LED Red" H 3842 5525 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 3850 5400 50  0001 C CNN
F 3 "https://optoelectronics.liteon.com/upload/download/DS22-2000-210/LTST-S270KRKT.pdf" H 3850 5400 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/lite-on-inc/LTST-S270KRKT/160-1479-1-ND/386894" H 3850 5400 50  0001 C CNN "Digikey Link"
	1    3850 5400
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R2
U 1 1 5F4CC9B0
P 4300 5400
F 0 "R2" V 4505 5400 50  0000 C CNN
F 1 "2k2" V 4414 5400 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4300 5400 50  0001 C CNN
F 3 "https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=1773204-3&DocType=DS&DocLang=English" H 4300 5400 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/te-connectivity-passive-product/CRGCQ0603J2K2/A130093CT-ND/8577925" V 4300 5400 50  0001 C CNN "Digikey Link"
	1    4300 5400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4000 5400 4200 5400
Connection ~ 3700 5400
Wire Wire Line
	6350 4800 6350 5400
Wire Wire Line
	4400 5400 6350 5400
$Comp
L Device:D_Zener_Small 9.1V1
U 1 1 5F7F1725
P 2100 4300
F 0 "9.1V1" V 2054 4368 50  0000 L CNN
F 1 "D_Zener_Small" V 2145 4368 50  0000 L CNN
F 2 "Diode_SMD:D_0603_1608Metric" V 2100 4300 50  0001 C CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Comchip%20PDFs/CZRU52C2-CZRU52C39_DS.pdf" V 2100 4300 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/comchip-technology/CZRU52C9V1/641-1035-1-ND/1121157" V 2100 4300 50  0001 C CNN "Digikey link"
	1    2100 4300
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US 100k1
U 1 1 5F7F24DD
P 1900 4600
F 0 "100k1" H 1968 4646 50  0000 L CNN
F 1 "R_Small_US" H 1968 4555 50  0000 L CNN
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
Wire Wire Line
	2100 4500 2100 4400
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
	2100 4050 2850 4050
Wire Wire Line
	2100 4200 2100 4050
Connection ~ 2100 4050
Wire Wire Line
	1900 4350 1900 4500
Connection ~ 1900 4500
Wire Wire Line
	4750 4850 4650 4850
Connection ~ 4650 4850
$Comp
L Device:C_Small C5
U 1 1 604D825D
P 3250 4450
F 0 "C5" H 3342 4496 50  0000 L CNN
F 1 "22uF" H 3342 4405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3250 4450 50  0001 C CNN
F 3 "https://content.kemet.com/datasheets/KEM_C1006_X5R_SMD.pdf" H 3250 4450 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/kemet/C0805C226M9PAC7210/399-C0805C226M9PAC7210CT-ND/12701503" H 3250 4450 50  0001 C CNN "Link"
	1    3250 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 4050 1700 4050
Wire Wire Line
	8050 4050 8550 4050
$Comp
L Device:C_Small C6
U 1 1 604E0D34
P 7100 4250
F 0 "C6" H 7192 4296 50  0000 L CNN
F 1 "68uF" H 7192 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 7100 4250 50  0001 C CNN
F 3 "https://product.tdk.com/en/system/files?file=dam/doc/product/capacitor/ceramic/mlcc/catalog/mlcc_commercial_general_en.pdf" H 7100 4250 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/tdk-corporation/C3216JB1A686M160AC/445-11687-1-ND/3953353" H 7100 4250 50  0001 C CNN "Link"
	1    7100 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 4050 7100 4050
Wire Wire Line
	7100 4050 7100 4150
Connection ~ 7100 4050
Wire Wire Line
	7100 4050 8050 4050
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 604D70E4
P 850 4050
F 0 "J1" H 956 4228 50  0000 C CNN
F 1 "Conn_01x02_Male" H 956 4137 50  0000 C CNN
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
P 10500 4050
F 0 "J2" H 10473 3980 50  0000 R CNN
F 1 "Conn_01x01_Male" H 10473 4071 50  0000 R CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x01_P1.00mm_Vertical_SMD_CUSTOM" H 10500 4050 50  0001 C CNN
F 3 "~" H 10500 4050 50  0001 C CNN
	1    10500 4050
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 604DB74C
P 8550 5650
F 0 "#PWR0111" H 8550 5400 50  0001 C CNN
F 1 "GND" H 8555 5477 50  0000 C CNN
F 2 "" H 8550 5650 50  0001 C CNN
F 3 "" H 8550 5650 50  0001 C CNN
	1    8550 5650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J3
U 1 1 604DC290
P 10500 4200
F 0 "J3" H 10473 4130 50  0000 R CNN
F 1 "Conn_01x01_Male" H 10473 4221 50  0000 R CNN
F 2 "Connector_PinHeader_1.00mm:PinHeader_1x01_P1.00mm_Vertical_SMD_CUSTOM" H 10500 4200 50  0001 C CNN
F 3 "~" H 10500 4200 50  0001 C CNN
	1    10500 4200
	-1   0    0    1   
$EndComp
Wire Wire Line
	2850 4050 3250 4050
$Comp
L power:GND #PWR0103
U 1 1 5F4B6F97
P 3250 4750
F 0 "#PWR0103" H 3250 4500 50  0001 C CNN
F 1 "GND" H 3255 4577 50  0000 C CNN
F 2 "" H 3250 4750 50  0001 C CNN
F 3 "" H 3250 4750 50  0001 C CNN
	1    3250 4750
	1    0    0    -1  
$EndComp
Connection ~ 3250 4050
Wire Wire Line
	3250 4050 4350 4050
Wire Wire Line
	3250 4550 3250 4750
$Comp
L Device:C_Small C1
U 1 1 60505078
P 8550 4300
F 0 "C1" H 8642 4346 50  0000 L CNN
F 1 "220uF" H 8642 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 8550 4300 50  0001 C CNN
F 3 "~" H 8550 4300 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL32A227MQVNNNE/1276-3375-1-ND/3891461" H 8550 4300 50  0001 C CNN "Link"
	1    8550 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 4200 8550 4050
Connection ~ 8550 4050
Wire Wire Line
	8550 4400 8550 4700
Wire Wire Line
	8550 4700 8550 5650
Wire Wire Line
	3250 4050 3250 4350
Wire Wire Line
	7100 4350 7100 4750
Wire Wire Line
	10300 4700 10300 4200
Connection ~ 8550 4700
Wire Wire Line
	8550 4700 8950 4700
Wire Wire Line
	8950 4400 8950 4700
Connection ~ 8950 4700
Wire Wire Line
	8950 4700 9350 4700
Wire Wire Line
	9350 4400 9350 4700
Connection ~ 9350 4700
Wire Wire Line
	9350 4700 9750 4700
Wire Wire Line
	9750 4400 9750 4700
Connection ~ 9750 4700
Wire Wire Line
	9750 4700 10300 4700
Wire Wire Line
	8550 4050 8950 4050
$Comp
L Device:C_Small C4
U 1 1 60699D26
P 8950 4300
F 0 "C4" H 9042 4346 50  0000 L CNN
F 1 "220uF" H 9042 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 8950 4300 50  0001 C CNN
F 3 "~" H 8950 4300 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL32A227MQVNNNE/1276-3375-1-ND/3891461" H 8950 4300 50  0001 C CNN "Link"
	1    8950 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C7
U 1 1 6069AB23
P 9350 4300
F 0 "C7" H 9442 4346 50  0000 L CNN
F 1 "220uF" H 9442 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 9350 4300 50  0001 C CNN
F 3 "~" H 9350 4300 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL32A227MQVNNNE/1276-3375-1-ND/3891461" H 9350 4300 50  0001 C CNN "Link"
	1    9350 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C8
U 1 1 6069B97B
P 9750 4300
F 0 "C8" H 9842 4346 50  0000 L CNN
F 1 "220uF" H 9842 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 9750 4300 50  0001 C CNN
F 3 "~" H 9750 4300 50  0001 C CNN
F 4 "https://www.digikey.com.au/product-detail/en/samsung-electro-mechanics/CL32A227MQVNNNE/1276-3375-1-ND/3891461" H 9750 4300 50  0001 C CNN "Link"
	1    9750 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 4050 8950 4200
Connection ~ 8950 4050
Wire Wire Line
	8950 4050 9350 4050
Wire Wire Line
	9350 4200 9350 4050
Connection ~ 9350 4050
Wire Wire Line
	9350 4050 9750 4050
Wire Wire Line
	9750 4050 9750 4200
Connection ~ 9750 4050
Wire Wire Line
	9750 4050 10300 4050
$EndSCHEMATC