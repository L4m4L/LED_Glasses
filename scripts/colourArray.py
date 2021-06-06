# Python script to create a LUT for LED glasses

import binascii
import pprint
import sys
pp = pprint.PrettyPrinter(indent=4)

# Result bytes 
ZERO_ZERO_BYTE 	= 136	# 0x00
ZERO_ONE_BYTE 	= 140	# 0x01
ONE_ZERO_BYTE	= 200	# 0x10
ONE_ONE_BYTE	= 204	# 0x11

# masks
TWO_MSB = 0b11000000
MID_MSB = 0b00110000
MID_LSB = 0b00001100
TWO_LSB = 0b00000011

results = []

for i in range(0,256):
	msb_val = i & TWO_MSB
	mid_msb_val = i & MID_MSB
	mid_lsb_val = i & MID_LSB
	lsb_val = i & TWO_LSB

	# set MSB
	if (msb_val > 128):
		results.append(hex(ONE_ONE_BYTE))
	elif (msb_val > 64):
		results.append(hex(ONE_ZERO_BYTE))
	elif (msb_val > 0):
		results.append(hex(ZERO_ONE_BYTE))
	else:
		results.append(hex(ZERO_ZERO_BYTE))

	# set MID_MSB
	if (mid_msb_val > 32):
		results.append(hex(ONE_ONE_BYTE))
	elif (mid_msb_val > 16):
		results.append(hex(ONE_ZERO_BYTE))
	elif (mid_msb_val > 0):
		results.append(hex(ZERO_ONE_BYTE))
	else:
		results.append(hex(ZERO_ZERO_BYTE))

	# set MID_MSB
	if (mid_lsb_val > 8):
		results.append(hex(ONE_ONE_BYTE))
	elif (mid_lsb_val > 4):
		results.append(hex(ONE_ZERO_BYTE))
	elif (mid_lsb_val > 0):
		results.append(hex(ZERO_ONE_BYTE))
	else:
		results.append(hex(ZERO_ZERO_BYTE))

	# set MID_MSB
	if (lsb_val > 2):
		results.append(hex(ONE_ONE_BYTE))
	elif (lsb_val > 1):
		results.append(hex(ONE_ZERO_BYTE))
	elif (lsb_val > 0):
		results.append(hex(ZERO_ONE_BYTE))
	else:
		results.append(hex(ZERO_ZERO_BYTE))

	#print(mid_lsb_val)

sys.stdout = open ("colourMap.txt", "w")

for index, byte in enumerate(results):
	print(byte, end=', ')
	index+=1
	if (index % 4 == 0):
		print('')

sys.stdout.close()