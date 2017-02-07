HW1: How to optimize gemm
Analysis of results

Comments on proc_parameters:
a) Number flops per cycle: I was not 100% sure about this one. I am running Intel(R) Core(TM) i5-5250U CPU @ 1.60GHz, with 2.7 GHz turbo (Broadwell), and according to specs, I have "16 DP FLOPs/cycle: two 4-wide FMA instructions 32 SP FLOPs/cycle: two 8-wide FMA instructions". Since my dgemm results require that the y-axis go up to 48 GFLOPs/sec ** I went ahead and selected num_flops_per_cycle =32. **Also, these results do seem a little strange, as the performance does not level out with increasing 'm=n=k' as in the reference examples. This will show up in #2.

Comments on performance tools:
I am using cachegrind (valgrind), and looking at the data cache reads and misses. In cachegrind they separate L1 and LL, where LL = L2+L3. Additionally, memory performance is compared for m<= 400. Also, somehow unable to correctly label dgemm and dsymm functions, thus doing analysis with perf tools on ubuntu desktop.

Implementations discussed:
Mult0/Ref: This is the standard triple loop that loops over all elements, and performs by element multiplication. I expect a lot of L1 cache misses.
Mult2: This has two loops, where the inner loop updates four elements of C - 4 additions of dot products to complete. Because of this I expect to see an improvement in L1 cache misses over the reference solution.
MMult_4x4_15: To update block of C, goes through row blocks of A and column blocks of B (takes 4 element subsection of blocks). For AddDot, using _mm_load_pd, which loads 2 packed doubles into one memory address.
MMult_4x4_17 - Method similar to MMult_4x4_15, but uses omp to loop through sets of 4 elements. For AddDot, using _mm256_loadu_pd and _mm256_storeu_pd, which load and store 4 packed doubles into one address. I expect improved GFLOPs/sec performance compared to MMult_4x4_15, as well as increased L1 cache performance expected. Additionally, for this method we use OMP_NUM_THREADS:=4.
MMult_dgemm: Uses cblas library cblas_dgemm.
MMult_dsymm: Uses cblas library cblas_dsymm. 

1. Compare two implementations
i) (MMult0 ,MMult2) For the first several optimization techniques we see little improvement in GFLOPS/sec. See compare_MMult0_MMult2.png for a plot comparison. In both these versions we see 0.7-0.9 GFLOPs/sec. Both report zero error.
Using perf tools for analysis, it seems that MMult2 requires more instructions, more L1 cache loads, and has more L1 cache misses than the reference - for all three MMult2 reports about 65%, while REF_MMult reports about 33%. I would have expected MMult2 to have fewer L1 cache misses than the REF_MMult.
 

ii) (MMult0 ,MMult_4x4_15) We see significantly improved optimization when comparing these two. For MMult0 we see about 0.7-0.9 GFLOPs/sec, while in MMult_4x4_15 we see about 4 GFLOPs/sec. This is about a 5x increase. For a plot comparison see compare_MMult0_MMult_4x4_15.png. When comparing MMult_4x4_15 data cache results to the reference solution, 
Using perf tools for analysis, most of the instruction is required by REF_MMult (60%), versus MMult_4x4_15 (AddDot4x4, 35%).In terms of L1 cache loads REF_MMult requires 50%, while MMult_4x4_15(AddDot) requires 45%. For L1 cache misses we see significant gains for MMult_4x4_15: REF_MMult takes 93% of the misses, while MMult_4x4_15(AddDot) accounts for 5%. Both have similar L1 cache stores ~48%. It was expected that the L1 cache misses would be greatly improved, as using _mm_load_pd, which load and store 2 packed doubles into one address.


iii) (MMult_4x4_15 ,MMult_4x4_17) In terms of GFLOPs/sec, we don't see a significant performance improvement until we go beyond m=300; in fact for m<300 MMult_4x4_15 shows better GFLOPs/sec. The primary difference of these two methods corresponds to omp and using 2 vs 4 packed doubles. I imagine for smaller matrices we don't gain as much because of the fork/join process - perhaps this is slowing down the process relative the the gains we obtain. See compare_MMult_4x4_15_MMult_4x4_17.png for GFLOPs/sec plot comparison. For m>300, MMult_4x4_15 stays around 4 GFLOPs/sec, and MMult_4x4_17 shows between 4 and 10 GFLOPs/sec.

As I was unable to use perf tools for MMult_4x4_17 (compiling issue on ubuntu), I have the following from cachegrind. These results seem wrong compared to the performance results I was able to obtain for MMult_4x4_15. I would expect even better performance from MMult_4x4_17 since it uses _mm256 and omp:
I find that REF_MMult takes 60% of the data cache reads, 66% of L1 misses, and 15% LL misses. In comparison MMult_4x4_17 takes 37% of reads, 30% of the L1 misses, and 25% of the LL misses (this is adding up the percentages from inner kernel, addDot,emmintrin.h:AddDot, and packedB). From these, the addDot functions require an added 45% of reads, 30% of L1 misses, and 60% of LL misses.


2. Compare favorite implementation with dgemm 
i) (MMult_4x4_17, MMult_dgemm)
See compare_MMult_4x4_17_MMult_dgemm.png for GFLOPs/sec plot comparison. MMult_4x4_17 improves as m increases, however, it stays mainly between 4 and 10 GFLOPs/sec. On the other hand, MMult_dgemm has a significantly better performance. After ~m-300, we see 40-50 GFLOPs/sec. In terms of error, MMult_4x4_17 prints out zero error, while MMult_dgemm prints out ~1e-15 error. 

Using perf tools on my desktop, we see that 58% of the instructions are done on MMult_dgemm, while 40% of them are done on the reference code. I see that most of the L1 cache loads are completed by MMult_dgemm (65%), while the next largest number of L1 cache loads are completed by the reference code (35%). However, the results are flipped when looking at L1 cache misses - 60% of them are by the reference code, while 38% are done by MMult_dgemm. This indicates that MMult_dgemm is much more efficient at using the L1 cache than the reference solution.   

3. Symmetric matrix - compare reference with dsymm 
(MMult0, MMult_dsymm)
The GFLOPs/sec performance of using dsymm is similar to that of dgemm. From m=40 to m=300 the value of GFLOPs/sec is steadily increasing, with values between 1 and 40. After m=300, we see GFLOPs/sec in the 30-50 GFLOPs/sec range. See compare_MMult0_MMult_symm.png.

Using perf tools on my desktop, we see that 51% of the instructions are done on MMult_dsymm, while 46% of them are done on the reference code. The increase of instruction for the reference code makes sense, as we are now dealing with a symmetric matrix. I see that most of the L1 cache loads are completed by  MMult_dsymm (58%), while the next largest number of L1 cache loads are completed by the reference code (39%). However, the results are flipped when looking at L1 cache misses - 75% of them are by the reference code, while 24% are done by MMult_dsymm. This indicates that MMult_dsymm is much more efficient at using the L1 cache than the reference solution. In terms of L1 stores, they both report 48%.   

Compared to using dgemm, the dsymm implementation: Relative to the reference code, using dsymm requires fewer instructions, fewer L1 loads, and has fewer L1 misses. I would have imagined more L1 cache misses to occur when using dsymm, as the access patterns of A and B are not aligned. 



