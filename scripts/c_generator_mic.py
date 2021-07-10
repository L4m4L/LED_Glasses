from scipy import signal
import numpy as np

sample_frequency = (16e6 * 10 / 3) / 32
frequency_bands = (0, 200, 1000, sample_frequency/2)
band_amplitude = (1, 1, 0, 0)
taps = 480
fir_output_max = 1<<16
cutoff = 20

fir = signal.firwin2(taps, frequency_bands, band_amplitude, fs=sample_frequency)
fir_scaled = np.array((fir_output_max / sum(fir)) * fir, dtype=int)

print(f'fir taps -> {taps}')
print(f'make sure fir_scaled can fit into uint8_t[] ({max(fir_scaled)} < 256)')
print(f'make sure sum(fir_scaled) can fit into uint16_t ({sum(fir_scaled)} < {1<<16})')
assert(max(fir_scaled) < 256)
assert(sum(fir_scaled) < 1<<16)

w, h = signal.freqz(fir_scaled, worN=4096)
frequency = 0.5 * sample_frequency * w / np.pi
amplitude = 20 * np.log10(np.abs(h))
dc = amplitude[0]
i = 0
while i < len(amplitude) and amplitude[i] > dc - cutoff:
    i += 1

decimated_frequency = 2 * frequency[i]
decimation_rate = int(sample_frequency / decimated_frequency)
decimation_rate = 16 * (decimation_rate // 16)
decimated_frequency_actual = sample_frequency / decimation_rate

print(f'fir -{cutoff}dB frequency -> {frequency[i]}Hz')
print(f'decimation frequency target -> {decimated_frequency}Hz')
print(f'decimation rate -> {decimation_rate}')
print(f'actual decimation frequency -> {decimated_frequency_actual}Hz')

for fft_size in (128, 256, 512, 1024):
    bin_size = decimated_frequency_actual / fft_size
    i = 0
    while i < fft_size and (i + 0.5) * bin_size < 200:
        i += 1
    i -= 1 # remove dc bin
    print(f'fft size of {fft_size} gives {i} usable frequency bins below 200Hz, bin size is {bin_size}Hz')

tab = 4*' '
output = ''

# generate fir filter array
output += f'static const uint8_t mic_dec_filt[{len(fir_scaled)}]' + ' = {\n'
line = []
for i in range(len(fir_scaled)):
    line += ['{:3d}'.format(fir_scaled[i])]
    if len(line) == 8:
        output += tab + ', '.join(line)
        if i < len(fir_scaled) - 1:
            output += ','
        output += '\n'
        line = []
if len(line) > 0:
    output += tab + ', '.join(line)
    output += '\n'
output += '};\n'
output += '\n'

# generate boxcar lookup table for uint8 values
output += 'static const uint8_t mic_filt_box[256] = {\n'
line = []
for i in range(256):
    total = 0
    j = i
    while j:
        j &= j - 1
        total += 1
    line += ['{:1d}'.format(total)]
    if len(line) == 16:
        output += tab + ', '.join(line)
        if i < len(fir_scaled) - 1:
            output += ','
        output += '\n'
        line = []
if len(line) > 0:
    output += tab + ', '.join(line)
    output += '\n'
output += '};\n'
output += '\n'

# generate hamming window fir filter lookup table
# -10dB is roughly at 4.8kHz, works well with a decimation rate of 160 (~11kHz)
dec_filt_taps = 320
fs = (16e6 * 10 / 3) / 32
dec_filt_ham = signal.firwin2(dec_filt_taps, (0,200,1000,fs/2), (1,1,0,0), fs=fs)
# the decimation filter will start with a unity gain (i.e. if the input amplitude is 1 the
# output will be 1). This means that each element is much smaller than 1 (~1/taps). We want the 
# result to fill a uint8 as much as possible without overflow. and uint8 saves heaps of 
# space over uint16.
gain_max = 0
for i in range(0, len(dec_filt_ham), 8):
    gain = np.sum(dec_filt_ham[i:i+8])
    if gain > gain_max:
        gain_max = gain
scale = 256 / gain_max
dec_filt_ham = np.array(scale * dec_filt_ham, dtype=int)
# calculate the actual lookup table
dec_filt_ham_lut = []
for i in range(0, len(dec_filt_ham), 8):
    dec_filt_ham_lut_tmp = []
    for pdm in range(256):
        dec_filt_out = 0
        for shift in range(8):
            if pdm & 1:
                dec_filt_out += dec_filt_ham[i + shift]
            pdm = pdm >> 1
        dec_filt_ham_lut_tmp.append(dec_filt_out)
    dec_filt_ham_lut.append(dec_filt_ham_lut_tmp)

output += 'static const uint8_t mic_dec_filt['
output += str(len(dec_filt_ham_lut))
output += ']['
output += str(len(dec_filt_ham_lut[0]))
output += '] = {\n'
output += tab + '{\n'
for i in range(len(dec_filt_ham_lut)):
    line = []
    for j in range(len(dec_filt_ham_lut[i])):
        line += ['{:3d}'.format(dec_filt_ham_lut[i][j])]
        if len(line) == 16:
            output += 2*tab + ', '.join(line)
            if j < len(dec_filt_ham_lut[i]) - 1:
                output += ','
            output += '\n'
            line = []
    output += tab + '}'
    if i < len(dec_filt_ham_lut) - 1:
        output += ', {'
    output += '\n'
output += '};\n'
output += '\n'

with open('mic.c', 'w') as output_file:
    output_file.write(output)
