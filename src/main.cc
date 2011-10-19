#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../include/DLC_colors.h"

#include "CImg.h"

using namespace cimg_library;


int main(int argc, char* argv[]) {
	/*int c, index;
	char* closed; 
	char* prima;
	char* appname = argv[0];
	vector<char*> ifiles;

	int one = 1, fd;
	int multiplefiles = (argc > 1);*/
	
	
	/*while((c = getopt(argc, argv, "p:c:")) != -1){			//taken from gnu example
		switch(c){
		case 'p':
			prima = optarg;
			break;
		case 'c':
			closed = optarg;
			break;
		default:
			fprintf(stderr, "usage: %s [-p background] [-c closed] others.tif ...\n", appname);
			exit(-1);
			
		}
	}*/

     

	//printf ("prima = %s, closed = %s\n ", prima, closed);

	//for (index = optind; index < argc; index++){
       // 	printf ("Image files %s\n", argv[index]);
	//	ifiles.push_back(argv[index]);
	//}


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
	for(yp=0;yp<ypix;yp++) //will loop on rho for this
		{
			X = (float) gsl_matrix_get(Xmat,dc,yp);
			Y = (float) gsl_matrix_get(Ymat,dc,yp);
			Z = (float) gsl_matrix_get(Zmat,dc,yp);
			
			img.set_linear_atXY(X,dc,yp,0,0,false);
			img.set_linear_atXY(Y,dc,yp,0,1,false);
			img.set_linear_atXY(Z,dc,yp,0,2,false);
		}
			//printf("X%g Y%g Z%g\n",X,Y,Z);
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
