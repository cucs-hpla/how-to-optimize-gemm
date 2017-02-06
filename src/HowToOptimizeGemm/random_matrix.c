#include <stdlib.h>

#define A( i,j ) a[ (j)*lda + (i) ]

void random_matrix( int m, int n, double *a, int lda )
{
  double drand48();
  int i,j;

    // symmetric
    for ( j=0; j<n; j++) {
        for ( i=0; i<m; i++) {
            if (i < j) {
                A( i,j ) = A( j,i ); // upper diagonal half
            }
            else {
                A( i,j ) = 2.0 * drand48( ) - 1.0;
            }
        }
    }

   // non-symmetric
   /*for ( j=0; j<n; j++ )
      for ( i=0; i<m; i++ )
        A( i,j ) = 2.0 * drand48( ) - 1.0;
   */
}
