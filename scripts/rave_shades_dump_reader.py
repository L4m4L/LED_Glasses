import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

def normalise(signal):
    signal = np.array(signal, dtype=float)
    signal_min = np.min(signal)
    signal_max = np.max(signal)
    signal_centered = signal - signal_min
    return (2 * signal_centered) / (signal_max - signal_min) - 1

def cutoff_freq(filt, cutoff_loss, fs):
    w, h = signal.freqz(filt)
    freq = fs * w / (2 * np.pi)
    pow = 20 * np.log10(np.abs(h))
    dc = pow[0]
    i = 1
    while i < len(pow) and pow[i] > dc - cutoff_loss:
        i += 1
    return freq[i-1]

hamming_filter_lut = None

def filt_lut_init(taps, fs):
    global hamming_filter_lut
    assert(taps % 8 == 0)
    hamming_filter_lut = np.array([(1<<8)*[0] for _ in range(taps//8)], dtype=int)
    hamming_filter = signal.firwin2(taps, (0,200,1000,fs/2), (1,1,0,0), fs=fs)
    sum_max = 0
    for i in range(0, len(hamming_filter), 8):
        sum_tmp = np.sum(hamming_filter[i:i+8])
        if sum_tmp > sum_max:
            sum_max = sum_tmp
    scale = 256 / sum_max
    hamming_filter = np.array(scale * hamming_filter, dtype=int)
    for filt_idx in range(0, len(hamming_filter), 8):
        for value in range(256):
            filt_sum = 0
            byte = value
            for shift in range(8):
                if byte & 1:
                    filt_sum += hamming_filter[filt_idx + shift]
                byte = byte >> 1
            hamming_filter_lut[filt_idx//8, value] = filt_sum

def filt_lut_apply(data, dec):
    global hamming_filt_lut
    assert(dec % 8 == 0)
    output = []
    taps = hamming_filter_lut.shape[0]
    i = taps
    while i <= len(data):
        output += [0]
        print(i-taps, taps)
        for j in range(taps):
            output[-1] += hamming_filter_lut[j, data[i-taps+j]]
        i += dec//8
    print(len(data))
    return np.array(output, dtype=int)

mic_pdm_dump_filename = 'filt_lut_test_input.bin'
fs = (16e6 * 10 / 3) / 32

mic_pdm_raw = bytes()
with open(mic_pdm_dump_filename, 'rb') as input_file:
    mic_pdm_raw = input_file.read()
#mic_pdm_raw = mic_pdm_raw[10:]

mic_sample_dur = 8 * len(mic_pdm_raw) / fs

mic_pdm = np.zeros(8*len(mic_pdm_raw))
for base, byte in enumerate(mic_pdm_raw):
    for shift in range(8):
        if (byte >> shift) & 1 == 1:
            mic_pdm[8*base+shift] = 1

taps = 320#16*30#(1<<8)
dec = 160

mic_filt_box = np.array(taps*[1], dtype=int)
mic_filt_ham = signal.firwin2(taps, (0,200,1000,fs/2), (1,1,0,0), fs=fs)
#mic_filt_ham = np.array(((1<<16)/sum(mic_filt_ham))*mic_filt_ham, dtype=int)
# match mic_filt_res_lut amplitude
mic_filt_ham = np.array(32830.17332009763 * mic_filt_ham, dtype=int)
filt_lut_init(taps, fs)

print('box -20dB cuttoff', len(mic_filt_box), cutoff_freq(mic_filt_box, 20, fs))
print('ham -20dB cuttoff', len(mic_filt_ham), cutoff_freq(mic_filt_ham, 20, fs))

mic_filt_res_box = np.convolve(mic_pdm, mic_filt_box, 'valid')
mic_filt_res_ham = np.convolve(mic_pdm, mic_filt_ham, 'valid')

mic_filt_res_box = mic_filt_res_box[::dec]
mic_filt_res_ham = mic_filt_res_ham[::dec]
mic_filt_res_lut = filt_lut_apply(mic_pdm_raw, dec)

import struct
a = bytes()
with open('filt_lut_test_result.bin', 'rb') as input_file:
    a = input_file.read()
b = struct.unpack(f'<{len(a)//2}H', a)
mic_filt_res_lut_2 = np.array(b)

mic_filt_box_fft = np.fft.fft(mic_filt_res_box)
mic_filt_box_fft = mic_filt_box_fft[:len(mic_filt_box_fft)//2]
mic_filt_box_fft = 20*np.log10(np.abs(mic_filt_box_fft))
mic_filt_ham_fft = np.fft.fft(mic_filt_res_ham)
mic_filt_ham_fft = mic_filt_ham_fft[:len(mic_filt_ham_fft)//2]
mic_filt_ham_fft = 20*np.log10(np.abs(mic_filt_ham_fft))
mic_filt_lut_fft = np.fft.fft(mic_filt_res_lut)
mic_filt_lut_fft = mic_filt_lut_fft[:len(mic_filt_lut_fft)//2]
mic_filt_lut_fft = 20*np.log10(np.abs(mic_filt_lut_fft))

avg1 = np.average(mic_filt_box_fft[1:20])
avg2 = np.average(mic_filt_ham_fft[1:20])
avg3 = np.average(mic_filt_lut_fft[1:20])
mic_filt_ham_fft -= avg2 - avg1
mic_filt_lut_fft -= avg3 - avg1

#mic_filt_res_box = normalise(mic_filt_res_box)
#mic_filt_res_ham = normalise(mic_filt_res_ham)
#mic_filt_res_lut = normalise(mic_filt_res_lut)

time_box = np.linspace(0, mic_sample_dur, len(mic_filt_res_box))
time_ham = np.linspace(0, mic_sample_dur, len(mic_filt_res_ham))
time_lut = np.linspace(0, mic_sample_dur, len(mic_filt_res_lut))
time_lut2 = np.linspace(0, mic_sample_dur, len(mic_filt_res_lut_2))

freq_box = np.linspace(0, (fs/2)/dec, len(mic_filt_box_fft))
freq_ham = np.linspace(0, (fs/2)/dec, len(mic_filt_ham_fft))
freq_lut = np.linspace(0, (fs/2)/dec, len(mic_filt_lut_fft))

plt.figure();
plt.plot(time_box, mic_filt_res_box);
plt.plot(time_ham, mic_filt_res_ham);
plt.plot(time_lut, mic_filt_res_lut);
plt.plot(time_lut2, mic_filt_res_lut_2);
plt.pause(0.1)
plt.figure();
plt.plot(freq_box[1:], mic_filt_box_fft[1:]);
plt.plot(freq_ham[1:], mic_filt_ham_fft[1:]);
plt.plot(freq_lut[1:], mic_filt_lut_fft[1:]);
plt.pause(0.1)






##time = np.linspace(0, mic_sample_dur, len(mic_signal))
##time = time[:-length]
##mic_signal = mic_signal[:-length]
##mic_signal_fft = np.fft.fft(mic_signal)
##mic_signal_fft = mic_signal_fft[:len(mic_signal_fft)//2]
##mic_signal_fft = np.abs(mic_signal_fft)
##mic_signal_fft = 20*np.log10(mic_signal_fft)
##freq = np.linspace(0, 1.33333e6/2, len(mic_signal_fft))
##
##mic_signal_1 = mic_decimate_1(mic_pdm)
##time_1 = np.linspace(0, mic_sample_dur, len(mic_signal_1))
##fft_1 = np.fft.fft(mic_signal_1)
##fft_1 = fft_1[:len(fft_1)//2]
##fft_1 = np.abs(fft_1)
##fft_1 = 20*np.log10(fft_1)
###freq_1 = np.linspace(0, 1.33333e6/144/2, len(fft_1))
##freq_1 = np.linspace(0, 1.33333e6/2, len(fft_1))
##
##mic_signal = normalise(mic_signal)
##mic_signal_1 = normalise(mic_signal_1)
##
##avg1 = np.average(mic_signal_fft[1:20])
##avg2 = np.average(fft_1[1:20])
##fft_1 -= avg2 - avg1
##
##plt.figure();
##plt.plot(time, mic_signal);
##plt.plot(time_1, mic_signal_1);
##plt.pause(0.1)
##plt.figure();
##plt.plot(freq[1:], mic_signal_fft[1:]);
##plt.plot(freq_1[1:], fft_1[1:]);
##plt.pause(0.1)

