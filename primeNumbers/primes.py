import sys
import time

# Parse string input into a numeric limit, supporting exponential notation (e.g., "10**5")
def parse_limit(input_str):
    if "**" in input_str:
        base, exp = input_str.split("**")
        return int(base) ** int(exp)
    return int(input_str)

# Method 1: Sieve of Eratosthenes
# Optimized by skipping even numbers. A bytearray is used for memory and speed efficiency.
# Index k maps to the integer value: (2 * k + 3).
def sieve_eratosthenes(n):
    if n < 2:
        return 0, 0

    max_k = (n - 3) // 2
    # bytearray is significantly faster than a boolean list in Python
    marked = bytearray(max_k + 1)

    count = 1  # 2 is implicitly prime
    last = 2

    p = 3
    while p * p <= n:
        k = (p - 3) // 2
        if not marked[k]:
            # Mark all odd multiples of p, starting directly from p^2
            for j in range(p * p, n + 1, 2 * p):
                marked[(j - 3) // 2] = 1
        p += 2

    # Count all unmarked indices and record the highest prime found
    for k in range(max_k + 1):
        if not marked[k]:
            count += 1
            last = 2 * k + 3

    return count, last

# Method 2: Naive Trial Division
# Tests each odd number for primality by attempting division up to its square root.
def naive_trial_division(n):
    if n < 2:
        return 0, 0

    count = 1  # 2 is implicitly prime
    last = 2

    # Iterate through all odd numbers up to N
    num = 3
    while num <= n:
        is_prime = True
        i = 3
        # Test odd divisors up to the square root of num
        while i * i <= num:
            if num % i == 0:
                is_prime = False
                break
            i += 2

        if is_prime:
            count += 1
            last = num
        num += 2

    return count, last

def main():
    input_str = sys.argv[1] if len(sys.argv) > 1 else "100"

    try:
        n = parse_limit(input_str)
    except ValueError:
        print("Error: Invalid limit", file=sys.stderr)
        sys.exit(1)

    print(f"Target: primes in range [0, {n}]")
    print("Benchmarking methods sequentially...")
    print("---")

    # Benchmark Naive Trial Division
    start_naive = time.perf_counter()
    naive_count, _ = naive_trial_division(n)
    naive_time = time.perf_counter() - start_naive

    # Benchmark Sieve of Eratosthenes
    start_sieve = time.perf_counter()
    sieve_count, sieve_last = sieve_eratosthenes(n)
    sieve_time = time.perf_counter() - start_sieve

    # Verify algorithms yield identical results
    if naive_count != sieve_count:
        print(f"ERROR: Result mismatch. Naive: {naive_count}, Sieve: {sieve_count}", file=sys.stderr)
        sys.exit(1)

    print(f"Found: {sieve_count} primes | Last prime: {sieve_last}\n")
    print("Execution Times:")
    print(f"  {'Naive Trial Division (Python)':<32} : {naive_time:.6f}s")
    print(f"  {'Sieve of Eratosthenes (Python)':<32} : {sieve_time:.6f}s")
    print("---")

if __name__ == "__main__":
    main()
