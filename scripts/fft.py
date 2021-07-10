import numpy as np

testbla = set()


def Pretty(x):
    x = np.around(x, 1)
    print('[' + ', '.join([str(i) for i in x]) + ']')

def BitReverse(a, n):
    b = 0
    for i in range(n):
        b |= ((a>>i)&1)<<(n-i-1)
    return b

def Swap(x, t=np.complex128):
    n = len(x)
    log2n = int(np.log2(n))
    A = np.zeros(n, dtype=t)
    for i in range(n):
        A[BitReverse(i, log2n)] = x[i]
    return A

def Weight(k, m):
    return np.exp(-2j * np.pi * k / m)

def FourierTransform(x):
    test = set()
    A = np.array(x, dtype=np.complex128)
    n = len(x)
    log2n = int(np.log2(n))
    for s in range(1, log2n+1):
        m = 1 << s
        r = 1 << log2n-s+1
        twiddle = 0
        twiddles = Swap(np.arange(m//2), t=int)
        for k in range(0, n, r):
            for j in range(0, r//2):
                even        = A[k+j]
                odd         = A[k+j+r//2] * Weight(twiddles[twiddle], m)
                test.add(twiddles[twiddle]/m)
                A[k+j]      = even + odd
                A[k+j+r//2] = even - odd
            twiddle += 1
    A = Swap(A)
    return A

def FourierTransform2(x):
    n = len(x)
    A = Swap(x)
    log2n = int(np.log2(n))
    for s in range(1, log2n+1):
        m = 1 << s
        for k in range(0, n, m):
            for j in range(0, m//2):
                even        = A[k+j]
                odd         = A[k+j+m//2] * Weight(j, m)
                A[k+j]      = even + odd
                A[k+j+m//2] = even - odd
    return A

#x = [45, 207, 70, 41, 4, 180, 120, 216]
x = [(19+0.01j), (9.15+0.11j), (-12.16-0.48j), (-29.26+0.25j), (-15.99+0.69j), (-43.12-0.18j), (38-0.36j), (48.06-0.83j)]

X1 = np.fft.fft(x)
X2 = FourierTransform(x)
X3 = FourierTransform2(x)

assert(np.allclose(X1,X2))
assert(np.allclose(X1,X3))
