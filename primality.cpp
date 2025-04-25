// NOTE: GPT helped in structuring and writing most of the code!
#include <iostream>
#include <gmpxx.h>
#include <chrono>
#include <random>
#include <string>
#include <numeric>

using namespace std;
using namespace std::chrono;

const int SAMPLES_PER_DIGIT = 5;

// Random BigInt Generator
mpz_class generate_random_bigint(int digits) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 9);

    string num_str = "1"; // make sure it's not leading with zero
    for (int i = 1; i < digits; ++i) {
        num_str += to_string(dist(gen));
    }
    return mpz_class(num_str);
}

// Fermat Primality Test (GMP)
bool is_prime_fermat(const mpz_class& n, int k = 5) {
    if (n < 4) return n == 2 || n == 3;
    gmp_randclass rr(gmp_randinit_mt);
    rr.seed(time(NULL));

    mpz_class n_minus_1 = n - 1;

    for (int i = 0; i < k; i++) {
        mpz_class a = rr.get_z_range(n - 3) + 2;
        mpz_class res;
        mpz_powm(res.get_mpz_t(), a.get_mpz_t(), n_minus_1.get_mpz_t(), n.get_mpz_t());
        if (res != 1)
            return false;
    }
    return true;
}

// Miller-Rabin Primality Test (GMP)
bool is_prime_miller(const mpz_class& n, int k = 5) {
    if (n < 4) return n == 2 || n == 3;

    mpz_class d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    gmp_randclass rr(gmp_randinit_mt);
    rr.seed(time(NULL));

    for (int i = 0; i < k; ++i) {
        mpz_class a = rr.get_z_range(n - 3) + 2;
        mpz_class x;
        mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
        if (x == 1 || x == n - 1)
            continue;

        bool cont = false;
        for (int r = 1; r < s; ++r) {
            mpz_powm_ui(x.get_mpz_t(), x.get_mpz_t(), 2, n.get_mpz_t());
            if (x == n - 1) {
                cont = true;
                break;
            }
        }

        if (!cont)
            return false;
    }

    return true;
}

int main() {
    cout << "Digits,AvgFermat(us),AvgMillerRabin(us)" << endl;

    for (int digits = 100; digits <= 10000; digits += 10) {
        vector<long long> fermat_times;
        vector<long long> miller_times;

        for (int i = 0; i < SAMPLES_PER_DIGIT; ++i) {
            mpz_class num = generate_random_bigint(digits);

            auto start = high_resolution_clock::now();
            is_prime_fermat(num);
            auto end = high_resolution_clock::now();
            fermat_times.push_back(duration_cast<microseconds>(end - start).count());

            start = high_resolution_clock::now();
            is_prime_miller(num);
            end = high_resolution_clock::now();
            miller_times.push_back(duration_cast<microseconds>(end - start).count());
        }

        auto avg_fermat = accumulate(fermat_times.begin(), fermat_times.end(), 0LL) / SAMPLES_PER_DIGIT;
        auto avg_miller = accumulate(miller_times.begin(), miller_times.end(), 0LL) / SAMPLES_PER_DIGIT;

        cout << digits << "," << avg_fermat << "," << avg_miller << endl;
    }

    return 0;
}

