/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j, p;

  for ( i=0; i<m; i++ ){        /* Loop over the rows of C */
    for ( j=0; j<n; j++ ){        /* Loop over the columns of C */
      p = 0;
      while ( p <=i ){        /* Update C( i,j ) with the inner
				       product of the ith row of A and				       
					the jth column of B */
	C( i,j ) = C( i,j ) +  A( i,p ) * B( p,j );  //assuming upper triangular part is significant
        C( p,j ) = C( p,j ) +  A( i,p ) * B( i,j );  // This B and C call will cause more cache misses than holding A... so not really optimized
//change so that each time a chunk of A gets pulled, it the corresponding matrix multiplication
      p++;                                          //on both sides of the axis of symmetry. This means the C and B must be updated to correspond to the values being multiplied to copute C. 
      }
    }
  }
}


  
