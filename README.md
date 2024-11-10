# Hamming Number Calculator

This C++ project calculates "Hamming numbers"—positive integers that have no prime factors other than 2, 3, or 5—using multiple approaches with varying efficiency and memory optimization.

## Project Overview

The Hamming Number Calculator provides three algorithms for computing Hamming numbers:

1. **Naive Approach (`Solve_Naive`)**  
   Uses a min-heap priority queue and a set for tracking unique values. This approach has higher memory usage and slower performance, making it suitable for smaller inputs.

2. **Efficient Approach (`Solve_Neat_Hamming`)**  
   Utilizes a vector to dynamically track minimum values using comparisons. This method is optimized for both time and memory efficiency and can handle larger input sizes.

3. **Sliding Window Approach (`Solve_Neater_Hamming`)**  
   Optimized for very large numbers by implementing a sliding window technique to control memory usage. This approach reduces memory complexity and is suited for very large Hamming numbers.

## Requirements

- C++11 or later

## Compilation and Execution

To compile and run the code, use the following commands:

```bash
g++ -std=c++11 -o hamming_calculator main.cpp ./hamming_calculator

or simply 
```bash
make
./hamming_calculator 
