#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <random>

using namespace boost::multiprecision;
using namespace std;

typedef cpp_int BigInt;

// Miller-Rabin Primality Test
bool is_prime(const BigInt& n, int k = 50) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    BigInt d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<BigInt> dist(2, n - 2);

    for (int i = 0; i < k; ++i) {
        BigInt a = dist(gen);
        BigInt x = powm(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool continue_outer = false;
        for (int j = 0; j < r - 1; ++j) {
            x = powm(x, 2, n);
            if (x == n - 1) {
                continue_outer = true;
                break;
            }
        }
        if (continue_outer) continue;
        return false;
    }

    return true;
}

bool fermat_primality_test(const BigInt& n, int k = 100) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<BigInt> dist(2, n - 2);

    for (int i = 0; i < k; ++i) {
        BigInt a = dist(gen);
        if (powm(a, n - 1, n) != 1)
            return false;  // definitely composite
    }

    return true; // probably prime
}

bool naive_is_prime(const BigInt& n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    for (BigInt i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }

    return true;
}
string generate_random_number_string(int digits) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> digit_dist(0, 9);
    uniform_int_distribution<> non_zero_dist(1, 9);
    uniform_int_distribution<> odd_digit_index(0, 4);
    array<int, 5> odd_digits = {1, 3, 5, 7, 9};

    string result;
    result += to_string(non_zero_dist(gen));  // First digit non-zero

    for (int i = 1; i < digits - 1; ++i) {
        result += to_string(digit_dist(gen));
    }

    result += to_string(odd_digits[odd_digit_index(gen)]);  // Last digit odd
    return result;
}


int jacobi(const BigInt& a, const BigInt& n) {
    if (n <= 0 || n % 2 == 0) return 0; // n must be positive and odd

    BigInt a_copy = a % n;
    BigInt n_copy = n;
    int result = 1;

    while (a_copy != 0) {
        while (a_copy % 2 == 0) {
            a_copy /= 2;
            BigInt r = n_copy % 8;
            if (r == 3 || r == 5) result = -result;
        }

        std::swap(a_copy, n_copy);

        if (a_copy % 4 == 3 && n_copy % 4 == 3)
            result = -result;

        a_copy %= n_copy;
    }

    return n_copy == 1 ? result : 0;
}

// Solovay-Strassen primality test
bool solovay_strassen(const BigInt& n, int k = 10) {
    if (n <= 2) return (n == 2);
    if (n % 2 == 0) return false;

    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<BigInt> dist(2, n - 2);

    for (int i = 0; i < k; ++i) {
        BigInt a = dist(gen);
        int jacobi_val = jacobi(a, n);
        if (jacobi_val == 0) return false;

        BigInt mod_exp = powm(a, (n - 1) / 2, n);
        BigInt jacobi_mod_n =  jacobi_val;
        if (jacobi_val < 0) {
            BigInt jacobi_mod_n =  n - 1;
        } 


        if (mod_exp != jacobi_mod_n) return false;
    }

    return true;
}

int main() {
    cout << "Digits,Time(s)" << endl;
    int total = 3000;

    for (int digits = 1; digits <= total; digits += 100) {
        string num_str = generate_random_number_string(digits);
        BigInt candidate(num_str);

        auto start = chrono::high_resolution_clock::now();
        is_prime(candidate, 10);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;
        cout << digits << "," << elapsed.count() << endl;
    }

    return 0;
}

