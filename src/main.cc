#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../include/DLC_colors.h"

#include "CImg.h"

using namespace cimg_library;


int main(int argc, char* argv[]) {

	DLC data;

	double Dn = 0.12;
	int dlen = 1024;
	int dstart = 100;
	int dend = 10000; //10um
	int ypix = 100;
	int yp,dc;

	double X, Y, Z;

	gsl_matrix * Xmat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Ymat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Zmat = gsl_matrix_alloc (dlen,ypix);
	
	data.Michel_levy(Dn,dlen,dstart,dend,Xmat,Ymat,Zmat);

	CImg<float> img(dlen,ypix,1,3);
	
	for(dc=0;dc<dlen;dc++)
	{
		for(yp=0;yp<ypix;yp++)
		{
			X = (float) gsl_matrix_get(Xmat,dc,yp);
			Y = (float) gsl_matrix_get(Ymat,dc,yp);
			Z = (float) gsl_matrix_get(Zmat,dc,yp);
			
			img.set_linear_atXY(X,dc,yp,0,0,false);
			img.set_linear_atXY(Y,dc,yp,0,1,false);
			img.set_linear_atXY(Z,dc,yp,0,2,false);
		}
	}

	CImgDisplay main_disp(img, "Michel Levy Chromatic Scale");
	while (!main_disp.is_closed()) {
      		main_disp.wait();
	}
	

	gsl_matrix_free(Xmat);
	gsl_matrix_free(Ymat);
	gsl_matrix_free(Zmat);

	return 0;
}
