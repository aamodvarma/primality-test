#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <cmath>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using namespace std;
using namespace chrono;

// Fast modular exponentiation
uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (__uint128_t(result) * base) % mod;
        base = (__uint128_t(base) * base) % mod;
        exp >>= 1;
    }
    return result;
}

bool is_prime_fermat(uint64_t n, int iter = 5) {
    if (n <= 4) return n == 2 || n == 3;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint64_t> dist(2, n - 2);
    for (int i = 0; i < iter; i++) {
        uint64_t a = dist(gen);
        if (mod_pow(a, n - 1, n) != 1)
            return false;
    }
    return true;
}

bool is_prime_miller_rabin(uint64_t n, int iter = 5) {
    if (n <= 4) return n == 2 || n == 3;
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint64_t> dist(2, n - 2);

    for (int i = 0; i < iter; i++) {
        uint64_t a = dist(gen);
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool continueLoop = false;
        for (int r = 0; r < s - 1; r++) {
            x = (__uint128_t(x) * x) % n;
            if (x == n - 1) {
                continueLoop = true;
                break;
            }
        }
        if (!continueLoop)
            return false;
    }
    return true;
}

// Generate a random large number with given bit length
uint64_t generate_large_number(int bits) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dist((1ULL << (bits - 1)) + 1, (1ULL << bits) - 1);
    return dist(gen) | 1; // ensure odd
}

// Find a large prime using specified test
uint64_t find_large_prime(int bits, bool use_miller) {
    while (true) {
        uint64_t candidate = generate_large_number(bits);
        if (use_miller ? is_prime_miller_rabin(candidate) : is_prime_fermat(candidate))
            return candidate;
    }
}

int main() {
    vector<int> bit_sizes = {32, 36, 40, 44, 48, 52, 56, 60}; // you can increase this
    vector<double> times_fermat, times_miller;

    for (int bits : bit_sizes) {
        auto start_f = high_resolution_clock::now();
        find_large_prime(bits, false);
        auto end_f = high_resolution_clock::now();
        times_fermat.push_back(duration<double>(end_f - start_f).count());

        auto start_m = high_resolution_clock::now();
        find_large_prime(bits, true);
        auto end_m = high_resolution_clock::now();
        times_miller.push_back(duration<double>(end_m - start_m).count());

        cout << "Bits: " << bits << " done.\n";
    }

    plt::plot(bit_sizes, times_fermat, "r--", {{"label", "Fermat"}});
    plt::plot(bit_sizes, times_miller, "g-", {{"label", "Miller-Rabin"}});
    plt::xlabel("Bit Length of Prime");
    plt::ylabel("Time (seconds)");
    plt::title("Primality Test Performance");
    plt::legend();
    plt::show();

    return 0;
}

