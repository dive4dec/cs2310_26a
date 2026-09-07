from time import perf_counter

from math import gcd   # Lehmer's algorithm

# The lecture’s example.
def gcd_euclid(a, b):
    while b:
        a, b = b, a % b
    return a

# Stein’s binary GCD
def gcd_binary(a, b):
    if a == 0:
        return b
    if b == 0:
        return a

    shift = 0
    while ((a | b) & 1) == 0:
        a >>= 1
        b >>= 1
        shift += 1

    while (a & 1) == 0:
        a >>= 1

    while b:
        while (b & 1) == 0:
            b >>= 1
        if a > b:
            a, b = b, a
        b -= a
    return a << shift


def fib(n):
    a, b = 0, 1
    for _ in range(n):
        a, b = b, a + b
    return a