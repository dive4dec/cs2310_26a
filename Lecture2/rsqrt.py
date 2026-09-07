from time import perf_counter
from math import sqrt


# (1) Bisection for rsqrt(x)
def rsqrt_bisect(x, eps=1e-15):
    lo = 0.0
    hi = 1.0 if x >= 1.0 else 1.0 / x

    while hi - lo > eps:
        mid = (lo + hi) / 2.0

        if 1.0 / (mid * mid) > x:
            lo = mid
        else:
            hi = mid

    return (lo + hi) / 2.0


# (2) Newton iteration for rsqrt(x)
#
# y = 1/sqrt(x)
#
# y_{n+1}
#   = y * (3/2 - x*y*y/2)
#
def rsqrt_newton(x, eps=1e-15, verbose=True):
    y = 1.0 / x      # crude initial guess

    for i in range(30):
        y1 = y * (1.5 - 0.5 * x * y * y)

        d = abs(y1 - y)

        if verbose:
            print(
                f"   step {i+1}: "
                f"y = {y1:.15f} "
                f"(|y1-y| = {d:.2e})"
            )

        if d < eps * abs(y1):
            return y1

        y = y1

    return y


# (3) "Schoolbook" integer rsqrt
#
# sqrt_digits() computes floor(sqrt(x)).
# rsqrt is then approximated as 1/p.
#
def sqrt_digits(x):
    p = 0
    r = 0

    d = 1
    while d <= x // 100:
        d *= 100

    while d >= 1:
        group = (x // d) % 100

        r = r * 100 + group

        q = 0
        while (20 * p + q + 1) * (q + 1) <= r:
            q += 1

        r -= (20 * p + q) * q
        p = 10 * p + q

        d //= 100

    return p


def rsqrt_digits(x):
    return 1.0 / sqrt_digits(x)
