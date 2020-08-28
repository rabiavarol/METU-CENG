/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
 * Please fill in the following student struct
 */
team_t team = {
    "2237881",              /* Student ID */

    "RABİA VAROL",     /* full name */
    "rabia.varol@metu.edu.tr",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};

#define SUM_OPERATION(sum_dst,jdimi,dim) ({sum_dst += src[jdimi-dim-1] * -1;\
                        sum_dst += src[jdimi-dim+1];\
                        sum_dst += src[jdimi-1] * -2;\
                        sum_dst += src[jdimi+1] << 1;\
                        sum_dst += src[jdimi+dim-1] * -1;\
                        sum_dst += src[jdimi+dim+1];\
                       })

/***************
 * Sobel KERNEL
 ***************/

/******************************************************
 * Your different versions of the sobel functions  go here
 ******************************************************/

/*
 * naive_sobel - The naive baseline version of Sobel
 */
char naive_sobel_descr[] = "sobel: Naive baseline implementation";
void naive_sobel(int dim,int *src, int *dst) {
    int i,j,k,l;
    int ker[3][3] = {{-1, 0, 1},
                     {-2, 0, 2},
                     {-1, 0, 1}};

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {
	        dst[j*dim+i]=0;
            if(!((i == 0) || (i == dim-1) || (j == 0) || (j == dim-1))){
            for(k = -1; k <= 1; k++)
                for(l = -1; l <= 1; l++) {
                    dst[j*dim+i]=dst[j*dim+i]+src[(j + l)*dim+(i + k)] * ker[(l+1)][(k+1)];
                }
            }

}
}

/*
 * sobel - Your current working version of sobel
 * IMPORTANT: This is the version you will be graded on
 */

char sobel_descr[] = "Dot product: Current working version";
void sobel(int dim,int *src,int *dst)
{
     register int i,j;
   register int dim_1, sum_dst,jdim,jdimi;

   dim_1 = dim - 1;
   /*when i = 0 || i == dim_1*/
   /*when j = 0 || j == dim_1*/
   for(i = 0, j = 0; i < dim && j < dim; i++, j++){
     jdim = j*dim;
     dst[jdim] = 0;
     dst[jdim+dim_1] = 0;
     dst[i] = 0;
     dst[dim_1*dim+i] = 0;
   }

   for(j = 1; j < dim_1; j++){
       for(i = 0; i < dim_1;i++){
           jdimi = j*dim+i;
           sum_dst = 0;
           if(!(i == 0)){
               SUM_OPERATION(sum_dst,jdimi,dim);
           }
           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;
           SUM_OPERATION(sum_dst,jdimi,dim);

           dst[jdimi++] = sum_dst;
           i++;
           sum_dst = 0;

           if(!(i == dim_1)){
             SUM_OPERATION(sum_dst,jdimi,dim);
           }
           dst[jdimi] = sum_dst;

       }
   }

}


/*********************************************************************
 * register_sobel_functions - Register all of your different versions
 *     of the sobel functions  with the driver by calling the
 *     add_sobel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/
 
void register_sobel_functions() {
    add_sobel_function(&naive_sobel, naive_sobel_descr);
    add_sobel_function(&sobel, sobel_descr);
    /* ... Register additional test functions here */
}




/***************
 * MIRROR KERNEL
 ***************/

/******************************************************
 * Your different versions of the mirror func  go here
 ******************************************************/

/*
 * naive_mirror - The naive baseline version of mirror
 */
char naive_mirror_descr[] = "Naive_mirror: Naive baseline implementation";
void naive_mirror(int dim,int *src,int *dst) {

 	 int i,j;

  for(j = 0; j < dim; j++)
        for(i = 0; i < dim; i++) {
            dst[RIDX(j,i,dim)]=src[RIDX(j,dim-1-i,dim)];

        }

}


/*
 * mirror - Your current working version of mirror
 * IMPORTANT: This is the version you will be graded on
 */
