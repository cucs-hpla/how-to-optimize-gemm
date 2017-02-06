Name: Jorge Luis Barrera Cruz
Date: 02/06/2017

--------------
--------------
HOMEWORK # 01
--------------
--------------

NOTE: The results computed for this homework assignment were computed in two computers / three different operating systems. First, I used Instruments in OSX since neither valgrind nor perf are compatible with macOS Sierra. Since I could not get the cache events to work on it (only the time performance summary that contains information similar to perf stat), I switched to a linux machine in my lab. In this second machine I did not have root access and the performance tools available were limited; so, despite being able to play a little bit with valgrind (cachegrind), I decided to create a bootable Ubuntu usb flash drive from my mac. I am not sure about the implications of running the codes using this configuration, and I would appreciate your feedback on this.

----------
PERF TOOLS
----------

Summary shown below:

Performace between 4-6, error ~ 10^-14.

*** Performance counter stats for './test_MMult.x':

      12877.640670      task-clock (msec)         #    1.000 CPUs utilized          
                16      context-switches          #    0.001 K/sec                  
                 0      cpu-migrations            #    0.000 K/sec                  
            39,821      page-faults               #    0.003 M/sec                  
    36,238,582,487      cycles                    #    2.814 GHz                    
   <not supported>      stalled-cycles-frontend  
   <not supported>      stalled-cycles-backend   
    49,666,741,594      instructions              #    1.37  insns per cycle        
     7,101,026,477      branches                  #  551.423 M/sec                  
        12,050,585      branch-misses             #    0.17% of all branches        

      12.877439766 seconds time elapsed

*** Samples: 22K of event 'L1-dcache-loads', Event count (approx.): 9161455688
Overhead  Command       Shared Object            Symbol                                  
  50.41%  test_MMult.x  test_MMult.x             [.] REF_MMult  (most on add) 
  44.33%  test_MMult.x  test_MMult.x             [.] AddDot4x4  (mixed moves and adds) 
   0.94%  test_MMult.x  libc-2.23.so             [.] __erand48_r                         
   0.63%  test_MMult.x  libc-2.23.so             [.] __drand48_iterate                   
   0.57%  test_MMult.x  test_MMult.x             [.] random_matrix 

*** Samples: 21K of event 'L1-dcache-load-misses', Event count (approx.): 3012666242
Overhead  Command       Shared Object            Symbol
  75.23%  test_MMult.x  test_MMult.x             [.] REF_MMult (most on add)
  24.03%  test_MMult.x  test_MMult.x             [.] AddDot4x4 (most on moves)

*** Samples: 23K of event 'L1-dcache-stores', Event count (approx.): 4317031888
Overhead  Command       Shared Object            Symbol
  53.42%  test_MMult.x  test_MMult.x             [.] REF_MMult (most on add) 
  40.51%  test_MMult.x  test_MMult.x             [.] AddDot4x4 (mixed moves and adds)
   1.57%  test_MMult.x  libc-2.23.so             [.] __erand48_r
   0.78%  test_MMult.x  libc-2.23.so             [.] __drand48_iterate

*** Samples: 22K of event 'cycles:pp', Event count (approx.): 16967748209
  Children      Self  Command       Shared Object     Symbol                             
+   69.66%    69.65%  test_MMult.x  test_MMult.x      [.] REF_MMult (fmadd to mov, 4:1)
+   27.54%    27.52%  test_MMult.x  test_MMult.x      [.] AddDot4x4 (mixed moves and adds)
+    0.95%     0.00%  test_MMult.x  [unknown]         [.] 0x0001000b00000000             
+    0.68%     0.29%  test_MMult.x  test_MMult.x      [.] random_matrix                  
+    0.58%     0.58%  test_MMult.x  libc-2.23.so      [.] __erand48_r                    
+    0.54%     0.00%  test_MMult.x  [unknown]         [.] 0xffffffff8106b7c2             
+    0.54%     0.00%  test_MMult.x  [unknown]         [.] 0xffffffff8182fcb8             
+    0.54%     0.00%  test_MMult.x  test_MMult.x      [.] main                           
+    0.51%     0.00%  test_MMult.x  [unknown]         [.] 0xffffffff8106b537             
+    0.50%     0.24%  test_MMult.x  test_MMult.x      [.] copy_matrix

