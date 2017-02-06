# GEMM Analysis

| Event                 | Reference      | MMult_4x4_16p  |
| --------------------- | -------------- | -------------- |
| Instructions          | 22,663,956,746 | 17,932,254,700 |
| L1-dcache-load-misses | 2,852,196,001  | 187,023,411    |

Both implementations issue close to the same number of instructions. This is
surprising as I would expect the optimized implementation to issue fewer
instruction because it uses more compact instructions (SSE instructions).

The difference in L1 cache misses is more in line with what I was expecting.
The optimized implementation blocks the input so that there is better locality.
With better locality, I expect fewer L1 cache misses.

### Optimized Portable Implementation vs BLAS

![](figures/compare_MMult_4x4_16p_MMult_blas.png)

### Symmetric Matrix Multiply

![](figures/compare_MMult0_MMult0_symm.png)

On a naive implementation of matrix multiplication (MMult0), assuming the
problem is symmetric worsens performance. Cache hit percentages decrease
because to different parts of B are loaded for each entry in A. This parts
could be close together, but they could also be very far apart. On the other
hand this approach can save approximately half the memory used to store a
non-symmetric A.
