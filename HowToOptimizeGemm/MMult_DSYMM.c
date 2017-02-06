#include <cblas.h>

/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{

    double alpha=1.0, beta=0.0;
    cblas_dsymm(CblasRowMajor, CblasLeft, CblasUpper,
                m, n,
                alpha, a, lda, b, ldb, 
		beta, c, ldc);
}


  
