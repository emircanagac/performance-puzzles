import java.util.Arrays;

public class Primes {

    // Parse string input into a numeric limit, supporting exponential notation (e.g., "10**5")
    public static long parseLimit(String input) {
        int idx = input.indexOf("**");
        if (idx != -1) {
            long base = Long.parseLong(input.substring(0, idx));
            long exp = Long.parseLong(input.substring(idx + 2));
            return (long) Math.pow(base, exp);
        }
        return Long.parseLong(input);
    }

    // Method 1: Sieve of Eratosthenes
    // Optimized by skipping even numbers. The boolean array only maps odd numbers starting from 3.
    // Index k maps to the integer value: (2 * k + 3).
    public static long[] sieveEratosthenes(long n) {
        if (n < 2) return new long[]{0, 0};

        int maxK = (int) ((n - 3) / 2);
        boolean[] marked = new boolean[maxK + 1];

        long count = 1; // 2 is implicitly prime
        long last = 2;

        for (long p = 3; p * p <= n; p += 2) {
            int k = (int) ((p - 3) / 2);
            if (!marked[k]) {
                // Mark all odd multiples of p, starting directly from p^2
                for (long j = p * p; j <= n; j += 2 * p) {
                    marked[(int) ((j - 3) / 2)] = true;
                }
            }
        }

        // Count all unmarked indices and record the highest prime found
        for (int k = 0; k <= maxK; k++) {
            if (!marked[k]) {
                count++;
                last = 2L * k + 3;
            }
        }

        return new long[]{count, last};
    }

    // Method 2: Naive Trial Division
    // Tests each odd number for primality by attempting division up to its square root.
    public static long[] naiveTrialDivision(long n) {
        if (n < 2) return new long[]{0, 0};

        long count = 1; // 2 is implicitly prime
        long last = 2;

        // Iterate through all odd numbers up to N
        for (long num = 3; num <= n; num += 2) {
            boolean isPrime = true;
            // Test odd divisors up to the square root of num
            for (long i = 3; i * i <= num; i += 2) {
                if (num % i == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) {
                count++;
                last = num;
            }
        }

        return new long[]{count, last};
    }

    public static void main(String[] args) {
        String input = args.length > 0 ? args[0] : "100";
        long n;

        try {
            n = parseLimit(input);
        } catch (Exception e) {
            System.err.println("Error: Invalid limit");
            System.exit(1);
            return;
        }

        System.out.printf("Target: primes in range [0, %d]\n", n);
        System.out.println("Benchmarking methods sequentially...");
        System.out.println("---");

        // Benchmark Naive Trial Division
        long startNaive = System.nanoTime();
        long[] naiveResult = naiveTrialDivision(n);
        double naiveTime = (System.nanoTime() - startNaive) / 1e9;

        // Benchmark Sieve of Eratosthenes
        long startSieve = System.nanoTime();
        long[] sieveResult = sieveEratosthenes(n);
        double sieveTime = (System.nanoTime() - startSieve) / 1e9;

        // Verify algorithms yield identical results
        if (naiveResult[0] != sieveResult[0]) {
            System.err.printf("ERROR: Result mismatch. Naive: %d, Sieve: %d\n", naiveResult[0], sieveResult[0]);
            System.exit(1);
        }

        System.out.printf("Found: %d primes | Last prime: %d\n\n", sieveResult[0], sieveResult[1]);
        System.out.println("Execution Times:");
        System.out.printf("  %-32s : %.6fs\n", "Naive Trial Division (Java)", naiveTime);
        System.out.printf("  %-32s : %.6fs\n", "Sieve of Eratosthenes (Java)", sieveTime);
        System.out.println("---");
    }
}
