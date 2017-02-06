#include <stdlib.h>

#define A( i,j ) a[ (j)*lda + (i) ]

void sym_matrix( int m, int n, double *a, int lda )
{
  double drand48();
  int i,j;

  for ( j=0; j<n; j++ )
    for ( i=0; i<=j; i++ ){
      A( i,j ) =  2.0 * drand48( ) - 1.0;
      A(j,i) = A(i,j);
    }
}
