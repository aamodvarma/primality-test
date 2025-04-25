# NOTE: GPT helped in generating code for Solovay and main loop and fixing any error along the way.
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


def is_prime_naive(n):
    if n < 2:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0:
        return False
    for i in range(3, int(n**0.5) + 1, 2):
        if n % i == 0:
            return False
    return True


def is_prime_fermat(n, k=80):
    if n <= 4:
        return n == 2 or n == 3
    for _ in range(k):
        a = random.randint(2, n - 2)
        if mod_pow(a, n - 1, n) != 1:
            return False
    return True


def jacobi(a, n):
    if a == 0:
        return 0
    if a == 1:
        return 1
    if a % 2 == 0:
        r = jacobi(a // 2, n)
        if n % 8 in [3, 5]:
            r = -r
    else:
        r = jacobi(n % a, a)
        if a % 4 == 3 and n % 4 == 3:
            r = -r
    return r


def is_prime_solovay(n, k=80):
    if n < 2 or n % 2 == 0:
        return False
    for _ in range(k):
        a = random.randint(2, n - 1)
        x = jacobi(a, n)
        if x == 0 or mod_pow(a, (n - 1) // 2, n) != (x % n):
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


def find_large_prime(bits, method):
    while True:
        n = generate_large_number(bits)
        if method(n):
            return n


bit_lengths = list(range(1024, 2048, 32))
trials = 10
results = {
    "Naive": [],
    "Fermat": [],
    "Solovay": [],
    "Miller-Rabin": [],
}

for bits in bit_lengths:
    print(f"\nTesting {bits}-bit primes...")

    naive_total = 0
    if bits <= 1:
        for _ in range(trials):
            start = time.time()
            find_large_prime(bits, is_prime_naive)
            naive_total += time.time() - start
        results["Naive"].append(naive_total / trials)
    else:
        results["Naive"].append(None)

    fermat_total = 0
    for _ in range(trials):
        start = time.time()
        find_large_prime(bits, is_prime_fermat)
        fermat_total += time.time() - start
    results["Fermat"].append(fermat_total / trials)

    solovay_total = 0
    for _ in range(trials):
        start = time.time()
        find_large_prime(bits, is_prime_solovay)
        solovay_total += time.time() - start
    results["Solovay"].append(solovay_total / trials)

    miller_total = 0
    for _ in range(trials):
        start = time.time()
        find_large_prime(bits, is_prime_miller_rabin)
        miller_total += time.time() - start
    results["Miller-Rabin"].append(miller_total / trials)

for name, times in results.items():
    if any(t is not None for t in times):
        clean_bits = [b for b, t in zip(bit_lengths, times) if t is not None]
        clean_times = [t for t in times if t is not None]
        plt.plot(clean_bits, clean_times, label=name)

plt.xlabel("Bit Length")
plt.ylabel("Avg Time to Find Prime (s)")
plt.title("Primality Test Comparison (avg over 5 runs)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
