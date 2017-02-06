First, we compare the number of instructions as well as the number of cache misses for the reference solution and the optimized version MMult_4x4_16.
The former is an unoptimized solution, while the latter incorporates a combination of loop unrolling and contiguous blocked computations to more effectively utilize vector instructions and maximizing cache use.

Number of instructions:

    REF: 52,844,057,604
    MMult_4x4_16: 22,810,897,158

Cache misses:

    REF: 7,462,969
    MMult_4x4_16: 2,734,311

As we can see, the optimized version clearly outperforms the reference solution both in terms of number of instructions and cache misses.

Next, we implement a simple function that utilizes an optimized BLAS **dgemm** routine, and compare it to MMult_4x4_16 used previously. The following figure shows the improvement that the BLAS **dgemm** routine provides over the previously optimized version.


![Figure 1](https://github.com/seblaud/how-to-optimize-gemm/blob/master/src/HowToOptimizeGemm/compare_MMult_4x4_16_MMult_dgemm.png "MMult_4x4_16 vs. BLAS **dgemm**")
