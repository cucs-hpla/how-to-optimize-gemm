/* Create macros so that the matrices are stored in column-major order */
#include <stdio.h>
#include <stdlib.h>
//#include </System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers/cblas.h>
#include</usr/local/opt/openblas/include/cblas.h>

/* Routine for computing C = A * B + C ,                                                                
using cblas dgemm function                                                                              
*/

void MY_MMult( int m, int n, int k, double *a, int lda,
	       double *b, int ldb,
	       double *c, int ldc )
{
  const double alpha = 1.0 ;
  const double beta = 1.0;

  cblas_dsymm( CblasColMajor, CblasLeft ,CblasUpper , m, n, alpha , a, lda, b, ldb, beta, c, ldc    );
  /*cblas_dsymm(const enum CBLAS_ORDER __Order, const enum CBLAS_SIDE __Side,
	      const enum CBLAS_UPLO __Uplo, const int __M, const int __N,
	      const double __alpha, const double *__A, const int __lda,
	      const double *__B, const int __ldb, const double __beta, double *__C,
	      const int __ldc)*/
}
