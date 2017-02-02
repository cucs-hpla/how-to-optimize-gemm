Kylee Budai Analysis:

Reference (MMult0)

Pseudo-Optimized Version (MMult_1x4_9)
   31.36% of the instructions in AddDot1x4, 64.77% in Ref 
   30.88% of cache misses in AddDot1x4, 66.37% in Ref
   35.51% of cache references in AddDot1x4, 63.2% in Ref
   45.31% of L1 cache load misses in AddDot1x4, 54.45% in Ref
   43.15% of L1 cache loads in AddDot1x4, 54.71% in Ref

Looking at the percent of cache misses per references, in this pseudo optimized version, 86% of cache references lead to cache misses while in the reference version, 95% of the cache loads lead to cache misses.  This is not hugely significant.  The most significant difference in these two versions is the amount of instructions.  There are about a third of the amount of instructions and therefore about a third of the number of cache misses are a result of the pseudo optimized version.  The L1 cache loads/misses show about the same thing.

Optimized Version (MMult_4x4_10)
   34.25% of the instructions in AddDot4x4, 61.80% in Ref
   22.85% of cache misses in AddDot4x4, 74.27% in Ref
   28.47% of cache references in AddDot4x4, 69.67% in Ref
   21.53% of L1 cache load misses in AddDot4x4, 78.13% in Ref
   43.48% of L1 cache loads in AddDot4x4, 53,68% in Ref

Cache misses/cache reference: 80% of cache references in optimized lead to cache miss, for reference, 92%.  This difference is significant because this version does not reduct the number of instructions much more than the psuedo-optimized version but the cache misses are significantly reduced as a percentage of cache misses.  


BLAS Function Dgemm: dgemm significantly reduces the amount of instructions (to fewer than 40%- can't tell exactly because each built in function uses a very small percentage of total instruction base) and the cache misses to less than 10%, leaving about 90% of the instructions being done in the reference version.  The cache references account for about 20% of the cache references.  Using BLAS optimises GEMM not only by reducing the number of instructions needed but also by significantly improving the use of cache.


SYMMETRIC MATRIX MULTIPLICATION:

