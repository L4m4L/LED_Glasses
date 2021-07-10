import cv2
import numpy as np

animations = [{'filename':'animation_heart.png', 'reversed':True, 'name':'display_animation_heart', 'monochrome':True, 'colour':(4,0,0), 'full':False}]
output_filename = 'display.c'
display_width = 10
display_height = 7
display_led_count = 66
display_led_coords = (
    (8, 6), (7, 6), (6, 6), (5, 5), (6, 5), (7, 5), (8, 5), (9, 5),
    (9, 4), (8, 4), (7, 4), (6, 4), (5, 4), (5, 3), (6, 3), (7, 3),
    (8, 3), (9, 3), (9, 2), (8, 2), (7, 2), (6, 2), (5, 2), (5, 1),
    (6, 1), (7, 1), (8, 1), (9, 1), (9, 0), (8, 0), (7, 0), (6, 0),
    (5, 0), (4, 0), (3, 0), (2, 0), (1, 0), (0, 0), (0, 1), (1, 1),
    (2, 1), (3, 1), (4, 1), (4, 2), (3, 2), (2, 2), (1, 2), (0, 2),
    (0, 3), (1, 3), (2, 3), (3, 3), (4, 3), (4, 4), (3, 4), (2, 4),
    (1, 4), (0, 4), (0, 5), (1, 5), (2, 5), (3, 5), (4, 5), (3, 6),
    (2, 6), (1, 6)
)

#for animation in animations:
anim = animations[0]
anim_img = cv2.imread(anim['filename'], cv2.IMREAD_GRAYSCALE)
anim_frames = []
y_start = 1
y_stop = 8
if anim['full']:
    # full display animation
    anim_len = (anim_img.shape[1] - 1) // (display_width + 1)
else:
    # per eye animation
    anim_len = (anim_img.shape[1] - 1) // (display_width // 2 + 1)
    for frame_idx in range(anim_len):
        x_start = (display_width // 2 + 1) * frame_idx + 1
        x_stop  = (display_width // 2 + 1) * (frame_idx + 1)
        anim_frame = anim_img[y_start:y_stop, x_start:x_stop]
        # duplicate to the right to represent both eyes
        anim_frame = np.tile(anim_frame, (1, 2))
        anim_frames.append(anim_frame)

if anim['reversed']:
    anim_frames = anim_frames[::-1]

display_frames = []
for anim_frame in anim_frames:
    display_frames.append([])
    for led_coord in display_led_coords:
        colour = (0, 0, 0)
        if anim_frame[led_coord[1], led_coord[0]] > 0:
            if anim['monochrome']:
                colour = anim['colour']
        display_frames[-1].append(colour)

tab = 4 * ' '
name = anim['name']
name_def = f'{name.upper()}_LENGTH'
output = f'#define {name_def} ({len(display_frames)})\n'
output += f'static const colour_t {name}[{name_def}][LED_COUNT] = '+'{'
for frame_idx, frame in enumerate(display_frames):
    output += '{ // frame ' + str(frame_idx+1) + '\n'
    line = []
    for led_idx, colour in enumerate(frame):
        line += ['{' + '{:3d}, {:3d}, {:3d}'.format(*colour) + '}']
        if len(line) == 8:
            output += 2*tab + ', '.join(line)
            if led_idx < len(frame) - 1:
                output += ','
            output += '\n'
            line = []
    if len(line) > 0:
        output += 2*tab + ', '.join(line) + '\n'
    if frame_idx < len(display_frames) - 1:
        output += tab + '}, '
    else:
        output += '}'
output += '};\n'
with open(output_filename, 'w') as output_file:
    output_file.write(output)
