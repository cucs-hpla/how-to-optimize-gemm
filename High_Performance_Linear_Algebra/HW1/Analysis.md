###################################################################
Analysis.md<br />
Eric Peters<br />
cucs-hpla<br />
HW1:Due 2017-02-06<br />
###################################################################

For this assignment I am running everything on a Mac, and was not able to use the Linux perf<br />
tool.  Instead I used the callgrind tool by valgrind.  This isn't quite the same thing<br />
as perf because it simulates the memory usage and therefore performs quite a bit slower.  In<br /> 
order to combat the speed deficit I only ran up to the 400 matricies instead of the 800 when<br /> 
recording instructions and cache misses.  I felt it was sufficient to still see the trends<br />
that were present.<br />

I started by forking the repository.(Might have done something a little incorrect here...)<br />

proc parameters.m was altered to reflect my machine parameters, where I am running a 2.2 GHz<br />
Intel Core i7 processor.<br />

The makefile was used to run different implementations of the general matrix matrix multiply<br /> 
and can be found in this directory.<br />

###################################################################
#Question # 1 GEneral Matrix Multiply
###################################################################

##a.) Compare the number of instructions and cache-misses for the reference implementation as well as your favorite optimized implementation.

valgrind --tool=callgrind --simulate-cache=yes program-to-run program-arguments<br />
callgrind annotate --auto=yes callgrind.out.pid<br />

The following counts are for the running the entire program!<br />

Implementation   Ir          I1mr  D1mr      D1mw<br />
Reference        3930631791  5480  76690602  470138<br />
MMult1           3931817109  5322  78395436  470287<br />
MMult 1x4 3	     3930562851  5387  78599691  484896<br />
MMult 1x4 9	     2367831438  5512  116627158 639682<br />
MMult 4x4 10     2089465161  5612  53343101  512180<br />
MMult 4x4 13     2112209129  5733  39062376  623747<br />

Ir: I cache reads (Instructions executed)<br />
I1mr: I1 cache read misses (instruction wasn't in I1 cache but was in L2)<br />
D1mr: D1 cache read misses (data location not in D1 cache, but in L2)<br />
D1mw: D1 cache write misses (location not in D1 cache, but in L2)<br />

In General as we optimize the number of instructions decreases as well as the cache misses.<br />

##b.) Write an impementation that calls the dgemm function from an optimized BLAS.  You'll need to edit the makefule to link -lblas.

I ended up using the openblas implementation of blas to do the general matrix matrix multiply.<br />

my make file has the following updates or it can be found in "makefile" in the current directory<br />

###################################################################
```c
OLD  := MMult0
NEW  := MMult_BLAS
#
# sample makefile
#

CC         := clang
LINKER     := $(CC)
CFLAGS     := -O2 -Wall -msse3 
LDFLAGS    := -lm -lblas -L/usr/local/opt/openblas/lib

UTIL       := copy_matrix.o \
              compare_matrices.o \
              random_matrix.o \
              dclock.o \
              REF_MMult.o \
              print_matrix.o

TEST_OBJS  := test_MMult.o $(NEW).o 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: 
	make clean;
	make test_MMult.x

test_MMult.x: $(TEST_OBJS) $(UTIL) parameters.h
	$(LINKER) $(TEST_OBJS) $(UTIL) $(LDFLAGS) \
	$(BLAS_LIB) -o $(TEST_BIN) $@ 

run:	
	make all
	echo "version = '$(NEW)';" > output_$(NEW).m
	OMP_NUM_THREADS=4 ./test_MMult.x | tee -a output_$(NEW).m
	cp output_$(OLD).m output_old.m
	cp output_$(NEW).m output_new.m

clean:
	rm -f *.o *~ core *.x

cleanall:
	rm -f *.o *~ core *.x output*.m *.eps *.png
```
###################################################################

The version I used for MY MULT with the openblas implementation looks like the following<br />

###################################################################
```c
//Create macros so that the matrices are stored in column-major order

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/*Routine for computing C = A * B + C */

extern void dgemm (char*, char*, int*, int*,int*, double*, double*, int*, double*, int*, double*, double*, int*);

void MY MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
char transa='N',transb='N';
double alpha=1.0000,beta=1.0000;
dgemm (&transa,&transb,&m,&n,&k,&alpha,a,&lda,b,&ldb,&beta,c,&ldc);
}
```
###################################################################

##c.) Make a plot comparing your favorite implementation to the BLAS implementation<br />

The plot for this comparison can be found in this directory under the file name<br />
"compare MMult 4x4 13 MMult BLAS.png"<br />

The blas implementation is running at nearly 40 GFLOPS/sec which is nearly the peak speed of<br /> 
the processor, where as the 4x4 13 optimization implementation is running at about 12 GFLOPS/sec<br />

![Comparison of GEMM Optimizations](compare_MMult_4x4_13_MMult_BLAS.png)

Its quite amazing how fast the BLAS implementation is!  If we run the BLAS implemenation with<br />
all avaible cores using openmp we can get up to 130 GFLOPS/sec!<br />

###################################################################
#Question # 2 SYmmetric Matrix Multiply
###################################################################

##a.) Extend the test harness to perform the muliplication C += A * B where A is symmetric. 

You may assume that either the upper or lower triangular part is significant.<br />

I ended up implementing the openblas implementation of symmetric matrix multiply, which looks<br />
like the following. "MMult BLAS SYMM.c" in this directory.<br />

###################################################################
```c
/* Create macros so that the matrices are stored in column-major order */

define A(i,j) a[ (j)*lda + (i) ]
define B(i,j) b[ (j)*ldb + (i) ]
define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */
extern void dsymm_(char*, char*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int*);

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
char side='l',uplo='u';
double alpha=1.0000,beta=1.0000;
dsymm_(&side,&uplo,&m,&n,&alpha,a,&lda,b,&ldb,&beta,c,&ldc);
}
```
###################################################################

As it turns out openblas's implementation speeds up the procedure minamally or <br />
if none at all, but it is difficult to know how many GFLOPS/sec because I don't know<br /> 
exactly how many FLOPS are being done?  As for the instructions issued and cache <br />
misses we get the following.<br />

Implementation	Ir		I1mr	D1mr		D1mw<br />
MMULT BLAS        1516723512  11198 33684261    1070239<br />
MMULT BLAS SYMM   1276902410  10673 33677093    1070972<br />

It seems to me that the symmetric matrix matrix multiply is able to handle memory and <br />
instructions better, even though it may be more complicated to implement.  Even saying so <br />
it looks to be that everything has been heavily optimized so that everything within <br />
openblas runs near peak performance.<br />

##b.) What is good and bad about this implementation relative to GEMM?

What is good about this sort of implementation is that we only have to store half of the matrix<br />
and the amount of FLOPS is cut in half.  However it is bad when trying to optimize instructions<br />
and cache misses because of the blocking strategies that have to be used are complicated, as<br />
the blocks are variable depending on where your at in the matrix if your only storing half <br />
of it.<br />

Note: That as you are using a block from symmetric matrix A you can multiply it by two parts in <br />
general matrix B while you have it in cache, however this will make access in general matrix <br />
C a little bit more tricky!<br />

Therefore you have an advantage in speed from doing more with matrix A, but then you loose it when<br />
 you have to access C.  So in general it still ends up being the same speed as GEMM with blas.<br />





