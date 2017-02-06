# HPLA Homework 1
Adam Heaton
Feb-5-2017

# Machine Parameters
1. Number of Processors   - 2
2. Number of Flops/cycle  - 8
3. Processor GHz          - 3.3

## Part(1): Comparing number of instructions and cache misses
Unfortunately my computer's architecture doesn't has no hardware counters so no (perf) events are available regarding the cache. Also, the school computers don't allow access to a terminal so there's no way for me to use perf on those computers. However I did compare the number of GFlops for a naive Matrix Multiplication against the an optimized one. In this case a 4x4 block matrix Multiplication

![alt text](https://github.com/MonumentToAllYourSins/how-to-optimize-gemm/blob/master/figures/MMult0vsMMult4.png)

As we can see, there isn't much of a performance gain when going from a naive Matrix Multiplication to one that performs in 4x4 blocks.

## Part(2): Comparing a naive implementation with a BLAS General MM implementation

![alt text](https://github.com/MonumentToAllYourSins/how-to-optimize-gemm/blob/master/figures/BLASvsMMult0.png)

In this case theres a huge performance gain when we use the BLAS implementation. This makes sense because BLAS GEMM does A LOT of blocking and splitting, and since the data become very parallel, this allows for loop exchange and just faster performance in general.

## Part(3): Comparing a BLAS General MM with a BLAS Symmetric MM

![alt text](https://github.com/MonumentToAllYourSins/how-to-optimize-gemm/blob/master/figures/BLASsymmVSBLASgemm.png)

In this case we can expect that the Symmetric implementation should be more optimized than that of the General implementation. This is because the Symmetric Implementation works like the General one, but since the matrix is symmetric you know the properties of the entire matrix by only looking at half of it, and even less calculations have to be to calculate the result.
