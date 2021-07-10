from scipy import signal
import matplotlib.pyplot as plt
import numpy as np
import math

def fir_gen(taps, freq):
    assert(taps % 2 == 1)
    fir = np.empty(taps, dtype=float)
    middle = taps // 2
    fir[middle] = 1
    for i in range(1, middle+1):
        x = freq * i
        sinc = math.sin(x) / x
        fir[middle + i] = sinc
        fir[middle - i] = sinc
    return fir

fs = (16e6*10/3)/32
bands = (0, 200, 1000, fs/2)
##bands = (0, fs/4, fs/3.9, fs/2)
desired = (1, 1, 0, 0)
taps = 480

fir_firwin2 = signal.firwin2(640, bands, desired, window=('kaiser', 8.6), fs=fs)
#fir_avg     = np.array(320*[1])
#fir_avg     = np.array(taps*[1/taps])
fir_avg     = signal.firwin2(taps, bands, desired, fs=fs)
#fir_gen     = fir_gen(taps, 0.01)


fir2_firwin2 = np.array(((1<<16) / sum(fir_firwin2)) * fir_firwin2, dtype=int)
fir2_avg     = np.array(((1<<16) / sum(fir_avg))     * fir_avg,     dtype=int)
#fir2_gen     = np.array(((1<<16) / sum(fir_gen))     * fir_gen,     dtype=int)

plt.figure()
hs = list()
ax = plt.gca()
for fir in (fir2_firwin2, fir2_avg):#, fir2_gen):
    w, h = signal.freqz(fir, worN=4**9)
    hs.append(ax.plot(0.5*fs*w/np.pi, 20*np.log10(np.abs(h)))[0])
ax.legend(hs, ('firwin2', 'avg', 'gen'),
          loc='lower center', frameon=False)
ax.set_xlabel('Frequency (Hz)')
ax.set_ylabel('Magnitude (dB)')
ax.grid(True)
plt.pause(0.1)

plt.figure()
hs = list()
ax = plt.gca()
for fir in (fir2_firwin2, fir2_avg):#, fir2_gen):
    hs.append(ax.plot(fir)[0])
ax.legend(hs, ('firwin2', 'avg', 'gen'),
          loc='lower center', frameon=False)
ax.set_xlabel('Tap')
ax.set_ylabel('Magnitude')
ax.grid(True)
plt.pause(0.1)

print(max(fir2_firwin2))

##from scipy.optimize import newton
##from scipy.signal import freqz, dimpulse, dstep
##from math import sin, cos, sqrt, pi
##import numpy as np
##import matplotlib.pyplot as plt
##
### Function for calculating the cut-off frequency of a moving average filter
##def get_sma_cutoff(N, **kwargs):
##    func = lambda w: sin(N*w/2) - N/sqrt(2) * sin(w/2)  # |H(e^jω)| = √2/2
##    deriv = lambda w: cos(N*w/2) * N/2 - N/sqrt(2) * cos(w/2) / 2  # dfunc/dx
##    omega_0 = pi/N  # Starting condition: halfway the first period of sin(Nω/2)
##    return newton(func, omega_0, deriv, **kwargs)
##
### Simple moving average design parameters
##f_s = (16e6*10/3)/32
##N = 1<<8
##
### Find the cut-off frequency of the SMA
##w_c = get_sma_cutoff(N)
##f_c = w_c * f_s / (2 * pi)
##
### Calculate the pole location of the EMA with the same cut-off frequency
##b = 2 - 2*cos(w_c)
##alpha = (-b + sqrt(b**2 + 4*b)) / 2
##
### SMA & EMA coefficients
##b_sma = np.ones(N)
##a_sma = np.array([N] + [0]*(N-1))
##
##b_ema = np.array((alpha, 0))
##a_ema = np.array((1, alpha - 1))
##
### Calculate the frequency response
##w, h_sma = freqz(b_sma, a_sma, worN=4096)
##w, h_ema = freqz(b_ema, a_ema, w)
##w *= f_s / (2 * pi)                      # Convert from rad/sample to Hz
##
### Plot the amplitude response
##plt.subplot(2, 1, 1)
##plt.suptitle('Bode Plot')
##plt.plot(w, 20 * np.log10(abs(h_sma)),   # Convert modulus to dB
##         color='blue', label='SMA')
##plt.plot(w, 20 * np.log10(abs(h_ema)),
##         color='green', label='EMA')
##plt.ylabel('Magnitude [dB]')
##plt.xlim(0, f_s / 2)
##plt.ylim(-60, 10)
##plt.axvline(f_c, color='red')
##plt.axhline(-3.01, linewidth=0.8, color='black', linestyle=':')
##plt.legend()
##
### Plot the phase response
##plt.subplot(2, 1, 2)
##plt.plot(w, 180 * np.angle(h_sma) / pi,  # Convert argument to degrees
##         color='blue')
##plt.plot(w, 180 * np.angle(h_ema) / pi,
##         color='green')
##plt.xlabel('Frequency [Hz]')
##plt.ylabel('Phase [°]')
##plt.xlim(0, f_s / 2)
##plt.ylim(-180, 90)
##plt.yticks([-180, -135, -90, -45, 0, 45, 90])
##plt.axvline(f_c, color='red')
##plt.show()
##
### Plot the impulse response
##t, y_sma = dimpulse((b_sma, a_sma, 1/f_s), n=2*N)
##t, y_ema = dimpulse((b_ema, a_ema, 1/f_s), n=2*N)
##plt.suptitle('Impulse Response')
##plt.plot(t, y_sma[0], 'o-',
##         color='blue', label='SMA')
##plt.plot(t, y_ema[0], 'o-',
##         color='green', label='EMA')
##plt.xlabel('Time [seconds]')
##plt.ylabel('Output')
##plt.xlim(-1/f_s, 2*N/f_s)
##plt.legend()
##plt.show()
##
### Plot the step response
##t, y_sma = dstep((b_sma, a_sma, 1/f_s), n=2*N)
##t, y_ema = dstep((b_ema, a_ema, 1/f_s), n=2*N)
##plt.suptitle('Step Response')
##plt.plot(t, y_sma[0], 'o-',
##         color='blue', label='SMA')
##plt.plot(t, y_ema[0], 'o-',
##         color='green', label='EMA')
##plt.xlabel('Time [seconds]')
##plt.ylabel('Output')
##plt.xlim(-1/f_s, 2*N/f_s)
##plt.legend()
##plt.show()

