# Prime Number Benchmark: Sieve vs. Naive

This puzzle finds all prime numbers up to a given limit (10,000,000), comparing two different algorithms across five programming environments.

The goal is to demonstrate how **algorithmic complexity** often outweighs raw language speed. A great algorithm in a "slow" language can easily beat a terrible algorithm in a "fast" language.

*(Note: A POSIX shell script `primes.sh` is included in the source code as a system baseline, but is excluded from this 10 million benchmark as it would take hours to finish).*

## The Algorithms

1. **Naive Trial Division:** $O(N \sqrt{N})$
   Iterates through every odd number and tries to divide it by all odd numbers up to its square root. Simple, but computationally expensive due to heavy modulo arithmetic.
2. **Sieve of Eratosthenes (Odd-Only):** $O(N \log \log N)$
   Allocates a boolean array/slice for odd numbers and eliminates multiples of primes. Incredibly fast as it relies on memory access and simple addition rather than heavy division.

## Benchmark Results (Target: 10,000,000)

All tests were executed sequentially on the same hardware.

| Language / Environment | Naive Trial Division | Sieve of Eratosthenes |
| :--- | :--- | :--- |
| **Rust (`rustc -O`)** | ~ 2.993 s | **~ 0.022 s** |
| **C (`gcc -O3`)** | ~ 7.972 s | **~ 0.026 s** |
| **Go (`go build`)** | ~ 8.623 s | **~ 0.039 s** |
| **Java (`javac` / JVM)** | ~ 8.367 s | **~ 0.049 s** |
| **Python (`python3`)** | ~ 64.269 s | **~ 0.829 s** |

> **The Grand Takeaways:**
> 1. **Algorithm > Language:** Python is widely considered "slow" for heavy math. However, Python using the Sieve algorithm (**0.82s**) absolutely destroyed C using the Naive algorithm (**7.97s**). Mathematics always wins!
> 2. **Compiler Magic:** In the heavy division loop (Naive test), Rust finished in ~3 seconds while C and Go took ~8 seconds. This highlights how different compiler backends (LLVM vs GCC) optimize loop unrolling and modulo arithmetic differently.
> 3. **The Power of JIT:** Java starts with a "cold" JVM, but its Just-In-Time compiler optimizes the math on the fly, allowing it to perform head-to-head with Go and C on the brute-force test.
> 4. **Memory is King:** Across all languages, memory-mapping addition (Sieve) was astronomically faster than CPU modulo division (Naive).

## How to Run

To keep the repository clean, we compile the binaries directly into the system's `/tmp/` directory.

### 1. C
Compile with maximum optimization, then run.
```bash
gcc -O3 primes.c -o /tmp/primes_c
/tmp/primes_c 10**7
```

### 2. Rust
Compile with the release flag, then run.
```bash
rustc -O primes.rs -o /tmp/primes_rust
/tmp/primes_rust 10**7
```

### 3. Go (Golang)
Build the binary, then run.
```bash
go build -o /tmp/primes_go primes.go
/tmp/primes_go 10**7
```

### 4. Java
Compile the class into the `/tmp/` directory, then run the JVM.
```bash
javac Primes.java -d /tmp/
java -cp /tmp/ Primes 10**7
```

### 5. Python
No compilation needed. Runs directly via the Python interpreter.
```bash
python3 primes.py 10**7
```

### 6. Bash / Awk (Baseline)
Runs on any UNIX-like system. *(Warning: Do not run Naive for numbers larger than `10**5`)*.
```bash
./primes.sh 10**5
```
