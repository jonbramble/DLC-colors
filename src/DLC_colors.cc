#include "../include/DLC_colors.h"

void DLC::Michel_levy(double Dn, int ypix, int dstart, int dend, int dlen, bool inccomp, gsl_matrix * Xmat, gsl_matrix * Ymat, gsl_matrix * Zmat)
{
    if(inccomp){
        std::cout << "Compensator In" << std::endl;
    }
    else {
        std::cout << "Compensator Out" << std::endl;
    }

    len = 471; // data length defined by cie data - do not edit
    lambda0 = 528;   // for my compensator this is correct - do not edit
    m = 4;		// this is an approximation to a real device - do not edit

    rho_max = M_PI/4;	// we rotate by 45 each way
    rhostep = (2*rho_max)/(double)ypix; // in steps of ypix

    gsl_complex one = gsl_complex_rect(0,1);
    gsl_complex zero = gsl_complex_rect(0,0);

    dstep = (dend-dstart)/dlen;

    //allocate storage
    gsl_vector_complex * A = gsl_vector_complex_alloc (2);
    gsl_matrix_complex * P = gsl_matrix_complex_alloc (2,2);

    gsl_matrix_complex * Comp = gsl_matrix_complex_alloc (2,2);

    gsl_matrix_complex * Sample = gsl_matrix_complex_alloc (2,2);
    gsl_matrix_complex * Reflect = gsl_matrix_complex_alloc (2,2);
    gsl_matrix_complex * TempA = gsl_matrix_complex_alloc (2,2);
    gsl_matrix_complex * TempB = gsl_matrix_complex_alloc (2,2);

    gsl_complex comp_c[2];
    gsl_complex sample_c[4];
    gsl_complex refl_c[4];

    gsl_complex Tx, Ty;

    //set analyser
    gsl_vector_complex_set(A,0,zero);
    gsl_vector_complex_set(A,1,one);

    //set polariser
    gsl_matrix_complex_set(P,0,0,one);
    gsl_matrix_complex_set(P,0,1,zero);
    gsl_matrix_complex_set(P,1,0,zero);
    gsl_matrix_complex_set(P,1,1,zero);

    //allocate storage
    gsl_vector * d = gsl_vector_alloc (dlen);
    gsl_vector * lambda = gsl_vector_alloc (len);
    gsl_vector * rho = gsl_vector_alloc (ypix);

    gsl_vector * Xr = gsl_vector_alloc (len);
    gsl_vector * Yr = gsl_vector_alloc (len);
    gsl_vector * Zr = gsl_vector_alloc (len);

    gsl_vector_complex * E = gsl_vector_complex_alloc (2);

    gsl_vector * XYZ = gsl_vector_alloc (3);
    gsl_vector * RGB = gsl_vector_alloc (3);

    // load cie data
    gsl_vector * xm = gsl_vector_alloc (len);
    gsl_vector * ym = gsl_vector_alloc (len);
    gsl_vector * zm = gsl_vector_alloc (len);

    //precalculate values for d
    std::cout << "Preallocate d values" << std::endl;
    for(i=0;i<dlen;i++)
    {
        gsl_vector_set(d,i,dstart+i*dstep);
        //std::cout << dstart+i*dstep << std::endl;
    }

    //precalculate values for lambda
    std::cout << "Preallocate lambda values" << std::endl;
    for(i=0;i<len;i++)
    {
        gsl_vector_set(lambda,i,360+i);
    }

    //precalculate values for rho
    std::cout << "ypix " << ypix << std::endl;
    std::cout << "Preallocate rho values" << std::endl;
    for(i=0;i<ypix;i++)
    {
        gsl_vector_set(rho,i,(-1*rho_max)+rhostep*i);
    }


    //should check these are present
    std::cout << "Loading color data ....";
    FILE * fxm = fopen("xm.txt", "r");
    FILE * fym = fopen("ym.txt", "r");
    FILE * fzm = fopen("zm.txt", "r");

    //read file data
    gsl_vector_fscanf(fxm,xm);
    gsl_vector_fscanf(fym,ym);
    gsl_vector_fscanf(fzm,zm);

    fclose(fxm);
    fclose(fym);
    fclose(fzm);
    // end of file loading
    std::cout << "complete" << std::endl;

    for (rhol=0;rhol<ypix;rhol++)
    {
        for (dc=0;dc<dlen;dc++)
        {
            for (l=0;l<len;l++)
            {

                DeltaSamp = 2*M_PI*Dn*gsl_vector_get(d,dc)/gsl_vector_get(lambda,l);

                //Reflec = [-exp(1i*pi),0;0,-exp(1i*pi)];

                cDS2 = cos(DeltaSamp/2);
                sDS2 = sin(DeltaSamp/2);

                crho = cos(gsl_vector_get(rho,rhol));
                srho = sin(gsl_vector_get(rho,rhol));

                if(inccomp)
                {
                    DeltaComp = (m*M_PI*lambda0)/gsl_vector_get(lambda,l);

                    cDC2 = cos(DeltaComp/2);
                    sDC2 = sin(DeltaComp/2);

                    comp_c[0] = gsl_complex_rect(cDC2,0);
                    comp_c[1] = gsl_complex_rect(0,sDC2);

                    gsl_matrix_complex_set(Comp,0,0,comp_c[0]);
                    gsl_matrix_complex_set(Comp,0,1,comp_c[1]);
                    gsl_matrix_complex_set(Comp,1,0,comp_c[1]);
                    gsl_matrix_complex_set(Comp,1,1,comp_c[0]);
                }

                sample_c[0] = gsl_complex_rect(cDS2,(1-2*pow(srho,2))*sDS2);
                sample_c[1] = gsl_complex_rect(0,2*crho*srho*sDS2);
                sample_c[2] = gsl_complex_rect(0,2*crho*srho*sDS2);
                sample_c[3] = gsl_complex_rect(cDS2,(1-2*pow(crho,2))*sDS2);

                gsl_matrix_complex_set(Sample,0,0,sample_c[0]);
                gsl_matrix_complex_set(Sample,0,1,sample_c[1]);
                gsl_matrix_complex_set(Sample,1,0,sample_c[2]);
                gsl_matrix_complex_set(Sample,1,1,sample_c[3]);

                //matrix manipulations
                //	%E = P*Sample*A;
                //	E = P*Sample*Reflec*Sample*A;
                //	Ts = E'*E;  careful with complex conj in C++
                //	T(l) = Ts; % generate the spectra

                if(inccomp)
                {
                    gsl_blas_zgemm(CblasNoTrans,CblasNoTrans,one,P,Sample,zero,TempA);   // TempA = P*Sample
                    gsl_blas_zgemm(CblasNoTrans,CblasNoTrans,one,TempA,Comp,zero,TempB);  // TempB = TempA*Comp
                    gsl_blas_zgemv(CblasNoTrans,one,TempB,A,zero,E);		     // E = TempB*A
                }
                else
                {
                    gsl_blas_zgemm(CblasNoTrans,CblasNoTrans,one,P,Sample,zero,TempA);   // TempA = P*Sample
                    gsl_blas_zgemv(CblasNoTrans,one,TempA,A,zero,E);		     // E = TempA*A
                }


                Ts = gsl_complex_abs2(gsl_vector_complex_get(E,0))+gsl_complex_abs2(gsl_vector_complex_get(E,1));

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

            Xn = X/Norm;
            Yn = Y/Norm;
            Zn = Z/Norm;

            //gsl_vector_set(XYZ,0,Xn);
            //gsl_vector_set(XYZ,1,Yn);
            //gsl_vector_set(XYZ,2,Zn);
            //XYZ2RGB(RGB,XYZ); // convert to RGB
            //R = gsl_vector_get(RGB,0);
            //G = gsl_vector_get(RGB,1);
            //B = gsl_vector_get(RGB,2);

            gsl_matrix_set(Xmat,dc,rhol,Xn); // changed from RGB to XYZ
            gsl_matrix_set(Ymat,dc,rhol,Yn);
            gsl_matrix_set(Zmat,dc,rhol,Zn);

	    //gsl_matrix_set(Xmat,dc,rhol,R); // changed from XYZ to RGB
            //gsl_matrix_set(Ymat,dc,rhol,G);
            //gsl_matrix_set(Zmat,dc,rhol,B);
        }
    }

    //free vector and matrix memory


    gsl_vector_free (xm);
    gsl_vector_free (ym);
    gsl_vector_free (zm);

    gsl_vector_free (Xr);
    gsl_vector_free (Yr);
    gsl_vector_free (Zr);

    gsl_vector_free (d);
    gsl_vector_free (lambda);
    gsl_vector_free (rho);

    gsl_vector_complex_free (A);
    gsl_matrix_complex_free (P);

    gsl_matrix_complex_free (Comp);
    gsl_matrix_complex_free (Sample);
    gsl_matrix_complex_free (Reflect);
    gsl_matrix_complex_free (TempA);
    gsl_matrix_complex_free (TempB);

    gsl_vector_free (XYZ);
    gsl_vector_free (RGB);

} 

void DLC::XYZ2RGB(gsl_vector * RGB, gsl_vector * XYZ)
{
    // http://www.easyrgb.com/
    double var_R, var_G, var_B;

    gsl_matrix * T = gsl_matrix_alloc (3,3);

    gsl_matrix_set(T,0,0,3.2406);
    gsl_matrix_set(T,0,1,-1.5372);
    gsl_matrix_set(T,0,2,-0.4986);
    gsl_matrix_set(T,1,0,-0.9689);
    gsl_matrix_set(T,1,1,1.8758);
    gsl_matrix_set(T,1,2,0.0415);
    gsl_matrix_set(T,2,0,0.0557);
    gsl_matrix_set(T,2,1,-0.2040);
    gsl_matrix_set(T,2,2,1.0570);

    gsl_blas_dgemv(CblasNoTrans,0.01,T,XYZ,0,RGB);

    var_R = gsl_vector_get(RGB,0);
    var_G = gsl_vector_get(RGB,1);
    var_B = gsl_vector_get(RGB,2);

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

    gsl_matrix_free(T);

}


