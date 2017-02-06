#include "cblas.h"

/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */
void cblas_dsymm(const CBLAS_LAYOUT layout, const CBLAS_SIDE Side,
                        const CBLAS_UPLO Uplo, const int M, const int N,
                        const double alpha, const double  *A, const int lda,
                        const double  *B, const int ldb, const double beta,
                        double  *C, const int ldc);

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
   cblas_dsymm(CblasColMajor, CblasLeft, CblasLower,m,n,1,a,lda,b,ldb,1,c,ldc);
}


  
