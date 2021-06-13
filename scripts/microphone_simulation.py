import numpy as np
import matplotlib.pyplot as plt

def scale(signal):
    signal_min = np.min(signal)
    return (signal - signal_min) / (np.max(signal) - signal_min)

def clamp(signal):
    signal[signal > 1] = 1
    signal[signal < 0] = 0
    return signal

def delta_sigma(signal):
    e = 0
    result = []
    for x in signal:
        e += x
        if e > 1:
            result.append(1)
            e -= 1
        else:
            result.append(0)
    return result

def decimate(pdm, bits, dec):
    n = 1<<bits
    i = n
    output = []
    while i <= len(pdm):
        output.append(np.sum(pdm[i-n:i]))
        i += dec
    return output

pdm_clock_freq = (16e6*10/3)/32
duration = 0.1
cycles = int(duration * pdm_clock_freq)
duration = cycles / pdm_clock_freq
time = np.linspace(0, duration, cycles, endpoint=False)

signal = np.sin(2*np.pi*1e3*time) + \
         np.sin(2*np.pi*2e3*time) + \
         np.sin(2*np.pi*3e3*time) + \
         np.sin(2*np.pi*4e3*time) + \
         np.sin(2*np.pi*5e3*time) + \
         np.sin(2*np.pi*6e3*time) + \
         np.sin(2*np.pi*7e3*time) + \
         np.sin(2*np.pi*8e3*time) + \
         np.sin(2*np.pi*9e3*time) + \
         np.sin(2*np.pi*10e3*time) + \
         np.sin(2*np.pi*11e3*time) + \
         np.sin(2*np.pi*12e3*time) + \
         np.sin(2*np.pi*13e3*time) + \
         np.sin(2*np.pi*14e3*time) + \
         np.sin(2*np.pi*15e3*time) + \
         np.sin(2*np.pi*16e3*time) + \
         np.sin(2*np.pi*17e3*time) + \
         np.sin(2*np.pi*18e3*time) + \
         np.sin(2*np.pi*19e3*time) + \
         np.sin(2*np.pi*20e3*time)
# signal = np.random.normal(0.5, 0.3, size=len(time))
signal = scale(signal)
# signal = clamp(signal)

pdm = delta_sigma(signal)

decimation_rate = 64
filt = decimate(pdm, 7, decimation_rate)

plt.figure()
plt.plot(time, pdm, '.')
plt.plot(time, signal)
time2 = np.linspace(0, duration, len(filt), endpoint=False)
plt.plot(time2, filt)

plt.figure()
T = 1/(pdm_clock_freq/decimation_rate)
N = int(duration * (pdm_clock_freq/decimation_rate))
yf = np.fft.fft(filt)
xf = np.linspace(0.0, 1.0/(2.0*T), int(N/2))
plt.plot(xf, 2.0/N * np.abs(yf[:N//2]), '.')

plt.show()
