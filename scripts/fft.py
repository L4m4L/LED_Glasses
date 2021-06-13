import numpy as np

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
                A[k+j]      = even + odd
                A[k+j+r//2] = even - odd
            twiddle += 1
    A = Swap(A)
    return A

def FourierTransform1(x):
    A = np.array(x, dtype=np.complex128)
    A = Swap(A)
    n = len(x)
    log2n = int(np.log2(n))
    for s in range(1, log2n+1):
        m = 1 << s
        r = 1 << log2n-s+1
        twiddle = 0
        twiddles = np.arange(m//2)
        #twiddles = Swap(twiddles, t=int)
        for k in range(0, n, r):
            for j in range(0, r//2):
                even        = A[k+j]
                odd         = A[k+j+r//2] * Weight(twiddles[twiddle], m)
                A[k+j]      = even + odd
                A[k+j+r//2] = even - odd
            twiddle += 1
    return A

x = np.random.random(1024)

X1 = np.fft.fft(x)
X2 = FourierTransform(x)
X3 = FourierTransform1(x)

assert(np.allclose(X1,X2))
assert(np.allclose(X1,X3))