##
##import numpy as np
##import matplotlib.pyplot as plt
##
##def scale(signal):
##    signal_min = np.min(signal)
##    return (signal - signal_min) / (np.max(signal) - signal_min)
##
##def clamp(signal):
##    signal[signal > 1] = 1
##    signal[signal < 0] = 0
##    return signal
##
##def delta_sigma(signal):
##    e = 0
##    result = []
##    for x in signal:
##        e += x
##        if e > 1:
##            result.append(1)
##            e -= 1
##        else:
##            result.append(0)
##    return result
##
##def decimate(pdm, bits, dec):
##    n = 1<<bits
##    i = n
##    output = []
##    while i <= len(pdm):
##        output.append(np.sum(pdm[i-n:i]))
##        i += dec
##    return output
##
##pdm_clock_freq = (16e6*10/3)/32
##duration = 0.1
##cycles = int(duration * pdm_clock_freq)
##duration = cycles / pdm_clock_freq
##time = np.linspace(0, duration, cycles, endpoint=False)
##
##signal = np.sin(2*np.pi*100*time) + \
##         np.sin(2*np.pi*200*time) + \
##         np.sin(2*np.pi*3e3*time) + \
##         np.sin(2*np.pi*4e3*time) + \
##         np.sin(2*np.pi*5e3*time) + \
##         np.sin(2*np.pi*6e3*time) + \
##         np.sin(2*np.pi*7e3*time) + \
##         np.sin(2*np.pi*8e3*time) + \
##         np.sin(2*np.pi*9e3*time) + \
##         np.sin(2*np.pi*10e3*time) + \
##         np.sin(2*np.pi*11e3*time) + \
##         np.sin(2*np.pi*12e3*time) + \
##         np.sin(2*np.pi*13e3*time) + \
##         np.sin(2*np.pi*14e3*time) + \
##         np.sin(2*np.pi*15e3*time) + \
##         np.sin(2*np.pi*16e3*time) + \
##         np.sin(2*np.pi*17e3*time) + \
##         np.sin(2*np.pi*18e3*time) + \
##         np.sin(2*np.pi*19e3*time) + \
##         np.sin(2*np.pi*20e3*time)
####signal = np.sin(2*np.pi*5e3*time)
### signal = np.random.normal(0.5, 0.3, size=len(time))
##signal = scale(signal)
### signal = clamp(signal)
##
##pdm = delta_sigma(signal)
##
##decimation_rate = 160
##filt = decimate(pdm, 8, decimation_rate)
####filt = scale(filt)
##
##plt.figure()
##plt.plot(time, pdm, '.')
##plt.plot(time, signal)
##time2 = np.linspace(0, duration, len(filt), endpoint=False)
##plt.plot(time2, filt)
##
##plt.figure()
##T = 1/(pdm_clock_freq/decimation_rate)
##N = int(duration * (pdm_clock_freq/decimation_rate))
##yf = np.abs(np.fft.fft(filt))
##xf = np.linspace(0.0, 1.0/(2.0*T), N//2)
##plt.plot(xf[1:], 2.0/N * np.abs(yf[1:N//2]), '.')
##
##plt.pause(0.1)
