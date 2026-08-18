#!/bin/sh
# POSIX-compliant Prime Finder & Benchmark

set -e

# Parse string input into a numeric limit, supporting exponential notation (e.g., "10**5")
expr=$(printf '%s' "${1:-100}" | sed 's/\*\*/^/g')
N=$(awk "BEGIN { printf \"%d\", $expr }" 2>/dev/null)

case "$N" in
  ''|*[!0-9]*) echo "Error: Invalid target limit '$1'" >&2; exit 1 ;;
esac

# Method 1: Sieve of Eratosthenes (Awk)
# Optimized by skipping even numbers. The array only maps odd numbers starting from 3.
sieve_eratosthenes() {
  awk -v N="$N" 'BEGIN {
    if (N >= 2) { count = 1; last = 2 }

    # Mark all odd multiples of p, starting directly from p^2
    for (p = 3; p * p <= N; p += 2) {
      if (!marked[p]) {
        for (j = p * p; j <= N; j += 2 * p) marked[j] = 1
      }
    }

    # Count all unmarked indices and record the highest prime found
    for (k = 3; k <= N; k += 2) {
      if (!marked[k]) { count++; last = k }
    }

    print (count + 0), (last + 0)
  }'
}

# Method 2: Naive Trial Division (Pure Shell)
# Tests each odd number for primality by attempting division up to its square root.
naive_trial_division() {
  count=0; last=0
  [ "$N" -ge 2 ] && { count=1; last=2; }

  # Iterate through all odd numbers up to N
  num=3
  while [ "$num" -le "$N" ]; do
    is_prime=1; i=3
    # Test odd divisors up to the square root of num
    while [ $((i * i)) -le "$num" ]; do
      if [ $((num % i)) -eq 0 ]; then is_prime=0; break; fi
      i=$((i + 2))
    done
    if [ "$is_prime" -eq 1 ]; then
      count=$((count + 1))
      last=$num
    fi
    num=$((num + 2))
  done
  echo "$count $last"
}

# Retrieve high-precision time in nanoseconds and run benchmarks
now_ns() {
  date +%s%N | sed 's/N.*/000000000/'
}

run_bench() {
  func_name=$1
  start_ns=$(now_ns)
  output=$("$func_name")
  end_ns=$(now_ns)
  echo "$((end_ns - start_ns))|$output" > "/tmp/primes_$$.$func_name"
}

echo "Target: primes in range [0, $N]"
echo "Benchmarking methods in parallel..."
echo "---"

run_bench naive_trial_division &
run_bench sieve_eratosthenes &
wait

IFS="|" read -r naive_ns naive_out < "/tmp/primes_$$.naive_trial_division"
IFS="|" read -r sieve_ns sieve_out < "/tmp/primes_$$.sieve_eratosthenes"
rm -f "/tmp/primes_$$.naive_trial_division" "/tmp/primes_$$.sieve_eratosthenes"

naive_count="${naive_out% *}"
sieve_count="${sieve_out% *}"
sieve_last="${sieve_out#* }"

# Verify algorithms yield identical results
if [ "$naive_count" != "$sieve_count" ]; then
  echo "ERROR: Result mismatch. Naive: $naive_count, Sieve: $sieve_count" >&2
  exit 1
fi

awk -v count="$sieve_count" -v last="$sieve_last" \
    -v t_naive="${naive_ns:-0}" -v t_sieve="${sieve_ns:-0}" '
BEGIN {
  printf "Found: %d primes | Last prime: %d\n\n", count, last
  printf "Execution Times:\n"
  printf "  %-32s : %.3fs\n", "Naive Trial Division (Shell)", t_naive / 1e9
  printf "  %-32s : %.3fs\n", "Sieve of Eratosthenes (Awk)",   t_sieve / 1e9
}'

echo "---"

if [ "$N" -ge 2 ]; then
  if factor "$sieve_last" | grep -q ": $sieve_last$"; then
    echo "[OK] Last prime verified with factor(1)."
  else
    echo "[FAIL] Verification failed for $sieve_last." >&2
    exit 1
  fi
else
  echo "[OK] No primes to verify."
fi
