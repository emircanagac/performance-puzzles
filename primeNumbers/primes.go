package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

// Parse string input into a numeric limit, supporting exponential notation (e.g., "10**5")
func parseLimit(input string) int64 {
	idx := strings.Index(input, "**")
	if idx != -1 {
		base, err1 := strconv.ParseInt(input[:idx], 10, 64)
		exp, err2 := strconv.ParseInt(input[idx+2:], 10, 64)
		if err1 == nil && err2 == nil {
			res := int64(1)
			for i := int64(0); i < exp; i++ {
				res *= base
			}
			return res
		}
	}
	val, _ := strconv.ParseInt(input, 10, 64)
	return val
}

// Method 1: Sieve of Eratosthenes
func sieveEratosthenes(n int64) (int64, int64) {
	if n < 2 {
		return 0, 0
	}

	maxK := (n - 3) / 2
	marked := make([]bool, maxK+1)

	count := int64(1)
	last := int64(2)

	for p := int64(3); p*p <= n; p += 2 {
		k := (p - 3) / 2
		if !marked[k] {
			for j := p * p; j <= n; j += 2*p {
				marked[(j-3)/2] = true
			}
		}
	}

	for k := int64(0); k <= maxK; k++ {
		if !marked[k] {
			count++
			last = 2*k + 3
		}
	}

	return count, last
}

// Method 2: Naive Trial Division
func naiveTrialDivision(n int64) (int64, int64) {
	if n < 2 {
		return 0, 0
	}

	count := int64(1)
	last := int64(2)

	for num := int64(3); num <= n; num += 2 {
		isPrime := true
		for i := int64(3); i*i <= num; i += 2 {
			if num%i == 0 {
				isPrime = false
				break
			}
		}
		if isPrime {
			count++
			last = num
		}
	}

	return count, last
}

func main() {
	input := "100"
	if len(os.Args) > 1 {
		input = os.Args[1]
	}

	n := parseLimit(input)
	if n <= 0 {
		fmt.Fprintln(os.Stderr, "Error: Invalid limit")
		os.Exit(1)
	}

	fmt.Printf("Target: primes in range [0, %d]\n", n)
	fmt.Println("Benchmarking methods sequentially...")
	fmt.Println("---")

	// Benchmark Naive Trial Division
	startNaive := time.Now()
	naiveCount, _ := naiveTrialDivision(n) // Fixed: Using '_' to ignore unused return value
	naiveTime := time.Since(startNaive).Seconds()

	// Benchmark Sieve of Eratosthenes
	startSieve := time.Now()
	sieveCount, sieveLast := sieveEratosthenes(n)
	sieveTime := time.Since(startSieve).Seconds()

	// Verify algorithms yield identical results
	if naiveCount != sieveCount {
		fmt.Fprintf(os.Stderr, "ERROR: Result mismatch. Naive: %d, Sieve: %d\n", naiveCount, sieveCount)
		os.Exit(1)
	}

	fmt.Printf("Found: %d primes | Last prime: %d\n\n", sieveCount, sieveLast)
	fmt.Println("Execution Times:")
	fmt.Printf("  %-32s : %.6fs\n", "Naive Trial Division (Go)", naiveTime)
	fmt.Printf("  %-32s : %.6fs\n", "Sieve of Eratosthenes (Go)", sieveTime)
	fmt.Println("---")
}
