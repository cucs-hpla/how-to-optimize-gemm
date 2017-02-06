#include "cblas.h"

/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
   cblas_dgemm(CblasColMajor,CblasNoTrans,CblasNoTrans,m,n,k,1,a,lda,b,ldb,1,c,ldc);
}


  
