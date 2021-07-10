import wave
import numpy as np
import struct
import pyaudio
import matplotlib.pyplot as plt
import threading
import time

SAMPLE_RATE = 0
SAMPLE_DECIMATION = 441
SAMPLE_SIZE = 1 << 11
##BEAT_FREQ_BINS = (0, 180, 800, 1600, 3200) # hertz
BEAT_FREQ_BINS = (0, 200, 400, 800, 1600, 3200) # hertz

wf = wave.open('test_audio.wav', 'rb')
SAMPLE_RATE = wf.getframerate()
signal_len = wf.getnframes()
signal = struct.unpack(f'{2*signal_len}h', wf.readframes(signal_len))
signal = np.array(signal[::2], dtype=float)
beats = [[] for _ in range(len(BEAT_FREQ_BINS)-1)]

beat_freq_bins_idx = [round(f*SAMPLE_SIZE/SAMPLE_RATE) for f in BEAT_FREQ_BINS]
beat_freq_bins_idx[0] += 1
beat_freq_bins_size = [beat_freq_bins_idx[i+1]-beat_freq_bins_idx[i] for i in range(len(BEAT_FREQ_BINS)-1)]

fft_data = []
signal_idx = SAMPLE_SIZE
while signal_idx < signal_len:
    freq = np.fft.fft(signal[signal_idx-SAMPLE_SIZE:signal_idx])
    freq = np.abs(freq[:len(freq)//2])
    fft_data.append(freq)
    for beat_idx in range(len(BEAT_FREQ_BINS)-1):
        freq_bin = freq[beat_freq_bins_idx[beat_idx]:beat_freq_bins_idx[beat_idx+1]]
        freq_bin_sum = np.sum(freq_bin) / beat_freq_bins_size[beat_idx]
        beats[beat_idx].append(freq_bin_sum)
    signal_idx += SAMPLE_DECIMATION

rises = []
for beat in beats:
    beat = np.array(beat, dtype=float)
    rise = np.maximum([beat[i]-beat[i-6] for i in range(6, len(beat))], 1)
    rise2 = rise
##    rise2 = []
##    for i in range(2, len(rise)-2):
##        if rise[i] > 0:
##            rise2.append(np.sum(rise[i-2:i+2]))
##        else:
##            rise2.append(1e-15)
    rises.append(rise2)
    
fft_data = np.transpose(fft_data)

##plt.figure();plt.plot(np.log10(beats[0])); plt.plot(np.log10(rises[0])); plt.pause(0.1)
##plt.figure();plt.plot(np.log10(beats[1])); plt.plot(np.log10(rises[1])); plt.pause(0.1)
##plt.figure();plt.imshow(fft_data); plt.pause(0.1)

##t = np.linspace(0, signal_len / SAMPLE_RATE, len(beats[0]))
##for i, beat in enumerate(beats):
##    plt.figure()
##    plt.plot(t, beat, label=str(BEAT_FREQ_BINS[i+1]))
##    ax = plt.gca()
##    ax.set_ylim(0, 1e7)
##    plt.legend()
##    plt.pause(0.1)

##
##raw = wf.readframes(SAMPLE_SIZE//2)
##data = []
####while len(raw) // 2 == chunk:
##for _ in range(800):
##    s = struct.unpack(f'{SAMPLE_SIZE}h', raw)
##    f = np.fft.fft(s)
##    d = np.abs(f[:len(f)//8])
##    data.append(d)
##    raw = wf.readframes(SAMPLE_SIZE//2)
##data = np.array(data)
##data = data.transpose()
##plt.imshow(data)
##plt.pause(0.05)



##SAMPLE_SIZE = 1 << 12
##SAMPLE_RATE = int(((16e6*10/3)/32)/128)
##
##BEAT_FREQ_BINS = (0, 200, 400, 800, 1600, 3200) # hertz
##BEAT_TEMPO_RANGE = (1, 5) # hertz
##
##class AudioInThread(threading.Thread):
##    def __init__(self, ):
##        
##(stream, buffer, run):
##    print('Thread started: audio_helper()')
##    run.wait()
##    while run.is_set():
##        print(run.is_set())
##        buffer += stream.read(SAMPLE_SIZE)
##        time.sleep(1)
##    print('Thread stopped: audio_helper()')
##
##beat_freq_bins_idx = [int(f*SAMPLE_SIZE/SAMPLE_RATE) for f in BEAT_FREQ_BINS]
##
##audio_object = pyaudio.PyAudio()
##audio_stream = audio_object.open(format=pyaudio.paInt16, channels=1, rate=SAMPLE_RATE, input=True, frames_per_buffer=SAMPLE_SIZE)
##audio_buffer = bytes()
##audio_thread_run = threading.Event()
##audio_thread_run.set()
##audio_helper = threading.Thread(target=audio_helper, args=(audio_stream, audio_buffer, audio_thread_run))
##audio_helper.start()
##
##
##try:
##    #while True:
##    pass
##finally:
##    audio_thread_run.clear()


##print(sample_rate,sample_size)
##in_stream = p.open(format=pyaudio.paInt16, channels=1, rate=sample_rate, input=True, frames_per_buffer=sample_size)
####in_stream = p.open(format=pyaudio.paInt16, channels=1, rate=60000, input=True, frames_per_buffer=chunk)
##plt.figure()
##while True:
##    b = in_stream.read(sample_size)
##    d = struct.unpack(f'{sample_size}h', b)
##    d = np.array(d, dtype=float)
##    f = np.fft.fft(d)
##    f2 = np.abs(f[:len(f)//2])
##    
##
##
##    
##    
##    f2 = np.abs(f[:len(f)//2])
##    i = np.argmax(f2)
##    print((sample_rate/2)/(sample_size/2)*i)
####    f = np.correlate(d, d, mode='full')
####    x = np.linspace(0,(sample_rate/2)/(sample_size/2)*sample_size//2, sample_size//2, endpoint=False)
##    plt.cla()
##    ax = plt.gca()
##    ax.set_ylim(0, 1e6)
##    plt.plot(x, f2, '.')
##    plt.pause(0.05)
##    
####    f = np.fft.fft(d)
##    
##in_stream.close()
##p.terminate()

##chunk = 1 << 10
##wf = wave.open('test_audio.wav', 'rb')
##raw = wf.readframes(chunk//2)
##data = []
####while len(raw) // 2 == chunk:
##for _ in range(800):
##    s = struct.unpack(f'{chunk}h', raw)
##    f = np.fft.fft(s)
##    d = np.abs(f[:len(f)//8])
##    data.append(d)
##    raw = wf.readframes(chunk//2)
##data = np.array(data)
##plt.imshow(data)
##plt.pause(0.05)


##### create an audio object
##p = pyaudio.PyAudio()
##
### open stream based on the wave object which has been input.
##stream = p.open(format =
##                p.get_format_from_width(wf.getsampwidth()),
##                channels = wf.getnchannels(),
##                rate = wf.getframerate(),
##                output = True)
##
##print(wf.getsampwidth(), wf.getnchannels(), wf.getframerate())
##
### read data (based on the chunk size)
##data = wf.readframes(chunk)
##
### play stream (looping from beginning of file to the end)
##while data != '':
##    # writing to the stream is what *actually* plays the sound.
##    stream.write(data)
##    data = wf.readframes(chunk)
##
### cleanup stuff.
##stream.close()    
##p.terminate()
##
##




##import numpy as np
##import pyaudio
##import struct
##import matplotlib.pyplot as plt
##import time
##import wave
##
##CHUNK = 1024
##FORMAT = pyaudio.paInt16
##CHANNELS = 1
####RATE = 26041
##RATE = 44100
##RECORD_SECONDS = 5
##
##p = pyaudio.PyAudio()
##
##
##
####in_stream = p.open(format=FORMAT,
####                   channels=CHANNELS,
####                   rate=RATE,
####                   input=True,
####                   frames_per_buffer=CHUNK)
##
##out_stream = p.open(format=FORMAT,
##                    channels=CHANNELS,
##                    rate=RATE,
##                    output=True,
##                    frames_per_buffer=CHUNK)
##
##print("recording")
##
##data = bytes()
##for i in range(0, int(RATE / (CHUNK) * RECORD_SECONDS)):
##    data = in_stream.read(CHUNK)
##    out_stream.write(data)
##    
##
##print("done recording")
##
##in_stream.stop_stream()
##out_stream.stop_stream()
##in_stream.close()
##out_stream.close()
##p.terminate()
##
##
