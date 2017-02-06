#include <stdio.h>
// #include <malloc.h>
#include <stdlib.h>

#include <cblas.h>

#define min(x,y) (((x) < (y)) ? (x) : (y))

//extern “C” void sgemm_( char *, char *, int *, int *, int * float *, float *, int *, float *, int *, float *, float *, int * );


int main()
{

printf( "hello, There\n" );

int i=0;
  double A[6] = {1.0,2.0,1.0,-3.0,4.0,-1.0};         
  double B[6] = {1.0,2.0,1.0,-3.0,4.0,-1.0};  
  double C[9] = {.5,.5,.5,.5,.5,.5,.5,.5,.5}; 
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,3,3,2,1,A, 3, B, 3,10,C,3);

  for(i=0; i<9; i++)
    printf("%lf ", C[i]);
  printf("\n");

}
