filename = 'rave_shades_display_buffer_dump_3.bin'
display_raw = bytes()
with open(filename, 'rb') as file:
    display_raw = file.read()

for led_idx in range(66):
    colour = [0, 0, 0]
    for i in range(12):
        j = i % 4
        k = i // 4
        lower = display_raw[12*led_idx+i] & 0x0F
        upper = display_raw[12*led_idx+i] & 0xF0
        if lower == 0x0C:
            colour[k] |= 1 << (2*j+0)
        if upper == 0xC0:
            colour[k] |= 1 << (2*j+1)
    tmp = colour[0]
    colour[0] = colour[1]
    colour[1] = tmp
    print(led_idx, colour)
