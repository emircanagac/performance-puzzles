use std::env;
use std::time::Instant;

// Parse string input into a numeric limit, supporting exponential notation (e.g., "10**5")
fn parse_limit(input: &str) -> Option<u64> {
    if let Some(idx) = input.find("**") {
        let base: u64 = input[..idx].parse().ok()?;
        let exp: u32 = input[idx + 2..].parse().ok()?;
        Some(base.pow(exp))
    } else {
        input.parse().ok()
    }
}

// Method 1: Sieve of Eratosthenes
fn sieve_eratosthenes(n: u64) -> (u64, u64) {
    if n < 2 { return (0, 0); }

    let max_k = ((n - 3) / 2) as usize;
    let mut marked = vec![false; max_k + 1];

    let mut count = 1;
    let mut last = 2;

    let mut p: u64 = 3;
    while p * p <= n {
        let k = ((p - 3) / 2) as usize;
        if !marked[k] {
            let mut j = p * p;
            while j <= n {
                marked[((j - 3) / 2) as usize] = true;
                j += 2 * p;
            }
        }
        p += 2;
    }

    for k in 0..=max_k {
        if !marked[k] {
            count += 1;
            last = 2 * (k as u64) + 3;
        }
    }

    (count, last)
}

// Method 2: Naive Trial Division
fn naive_trial_division(n: u64) -> (u64, u64) {
    if n < 2 { return (0, 0); }

    let mut count = 1;
    let mut last = 2;

    let mut num = 3;
    while num <= n {
        let mut is_prime = true;
        let mut i = 3;
        while i * i <= num {
            if num % i == 0 {
                is_prime = false;
                break;
            }
            i += 2;
        }
        if is_prime {
            count += 1;
            last = num;
        }
        num += 2;
    }

    (count, last)
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let input = if args.len() > 1 { &args[1] } else { "100" };

    let n = match parse_limit(input) {
        Some(val) => val,
        None => {
            eprintln!("Error: Invalid limit");
            std::process::exit(1);
        }
    };

    println!("Target: primes in range [0, {}]", n);
    println!("Benchmarking methods sequentially...");
    println!("---");

    // Benchmark Naive Trial Division
    let start_naive = Instant::now();
    let (naive_count, _) = naive_trial_division(n); // Fixed: Using '_' to suppress warning
    let naive_time = start_naive.elapsed().as_secs_f64();

    // Benchmark Sieve of Eratosthenes
    let start_sieve = Instant::now();
    let (sieve_count, sieve_last) = sieve_eratosthenes(n);
    let sieve_time = start_sieve.elapsed().as_secs_f64();

    // Verify algorithms yield identical results
    if naive_count != sieve_count {
        eprintln!("ERROR: Result mismatch. Naive: {}, Sieve: {}", naive_count, sieve_count);
        std::process::exit(1);
    }

    println!("Found: {} primes | Last prime: {}\n", sieve_count, sieve_last);
    println!("Execution Times:");
    println!("  {:<32} : {:.6}s", "Naive Trial Division (Rust)", naive_time);
    println!("  {:<32} : {:.6}s", "Sieve of Eratosthenes (Rust)", sieve_time);
    println!("---");
}
