import random
import time

import matplotlib.pyplot as plt


def mod_pow(base, exp, mod):
    result = 1
    base %= mod
    while exp > 0:
        if exp % 2 == 1:
            result = (result * base) % mod
        base = (base * base) % mod
        exp //= 2
    return result


def is_prime_fermat(n, k=80):
    if n <= 4:
        return n == 2 or n == 3
    for _ in range(k):
        a = random.randint(2, n - 2)
        if mod_pow(a, n - 1, n) != 1:
            return False
    return True


def is_prime_miller_rabin(n, k=40):
    if n <= 4:
        return n == 2 or n == 3
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = mod_pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(s - 1):
            x = (x * x) % n
            if x == n - 1:
                break
        else:
            return False
    return True


def generate_large_number(bits):
    return random.getrandbits(bits) | 1 | (1 << (bits - 1))


def find_large_prime(bits, use_miller=True):
    while True:
        candidate = generate_large_number(bits)
        if (
            is_prime_miller_rabin(candidate)
            if use_miller
            else is_prime_fermat(candidate)
        ):
            return candidate


# Experiment setup
bit_lengths = list(range(32, 512, 4))
fermat_avg = []
miller_avg = []
trials = 5

for bits in bit_lengths:
    fermat_total = 0
    miller_total = 0

    for _ in range(trials):
        start = time.time()
        find_large_prime(bits, use_miller=False)
        fermat_total += time.time() - start

        start = time.time()
        find_large_prime(bits, use_miller=True)
        miller_total += time.time() - start

    fermat_avg.append(fermat_total / trials)
    miller_avg.append(miller_total / trials)
    print(f"{bits} bits done.")

# Plotting
plt.plot(bit_lengths, fermat_avg, "r--", label="Fermat (avg of 5)")
plt.plot(bit_lengths, miller_avg, "g-", label="Miller-Rabin (avg of 5)")
plt.xlabel("Bit Length")
plt.ylabel("Avg Time to Find Prime (s)")
plt.title("Smoothed Performance: Fermat vs Miller-Rabin")
plt.legend()
plt.grid(True)
plt.show()
