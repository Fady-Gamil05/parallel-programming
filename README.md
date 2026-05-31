# Parallel Computing – Matrix-Vector Multiplication 

Parallel C implementation of matrix-vector multiplication and dot product 
using POSIX Pthreads, developed as part of COMP2860 at the University of Leeds.

## What it Does
- Multiplies an N×N matrix M by an N-vector u to produce vector v (Mu → v)
- Computes the scalar (dot) product of v with itself
- Both operations performed primarily in parallel using Pthreads

## Tech Stack
C · Pthreads · GCC · Linux

## How to Run
1. Compile:
   gcc -o portfolioExercise portfolioExercise.c -lpthread
2. Run with problem size N and number of threads p:
   ./portfolioExercise 1000 4

## Performance
See readme.txt for timing results and parallel speed-up across 1, 2 and 4 threads.
