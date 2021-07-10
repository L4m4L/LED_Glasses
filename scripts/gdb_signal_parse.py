import re
import numpy as np

filename = 'gdb_dump_signal_1.txt'

data = []
with open(filename) as file:
    for line in file:
        tokens = re.split('\s', line.strip())[2:]
        tokens = [t[2:] for t in tokens]
        for t in tokens:
            data.append(int(t[:2], 16))
            data.append(int(t[2:4], 16))
            data.append(int(t[4:6], 16))
            data.append(int(t[6:], 16))