char mirror_descr[] = "Mirror: Current working version";
void mirror(int dim,int *src,int *dst)
{
    register int i,j,dim_1,jdim,jdim_1;
  register int jdim2, jdim2_1;
  dim_1 = dim-1;
  //when j == 0
  for(i = 0; i < dim; i++){
    dst[i] = src[dim_1-i];
    dst[dim+i] = src[dim+dim-1-i];
  }
  for(j = 2; j < dim; j+=2){
    jdim = j*dim;
    jdim_1 = jdim+dim_1;
    jdim2 = jdim_1+1;
    jdim2_1 = jdim2+dim_1;
    for(i = 0; i < dim; i+=32) {
        //dst[j*dim+i] = src[j*dim+dim_1-i];
        //RIDX(i,j,n) ((i)*(n)+(j))
        dst[jdim+i+0]=src[jdim_1-i-0];
        dst[jdim+i+1]=src[jdim_1-i-1];
        dst[jdim+i+2]=src[jdim_1-i-2];
        dst[jdim+i+3]=src[jdim_1-i-3];
        dst[jdim+i+4]=src[jdim_1-i-4];
        dst[jdim+i+5]=src[jdim_1-i-5];
        dst[jdim+i+6]=src[jdim_1-i-6];
        dst[jdim+i+7]=src[jdim_1-i-7];
        dst[jdim+i+8]=src[jdim_1-i-8];
        dst[jdim+i+9]=src[jdim_1-i-9];
        dst[jdim+i+10]=src[jdim_1-i-10];
        dst[jdim+i+11]=src[jdim_1-i-11];
        dst[jdim+i+12]=src[jdim_1-i-12];
        dst[jdim+i+13]=src[jdim_1-i-13];
        dst[jdim+i+14]=src[jdim_1-i-14];
        dst[jdim+i+15]=src[jdim_1-i-15];
        dst[jdim+i+16]=src[jdim_1-i-16];
        dst[jdim+i+17]=src[jdim_1-i-17];
        dst[jdim+i+18]=src[jdim_1-i-18];
        dst[jdim+i+19]=src[jdim_1-i-19];
        dst[jdim+i+20]=src[jdim_1-i-20];
        dst[jdim+i+21]=src[jdim_1-i-21];
        dst[jdim+i+22]=src[jdim_1-i-22];
        dst[jdim+i+23]=src[jdim_1-i-23];
        dst[jdim+i+24]=src[jdim_1-i-24];
        dst[jdim+i+25]=src[jdim_1-i-25];
        dst[jdim+i+26]=src[jdim_1-i-26];
        dst[jdim+i+27]=src[jdim_1-i-27];
        dst[jdim+i+28]=src[jdim_1-i-28];
        dst[jdim+i+29]=src[jdim_1-i-29];
        dst[jdim+i+30]=src[jdim_1-i-30];
        dst[jdim+i+31]=src[jdim_1-i-31];
        /*******************************/
        dst[jdim2+i+0]=src[jdim2_1-i-0];
        dst[jdim2+i+1]=src[jdim2_1-i-1];
        dst[jdim2+i+2]=src[jdim2_1-i-2];
        dst[jdim2+i+3]=src[jdim2_1-i-3];
        dst[jdim2+i+4]=src[jdim2_1-i-4];
        dst[jdim2+i+5]=src[jdim2_1-i-5];
        dst[jdim2+i+6]=src[jdim2_1-i-6];
        dst[jdim2+i+7]=src[jdim2_1-i-7];
        dst[jdim2+i+8]=src[jdim2_1-i-8];
        dst[jdim2+i+9]=src[jdim2_1-i-9];
        dst[jdim2+i+10]=src[jdim2_1-i-10];
        dst[jdim2+i+11]=src[jdim2_1-i-11];
        dst[jdim2+i+12]=src[jdim2_1-i-12];
        dst[jdim2+i+13]=src[jdim2_1-i-13];
        dst[jdim2+i+14]=src[jdim2_1-i-14];
        dst[jdim2+i+15]=src[jdim2_1-i-15];
        dst[jdim2+i+16]=src[jdim2_1-i-16];
        dst[jdim2+i+17]=src[jdim2_1-i-17];
        dst[jdim2+i+18]=src[jdim2_1-i-18];
        dst[jdim2+i+19]=src[jdim2_1-i-19];
        dst[jdim2+i+20]=src[jdim2_1-i-20];
        dst[jdim2+i+21]=src[jdim2_1-i-21];
        dst[jdim2+i+22]=src[jdim2_1-i-22];
        dst[jdim2+i+23]=src[jdim2_1-i-23];
        dst[jdim2+i+24]=src[jdim2_1-i-24];
        dst[jdim2+i+25]=src[jdim2_1-i-25];
        dst[jdim2+i+26]=src[jdim2_1-i-26];
        dst[jdim2+i+27]=src[jdim2_1-i-27];
        dst[jdim2+i+28]=src[jdim2_1-i-28];
        dst[jdim2+i+29]=src[jdim2_1-i-29];
        dst[jdim2+i+30]=src[jdim2_1-i-30];
        dst[jdim2+i+31]=src[jdim2_1-i-31];

    }
  }

}


/*********************************************************************
 * register_mirror_functions - Register all of your different versions
 *     of the mirror functions  with the driver by calling the
 *     add_mirror_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_mirror_functions() {
    add_mirror_function(&naive_mirror, naive_mirror_descr);
    add_mirror_function(&mirror, mirror_descr);
    /* ... Register additional test functions here */
}
