#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Parse string input into a numeric limit, supporting exponential notation (e.g., "10**5")
long long parse_limit(char *str) {
    char *exp_ptr = strstr(str, "**");
    if (exp_ptr) {
        *exp_ptr = '\0';
        long long base = atoll(str);
        long long exp = atoll(exp_ptr + 2);
        long long res = 1;
        for (long long i = 0; i < exp; i++) res *= base;
        return res;
    }
    return atoll(str);
}

// Retrieve high-precision monotonic time in seconds
double get_time_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Method 1: Sieve of Eratosthenes
// Optimized by skipping even numbers. The boolean array only maps odd numbers starting from 3.
// Index k maps to the integer value: (2 * k + 3).
void sieve_eratosthenes(long long N, long long *out_count, long long *out_last) {
    if (N < 2) { *out_count = 0; *out_last = 0; return; }

    // Calculate the maximum index required for odd numbers up to N
    long long max_k = (N - 3) / 2;
    bool *marked = calloc(max_k + 1, sizeof(bool));

    long long count = 1; // 2 is implicitly prime
    long long last = 2;

    // Process each odd number up to the square root of N
    for (long long p = 3; p * p <= N; p += 2) {
        long long k = (p - 3) / 2;
        if (!marked[k]) {
            // Mark all odd multiples of p, starting directly from p^2
            for (long long j = p * p; j <= N; j += 2 * p) {
                marked[(j - 3) / 2] = true;
            }
        }
    }

    // Count all unmarked indices and record the highest prime found
    for (long long k = 0; k <= max_k; k++) {
        if (!marked[k]) {
            count++;
            last = 2 * k + 3;
        }
    }

    free(marked);
    *out_count = count;
    *out_last = last;
}

// Method 2: Naive Trial Division
// Tests each odd number for primality by attempting division up to its square root.
void naive_trial_division(long long N, long long *out_count, long long *out_last) {
    if (N < 2) { *out_count = 0; *out_last = 0; return; }

    long long count = 1; // 2 is implicitly prime
    long long last = 2;

    // Iterate through all odd numbers up to N
    for (long long num = 3; num <= N; num += 2) {
        bool is_prime = true;
        // Test odd divisors up to the square root of num
        for (long long i = 3; i * i <= num; i += 2) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            count++;
            last = num;
        }
    }

    *out_count = count;
    *out_last = last;
}

int main(int argc, char **argv) {
    char input[64] = "100";
    if (argc > 1) strncpy(input, argv[1], 63);

    long long N = parse_limit(input);
    if (N < 0) {
        fprintf(stderr, "Error: Invalid limit\n");
        return 1;
    }

    printf("Target: primes in range [0, %lld]\n", N);
    printf("Benchmarking methods sequentially...\n");
    printf("---\n");

    long long naive_count = 0, naive_last = 0;
    long long sieve_count = 0, sieve_last = 0;

    // Benchmark Naive Trial Division
    double t0 = get_time_sec();
    naive_trial_division(N, &naive_count, &naive_last);
    double t1 = get_time_sec();
    double naive_time = t1 - t0;

    // Benchmark Sieve of Eratosthenes
    t0 = get_time_sec();
    sieve_eratosthenes(N, &sieve_count, &sieve_last);
    t1 = get_time_sec();
    double sieve_time = t1 - t0;

    // Verify algorithms yield identical results
    if (naive_count != sieve_count) {
        fprintf(stderr, "ERROR: Result mismatch. Naive: %lld, Sieve: %lld\n", naive_count, sieve_count);
        return 1;
    }

    printf("Found: %lld primes | Last prime: %lld\n\n", sieve_count, sieve_last);
    printf("Execution Times:\n");
    printf("  %-32s : %.6fs\n", "Naive Trial Division (C)", naive_time);
    printf("  %-32s : %.6fs\n", "Sieve of Eratosthenes (C)", sieve_time);
    printf("---\n");

    return 0;
}
