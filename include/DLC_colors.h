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
    void Michel_levy(double,int,int,int,gsl_matrix*,gsl_matrix*,gsl_matrix*);
private:
    void XYZ2RGB(gsl_vector*, gsl_vector*);
};

#endif
