#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../include/DLC_colors.h"
#include "CImg.h"

using namespace cimg_library;

int main(int argc, char* argv[]) {

	DLC *data = new DLC();	

	double Dn = 0.12;
	int dlen = 500;
	int dstart = 10;
	int dend = 5000;
	int ypix = 100;
	int ypm,dcm;

	double X, Y, Z;

	gsl_matrix * Xmat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Ymat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Zmat = gsl_matrix_alloc (dlen,ypix);
	
	data->Michel_levy(Dn,ypix,dstart,dend,dlen,Xmat,Ymat,Zmat);

	CImg<float> img(dlen,ypix,1,3);
	
	for(ypm=0;ypm<ypix;ypm++)
	{
		for(dcm=0;dcm<dlen;dcm++)
		{
			X = (float) gsl_matrix_get(Xmat,dcm,ypm);
			Y = (float) gsl_matrix_get(Ymat,dcm,ypm);
			Z = (float) gsl_matrix_get(Zmat,dcm,ypm);
			
			img.set_linear_atXY(X,dcm,ypm,0,0,false);
			img.set_linear_atXY(Y,dcm,ypm,0,1,false);
			img.set_linear_atXY(Z,dcm,ypm,0,2,false);
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
