#include "../include/DLC_colors.h"

void DLC::Michel_levy(double Dn, int dlen, int dstart, int dend, gsl_matrix * Xmat, gsl_matrix * Ymat, gsl_matrix * Zmat)
{
	const int len = 471; // data length defined by cie data
	//const int dlen = 1000; // number of points in thickness
	//const int dstart = 0;
	//const int dend = 15000;
	const int ypix=100;
	int yp;

 	double dstep, X, Y, Z, Xn, Yn, Zn, Norm, DeltaComp, DeltaSamp, cDC2, sDC2, cDS2, sDS2, crho, srho, Ts, xmi, ymi, zmi, R, G, B;
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

	gsl_vector * Xr = gsl_vector_alloc (len);
	gsl_vector * Yr = gsl_vector_alloc (len);
	gsl_vector * Zr = gsl_vector_alloc (len);

	gsl_vector_complex * E = gsl_vector_complex_alloc (2);

	gsl_vector * XYZ = gsl_vector_alloc (3);
	gsl_vector * RGB = gsl_vector_alloc (3);

	for(int i=0;i<dlen;i++)
	{
		gsl_vector_set(d,i,dstart+i*dstep);	
	}

	for(int i=0;i<len;i++)
	{
		gsl_vector_set(lambda,i,360+i);
	}

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

            		//Reflec = [-exp(1i*pi),0;0,-exp(1i*pi)];

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

			// bypass matrix manipulations here for simple case, test in matlab
			//gsl_matrix_complex_get(E,0,0)

			gsl_vector_complex_set(E,0,sample_c[1]); //check index

			Ts = gsl_complex_abs2(sample_c[1]); 

			//printf("Ts %g\n",Ts);

			xmi = gsl_vector_get(xm,l);	
			ymi = gsl_vector_get(ym,l);
			zmi = gsl_vector_get(zm,l);

			gsl_vector_set(Xr,l,xmi*Ts);
			gsl_vector_set(Yr,l,ymi*Ts);
			gsl_vector_set(Zr,l,zmi*Ts);

			
		}

		//trapezium method - step is 1nm
		X = gsl_blas_dasum(Xr)-(gsl_vector_get(Xr,len-1))/2-(gsl_vector_get(Xr,0))/2;
		Y = gsl_blas_dasum(Yr)-(gsl_vector_get(Yr,len-1))/2-(gsl_vector_get(Yr,0))/2;
		Z = gsl_blas_dasum(Zr)-(gsl_vector_get(Zr,len-1))/2-(gsl_vector_get(Zr,0))/2;

		Norm = X+Y+Z;
		
		//Xn = X/Norm;
        	//Yn = Y/Norm;
        	//Zn = Z/Norm;

		//printf("X%g Y%g Z%g\n",X,Y,Z);

		//gsl_vector_set(XYZ,0,X);
		//gsl_vector_set(XYZ,1,Y);
		//gsl_vector_set(XYZ,2,Z);

		//XYZ2RGB(RGB,XYZ);

		for(yp=0;yp<ypix;yp++) //will loop on rho for this
		{
			gsl_matrix_set(Xmat,dc,yp,Xn);
			gsl_matrix_set(Ymat,dc,yp,Yn);
			gsl_matrix_set(Zmat,dc,yp,Zn);	
		}
	
	//printf("R%g G%g B%g\n",gsl_vector_get(RGB,0),gsl_vector_get(RGB,1),gsl_vector_get(RGB,2));
		
	}
	
	//free vector and matrix memory
	gsl_vector_free (xm);
	gsl_vector_free (ym);
	gsl_vector_free (zm);

	gsl_vector_free (Xr);
	gsl_vector_free (Yr);
	gsl_vector_free (Zr);

	gsl_matrix_free (A);
	gsl_matrix_free (P);

	gsl_matrix_complex_free (Comp);
	gsl_matrix_complex_free (Sample);
	gsl_matrix_complex_free (Reflect);

	gsl_vector_free (XYZ);
	gsl_vector_free (RGB);
	
} 

void DLC::XYZ2RGB(gsl_vector * RGB, gsl_vector * XYZ)
{
	// http://www.easyrgb.com/
	//replace with matrix vector multiplication
	
	double var_R, var_G, var_B;
	double X, Y, Z;

	X = gsl_vector_get(XYZ,0);
	Y = gsl_vector_get(XYZ,1);
	Z = gsl_vector_get(XYZ,2);

	
	var_R = X/100 *  3.2406 + Y/100 * -1.5372 + Z/100 * -0.4986;
	var_G = X/100 * -0.9689 + Y/100 *  1.8758 + Z/100 *  0.0415;
	var_B = X/100 *  0.0557 + Y/100 * -0.2040 + Z/100 *  1.0570;

	if (var_R>0.0031308) {
		var_R=1.055*pow(var_R,(1/2.4))-0.055;
	}
	else {
		var_R = 12.92*var_R;
	}

	if ( var_G > 0.0031308 ){
		var_G = 1.055*pow(var_G,(1/2.4))-0.055;
	}
	else      
	{               
		var_G = 12.92*var_G;
	}

	if ( var_B > 0.0031308 ) {
		var_B = 1.055 * pow(var_B,(1/2.4))- 0.055;
	}
	else       
	{              
		var_B = 12.92 * var_B;
	}

	gsl_vector_set(RGB,0,var_R*255);
	gsl_vector_set(RGB,1,var_G*255);
	gsl_vector_set(RGB,2,var_B*255);

}


