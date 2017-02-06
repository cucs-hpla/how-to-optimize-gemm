The results contained in this file concern the solution of `C += A * B` for matrices `A`, `B`, and `C`.
First, we compare the number of instructions as well as the number of cache misses for the reference solution and the optimized version MMult_4x4_16.
The former is an unoptimized solution, while the latter incorporates a combination of loop unrolling and contiguous blocked computations to more effectively utilize vector instructions and maximize cache use. We use the `perf` tool to gather the needed information (`make && perf stat -e instructions,cache-misses ./test_MMult.x`):

Number of instructions:

    REF: 52,844,057,604
    MMult_4x4_16: 22,810,897,158

Cache misses:

    REF: 7,462,969
    MMult_4x4_16: 2,734,311

As we can see, the optimized version clearly outperforms the reference solution both in terms of number of instructions and cache misses.

Next, we implement a simple function that utilizes an optimized BLAS **dgemm** routine, and compare it to MMult_4x4_16 used previously. The following figure shows the improvement that the BLAS **dgemm** routine provides over the previously optimized version.


![Figure 1](https://github.com/seblaud/how-to-optimize-gemm/blob/master/src/HowToOptimizeGemm/compare_MMult_4x4_16_MMult_dgemm.png "MMult_4x4_16 vs. BLAS **dgemm**")

We can see that the BLAS **dgemm** routine performs better than our previously optimized routine. There are some interesting points in this plot, namely that there are some dips in the performance of **dgemm** when the matrix size is around 400 and 500, though it is still superior to the previously optimized solution.

Now we look at the case where `A` is symmetric, so we modify the random matrix function to produce a symmetric matrix `A`. Then, we modify the reference solution to take advantage of the symmetry, and we also create a function to call the optimized BLAS routine **dsymm**, which computes `C += A * B` for symmetric `A`. The following figure shows the comparison between **dgemm** and **dsymm**:

![Figure 2](https://github.com/seblaud/how-to-optimize-gemm/blob/master/src/HowToOptimizeGemm/compare_MMult_dgemm_MMult_dsymm.png "**dgemm** vs. **dsymm**")

As the figure suggests, the performance of **dgemm** and **dsymm** is quite comparable. The good thing about symmetric matrices is that we need to store less information to encode the entire matrix, but the bad thing is that (in general) the performance of computing `C += A * B` for dense symmetric matrices is no better than for dense non-symmetric matrices (when using optimized BLAS routines). We also do still see the similar dips in performance for **dsymm** (when `m = n = k = 400` or `500`), but they seem to be less severe and less frequent than for **dgemm**.
