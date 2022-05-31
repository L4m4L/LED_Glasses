from scipy import signal
import numpy as np
import matplotlib.pyplot as plt

def create_pdm_decimation_filter_lut(taps=480, dc_offset=0.5):
    assert(taps % 8 == 0)

    sample_frequency = (16e6 * 10 / 3) / 32
    coefficients = signal.firwin2(taps,
                                  (0, 200, 1000, sample_frequency / 2),
                                  (1, 1, 0, 0),
                                  window=('kaiser', 8.6),
                                  fs=sample_frequency)

    # Scale the filter coefficients so that they fit nicely into an 8 bit
    # value in the lookup table.
    # TODO: the other constraint is to make sure the biggest return from the
    #       LUT filter can fit into 16 bits.
    gain_max = 0
    for i in range(0, len(coefficients), 8):
        gain = np.sum(coefficients[i:i + 8])
        if gain > gain_max:
            gain_max = gain
    print(gain_max)
    scale = 256 / gain_max
    #scale = 2**16 / gain_max
    print(scale)
    coefficients_scaled = scale * coefficients
    coefficients_integer = np.array(coefficients_scaled, dtype=int)

    lookup_table = []
    for i in range(0, len(coefficients_integer), 8):
        lookup_table_inner = []
        for pdm in range(256):
            filter_sum = 0
            for shift in range(8):
                filter_sum += coefficients_integer[i + shift] * ((pdm & 1) - dc_offset)
                pdm = pdm >> 1
            lookup_table_inner.append(filter_sum)
        lookup_table.append(lookup_table_inner)
    lookup_table = np.array(lookup_table, dtype=int)

    return lookup_table, coefficients_integer, coefficients_scaled

def decimate_pdm(signal, filter_lut, taps=480, dec=120):
    assert(taps % 8 == 0)
    assert(dec  % 8 == 0)
    assert(filter_lut.shape[0] == taps // 8)
    assert(filter_lut.shape[1] == 256)

    stages = taps // dec
    taps8  = taps // 8
    dec8   = dec // 8

    results = stages * [0]
    steps   = stages * [0]
    for i in range(stages):
        steps[i] = (taps8 - i * dec8) % taps8

    warm = False
    signal_filtered = []
    for pdm in signal:
        for i in range(stages):
            results[i] += filter_lut[steps[i]][pdm]
            steps[i] += 1
            if steps[i] == taps8:
                steps[i] = 0
                if warm:
                    signal_filtered.append(results[i])
            results[i]
        warm = True
    return np.array(signal_filtered)

a = None

if __name__=="__main__":
    filter_lut, filter_coef_int, filter_coef = create_pdm_decimation_filter_lut()
    a = filter_lut

    fs = (16e6 * 10 / 3) / 32
    w, h = signal.freqz(filter_coef, worN=4**10)
    frequency = 0.5 * fs * w / np.pi
    amplitude = 20 * np.log10(np.abs(h))

    w, h = signal.freqz(filter_coef_int, worN=4**10)
    frequency_int = 0.5 * fs * w / np.pi
    amplitude_int = 20 * np.log10(np.abs(h))

    dc = amplitude_int[0]
    i = 0
    while i < len(amplitude_int) and amplitude_int[i] > dc - 3:
        i += 1
    print(f'fir -3dB frequency -> {frequency_int[i]}Hz')
    while i < len(amplitude_int) and amplitude_int[i] > dc - 10:
        i += 1
    print(f'fir -10dB frequency -> {frequency_int[i]}Hz')
    while i < len(amplitude_int) and amplitude_int[i] > dc - 20:
        i += 1

    plt.figure()
    plt.plot(filter_coef)
    plt.plot(filter_coef_int)
    plt.pause(0.1)
    plt.figure()
    plt.plot(frequency, amplitude)
    plt.plot(frequency_int, amplitude_int)
    plt.pause(0.1)

    decimated_frequency = 2 * frequency_int[i]
    #decimation_rate = int(fs / decimated_frequency)
    decimation_rate = 120#8 * (decimation_rate // 8)
    decimated_frequency_actual = fs / decimation_rate
    print(f'fir -{20}dB frequency -> {frequency[i]}Hz')
    print(f'decimation frequency target -> {decimated_frequency}Hz')
    print(f'decimation rate -> {decimation_rate}')
    print(f'actual decimation frequency -> {decimated_frequency_actual}Hz')

    tab = 4*' '
    output = 'const int8_t filter_lookup_table[FILTER_TAPS8][256] = {\n'
    output += tab + '{\n'
    for i in range(len(filter_lut)):
        line = []
        for j in range(len(filter_lut[i])):
            line += ['{:6d}'.format(filter_lut[i][j])]
            if len(line) == 16:
                output += 2*tab + ', '.join(line)
                if j < len(filter_lut[i]) - 1:
                    output += ','
                output += '\n'
                line = []
        output += tab + '}'
        if i < len(filter_lut) - 1:
            output += ', {'
        output += '\n'
    output += '};\n'
    output += '\n'

    with open('filter.c', 'w') as output_file:
        output_file.write(output)
