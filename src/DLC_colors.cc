#include "../include/DLC_colors.h"

#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_math.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>


void DLC::Michel_levy()
{
	const int len = 471; // data length defined by cie data
	const int dlen = 100; // number of points in thickness
	const int dstart = 100;
	const int dend = 200;

 	double dstep, X, Y, Z, Norm, DeltaComp, DeltaSamp;
	const double Dn = 0.11;
	const double lambda0 = 528;
	const int m = 4;

	dstep = (dend-dstart)/dlen;

	gsl_matrix * A = gsl_matrix_alloc (1,2);
	gsl_matrix * P = gsl_matrix_alloc (2,2);

	gsl_matrix_set_zero(A);
	gsl_matrix_set_zero(P);

	gsl_matrix_set(A,0,1,1);
	gsl_matrix_set(P,0,0,1);	

	gsl_vector * d = gsl_vector_alloc (dlen);
	gsl_vector * lambda = gsl_vector_alloc (len);

	// gsl_matrix * rgb = gsl_matrix_alloc (1,dlen,3); cant do this

	for(int i=0;i<dlen;i++)
	{
		gsl_vector_set(d,i,dstart+i*dstep);	
	}

	for(int i=0;i<len;i++)
	{
		gsl_vector_set(lambda,i,360+i);
	}

	//for(int i=0;i<len;i++)
	//{
	//	printf("%g\n",gsl_vector_get(lambda,i));	
	//}


	// load cie data
  	gsl_vector * xm = gsl_vector_alloc (len);
	gsl_vector * ym = gsl_vector_alloc (len);
	gsl_vector * zm = gsl_vector_alloc (len);
  
	FILE * fxm = fopen("xm.txt", "r");
	FILE * fym = fopen("ym.txt", "r");
	FILE * fzm = fopen("zm.txt", "r");

	gsl_vector_fscanf(fxm,xm);
	gsl_vector_fscanf(fym,ym);
	gsl_vector_fscanf(fzm,zm);

	fclose(fxm);
	fclose(fym);
	fclose(fzm);


	for (int dc=0;dc<dlen;dc++)
	{
		for (int l=0;l<len;l++)	
		{
			DeltaComp = (m*M_PI*lambda0)/gsl_vector_get(lambda,l);
			DeltaSamp = 2*M_PI*Dn*gsl_vector_get(d,dc)/gsl_vector_get(lambda,l); 

			
		}

		
	}

	
	

	//trapezium method - step is 1nm
	X = gsl_blas_dasum(xm)-(gsl_vector_get(xm,len-1))/2-(gsl_vector_get(xm,0))/2;
	Y = gsl_blas_dasum(ym)-(gsl_vector_get(ym,len-1))/2-(gsl_vector_get(ym,0))/2;
	Z = gsl_blas_dasum(zm)-(gsl_vector_get(zm,len-1))/2-(gsl_vector_get(zm,0))/2;
	
	//free vector memory
	gsl_vector_free (xm);
	gsl_vector_free (ym);
	gsl_vector_free (zm);

	gsl_matrix_free (A);
	gsl_matrix_free (P);
	
	Norm = X+Y+Z;
	
	printf("%g\n",Norm);

} 
