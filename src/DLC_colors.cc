#include "../include/DLC_colors.h"

void DLC::Michel_levy()
{
	const int len = 471; // data length defined by cie data
	const int dlen = 100; // number of points in thickness
	const int dstart = 100;
	const int dend = 200;

 	double dstep, X, Y, Z, Norm, DeltaComp, DeltaSamp, cDC2, sDC2, cDS2, sDS2, crho, srho, Ts;
	const double Dn = 0.11;
	const double lambda0 = 528;
	const int m = 4;

	const double rho = M_PI/4;

	dstep = (dend-dstart)/dlen;

	gsl_matrix * A = gsl_matrix_alloc (1,2);
	gsl_matrix * P = gsl_matrix_alloc (2,2);

	gsl_matrix_complex * Comp = gsl_matrix_complex_alloc (2,2);
	gsl_matrix_complex * Sample = gsl_matrix_complex_alloc (2,2);
	gsl_matrix_complex * Reflect = gsl_matrix_complex_alloc (2,2);

	gsl_complex comp_c[2];
	gsl_complex sample_c[4];
	gsl_complex refl_c[4];

	gsl_matrix_set_zero(A);
	gsl_matrix_set_zero(P);

	gsl_matrix_set(A,0,1,1); // only non-zero elements
	gsl_matrix_set(P,0,0,1);	

	gsl_vector * d = gsl_vector_alloc (dlen);
	gsl_vector * lambda = gsl_vector_alloc (len);
	gsl_vector * T = gsl_vector_alloc (len);

	gsl_vector * Xr = gsl_vector_alloc (len);
	gsl_vector * Yr = gsl_vector_alloc (len);
	gsl_vector * Zr = gsl_vector_alloc (len);

	gsl_vector_complex * E = gsl_vector_complex_alloc (2);
	

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
			
			/*Comp = [cos(DeltaComp/2),1i*sin(DeltaComp/2);1i*sin(DeltaComp/2),cos(DeltaComp/2)];
            Sample = [exp(1i*(DeltaSamp/2))*cos(rho(r))^2+exp(1i*(-DeltaSamp/2))*sin(rho(r))^2,2*1i*sin(rho(r))*cos(rho(r))*sin(DeltaSamp/2);2*1i*sin(rho(r))*cos(rho(r))*sin(DeltaSamp/2),exp(1i*(-DeltaSamp/2))*cos(rho(r))^2+exp(1i*(DeltaSamp/2))*sin(rho(r))^2];
            Reflec = [-exp(1i*pi),0;0,-exp(1i*pi)];*/

			cDC2 = cos(DeltaComp/2);
			sDC2 = sin(DeltaComp/2);
		
			cDS2 = cos(DeltaSamp/2);
			sDS2 = sin(DeltaSamp/2);

			crho = cos(rho);
			srho = sin(rho);

			comp_c[0] = gsl_complex_rect(cDC2,0);
			comp_c[1] = gsl_complex_rect(0,sDC2);
			
			gsl_matrix_complex_set(Comp,0,0,comp_c[0]);
			gsl_matrix_complex_set(Comp,0,1,comp_c[1]);
			gsl_matrix_complex_set(Comp,1,0,comp_c[1]);
			gsl_matrix_complex_set(Comp,1,1,comp_c[0]);

			sample_c[0] = gsl_complex_rect(cDS2,(1-2*pow(srho,2))*sDS2);
			sample_c[1] = gsl_complex_rect(0,2*crho*srho*sDS2);
			sample_c[2] = gsl_complex_rect(0,2*crho*srho*sDS2);
			sample_c[3] = gsl_complex_rect(cDS2,(1-2*pow(crho,2))*sDS2);

			gsl_matrix_complex_set(Sample,0,0,sample_c[0]);
			gsl_matrix_complex_set(Sample,0,1,sample_c[1]);
			gsl_matrix_complex_set(Sample,1,0,sample_c[2]);
			gsl_matrix_complex_set(Sample,1,1,sample_c[3]);

			//matrix manipluations next gsl cblas
			//%E = P*Sample*A;
            		//	E = P*Sample*Reflec*Sample*A;   
            		//	Ts = E'*E;  careful with complex comps in C++
            		//	T(l) = Ts; % generate the spectra 

			
			Ts=0.0; 
			gsl_vector_set(T,l,Ts);	
		}

		//trapezium method - step is 1nm
		X = gsl_blas_dasum(xm)-(gsl_vector_get(xm,len-1))/2-(gsl_vector_get(xm,0))/2;
		Y = gsl_blas_dasum(ym)-(gsl_vector_get(ym,len-1))/2-(gsl_vector_get(ym,0))/2;
		Z = gsl_blas_dasum(zm)-(gsl_vector_get(zm,len-1))/2-(gsl_vector_get(zm,0))/2;

		
	}

	
	
	

	
	
	//free vector memory
	gsl_vector_free (xm);
	gsl_vector_free (ym);
	gsl_vector_free (zm);

	gsl_matrix_free (A);
	gsl_matrix_free (P);

	gsl_matrix_complex_free (Comp);
	gsl_matrix_complex_free (Sample);
	gsl_matrix_complex_free (Reflect);
	
	Norm = X+Y+Z;
	
	//printf("%g\n",GSL_REAL(comp_c[0]));

} 
