tab = 4*' '
data_byte_width = 2
assert((data_byte_width == 1) or (data_byte_width == 2))

def generate_led_c_file():
    with open('led.c', 'w') as output_file:
        output =  ''
        
        # generate bit definitions
        bit_defs = []
        for i in range(1 << (2 * data_byte_width)):
            output += '#define '
            
            bit_def = 'LED_'
            binary = ''
            if data_byte_width == 1:
                binary = '{:02b}'.format(i)
            else:
                binary = '{:04b}'.format(i)
            bit_def += binary
            bit_defs.append(bit_def)
            
            output += bit_def
            output += ' (0x'
            for c in binary:
                if c == '0':
                    output += '8'
                else:
                    output += 'C'
            output += 'U)\n'
        output += '\n'
        
        # generate colour map
        output += 'static const '
        if data_byte_width == 1:
            output += 'uint8_t'
        else:
            output += 'uint16_t'
        output += ' led_colour_map[] = {\n'
        for i in range(256//4):
            output += tab
            for j in range(4):
                k = 4 * i + j
                if data_byte_width == 1:
                    output += bit_defs[(k >> 6) & 0b11] + ', '
                    output += bit_defs[(k >> 4) & 0b11] + ', '
                    output += bit_defs[(k >> 2) & 0b11] + ', '
                    output += bit_defs[(k >> 0) & 0b11]
                else:
                    output += bit_defs[(k >> 4) & 0b1111] + ', '
                    output += bit_defs[(k >> 0) & 0b1111]
                if k < 255:
                     output += ','
                if j < 3:
                    output += '   '
            output += '\n'
        output += '};\n'
        output += '\n'

        # generate led addresses
        output += 'static const uint32_t led_addresses[LED_ROWS][LED_COLS] = {\n'
        addresses = ((37,36,35,34,33,32,31,30,29,28),
                     (38,39,40,41,42,23,24,25,26,27),
                     (47,46,45,44,43,22,21,20,19,18),
                     (48,49,50,51,52,13,14,15,16,17),
                     (57,56,55,54,53,12,11,10,9,8),
                     (58,59,60,61,62,3,4,5,6,7),
                     (999,65,64,63,999,999,2,1,0,999))
        for addresses_line in addresses:
            half = len(addresses_line) // 2
            output += tab + '{'
            output += ', '.join(['{:4d}'.format(i) for i in addresses_line[:half]])
            output += ',      '
            output += ', '.join(['{:4d}'.format(i) for i in addresses_line[half:]])
            output += '},\n'
        output += '};\n'
        
        output_file.write(output)

generate_led_c_file()
