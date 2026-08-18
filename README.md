# Performance Puzzles

A collection of computational benchmarks, algorithms, and performance experiments across different programming languages.

The goal of this repository is not just to say "Language X is faster than Language Y," but to demonstrate how **algorithmic complexity** (e.g., $O(N \sqrt{N})$ vs. $O(N \log \log N)$) often outweighs raw language speed, and to showcase the extreme differences between interpreted scripts, virtual machines, and compiled systems programming languages.

## Puzzles Catalog

| Puzzle | Description | Languages |
| :--- | :--- | :--- |
| [**Prime Numbers**](./primeNumbers/) | Comparing Naive Trial Division vs. Sieve of Eratosthenes. | Bash/Awk, C, Go, Java, Python, Rust |
| *(More coming soon)* | ... | ... |

## Philosophy
- **No Dependencies:** All scripts and codes use standard libraries only.
- **POSIX Compliant:** Shell scripts are strictly POSIX `sh` to ensure they run on any Linux/macOS environment without `bash` or `zsh` dependencies.
- **Max Optimization:** Compiled languages use their respective maximum optimization flags (e.g., `-O3` for GCC, `-O` for Rust).

## Contributing
Contributions are welcome! If you want to translate a puzzle into a new language (e.g., Zig, C++, C#) or propose an entirely new computational puzzle, feel free to open a Pull Request.

---
*Created for educational purposes to explore algorithms and compiler behaviors.*