- Verified that perf report children column does not tell us how much faster is one implementation with respect to the other because it only counts the instructions, and not any installs.

- By using the annotation tool, the most expensive parts of the computations were also checked. More time in movement of data was observed in lower performance algorithms. Also, a considerable amount of loads and stores made the les optimal alternatives more expensive.

- By checking the stats of MMult_4x4_9.c, we found that there wasn't just one hot spot to optimize, but a lot of mildly expensive actions.

------
DGEMM
------

*** Performance counter stats for 'make run':

       7109.567207      task-clock (msec)         #    1.000 CPUs utilized          
               140      context-switches          #    0.020 K/sec                  
                95      cpu-migrations            #    0.013 K/sec                  
            65,039      page-faults               #    0.009 M/sec                  
    21,982,602,617      cycles                    #    3.092 GHz                    
   <not supported>      stalled-cycles-frontend  
   <not supported>      stalled-cycles-backend   
    50,476,666,340      instructions              #    2.30  insns per cycle        
     7,272,905,970      branches                  # 1022.975 M/sec                  
        17,309,041      branch-misses             #    0.24% of all branches  

	
*** Samples: 29K of event 'L1-dcache-loads', Event count (approx.): 14338807133
Overhead  Command       Shared Object            Symbol
  64.57%  test_MMult.x  libblas.so.3.6.0         [.] dgemm_
  32.24%  test_MMult.x  test_MMult.x             [.] REF_MMult
   0.64%  test_MMult.x  libc-2.23.so             [.] __erand48_r
...

In dgemm: 
 22.62 │4b0:   movsd  0x8(%r8,%rax,8),%xmm0                                              ▒
  2.54 │       mulsd  %xmm1,%xmm0                                                        ▒
 35.09 │       addsd  0x8(%rdi,%rax,8),%xmm0                                             ▒
 10.43 │       movsd  %xmm0,0x8(%rdi,%rax,8)                                             ◆
 28.16 │       add    $0x1,%rax  
In reference:
 99.15 │      add    $0x1,%rax


*** Samples: 27K of event 'L1-dcache-load-misses', Event count (approx.): 2893260463
Overhead  Command       Shared Object            Symbol                                  
  78.39%  test_MMult.x  test_MMult.x             [.] REF_MMult ( mostly adds) 
  20.89%  test_MMult.x  libblas.so.3.6.0         [.] dgemm_  (mov to add ratio, 4:1)


*** Samples: 29K of event 'L1-dcache-stores', Event count (approx.): 7180234897
Overhead  Command       Shared Object            Symbol
  64.40%  test_MMult.x  libblas.so.3.6.0         [.] dgemm_  (add to move ratio, ~2:1)
  32.17%  test_MMult.x  test_MMult.x             [.] REF_MMult (mostly add)
   0.86%  test_MMult.x  libc-2.23.so             [.] __erand48_r
   0.50%  test_MMult.x  libc-2.23.so             [.] __drand48_iterate

*** Samples: 29K of event 'cycles:pp', Event count (approx.): 22282177172
  Children      Self  Command       Shared Object            Symbol   
+   52.99%    52.98%  test_MMult.x  test_MMult.x             [.] REF_MMult (add to mov, 4:1)
+   41.73%    41.71%  test_MMult.x  libblas.so.3.6.0         [.] dgemm_    (mult,add to mov, 4:3)
+    0.72%     0.00%  test_MMult.x  [unknown]                [.] 0x0001000b00000000   


- Increase in data locality is reflected in an improvement on performance.

- The performance of cblas_dgemm was not significantly higher, contrary to what was expected. 

- Plot comparing one of the given implementations with cblas_dgemm can be found in compare_MMult_4x4_9_MMult_DGEMM.png

------
DSYMM
------

- In this case we only have to access half of A (the symmetric matrix), which save a significant amount of memory. In terms of performance, cblas_dgemm and cblas_dsymm implementations perform similarly with cblas_dsymm being slightly better than cblas_dgemm. 

- If A is symmetric positive definite, we could do A = LL^T, where L is the unique lower triangular matrix (Cholesky factorization). This has the advantage that the factorization takes the same amount of space as the original matrix, which is n(n + 1)/2 elements. 
