/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]


/* Routine for computing C = A * B + C */

//extern void dgemm_(char*, char*, int*, int*,int*, double*, double*, int*, double*, int*, double*, double*, int*);

extern void dsymm_(char*, char*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int*);

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
char side='l',uplo='l';
double alpha=1.0000,beta=1.0000;
dsymm_(&side,&uplo,&m,&n,&alpha,a,&lda,b,&ldb,&beta,c,&ldc);

}


  
