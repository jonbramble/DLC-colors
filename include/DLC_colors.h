#ifndef DLC_colors_H
#define DLC_colors_H

#include <iostream>
#include <string>

#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_math.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>

class DLC
{
public:
    void Michel_levy(double,int,int,int,int,gsl_matrix*,gsl_matrix*,gsl_matrix*);
	
private:
    static void XYZ2RGB(gsl_vector*, gsl_vector*);

    int m, len, yp, rhol, dc, l, i, rhostep;
    double dstep, X, Y, Z, Xn, Yn, Zn, Norm, DeltaComp, DeltaSamp, cDC2, sDC2, cDS2, sDS2, crho, srho, Ts, xmi, ymi, zmi, R, G, B;
    double lambda0, rho_max;
};

#endif
