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

	double Dn = 0.11;
	int dlen = 500;
	int dstart = 100;
	int dend = 1000; 
	int ypix = 100;
	int yp,dc;

	double X, Y, Z;

	gsl_matrix * Xmat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Ymat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Zmat = gsl_matrix_alloc (dlen,ypix);
	
	data.Michel_levy(Dn,dlen,dstart,dend,Xmat,Ymat,Zmat);

	CImg<double> img(dlen,ypix,0,3,0);
	
	for(dc=0;dc<dlen;dc++)
	{
	for(yp=0;yp<ypix;yp++) //will loop on rho for this
		{
			X = gsl_matrix_get(Xmat,dc,yp);
			Y = gsl_matrix_get(Ymat,dc,yp);
			Z = gsl_matrix_get(Zmat,dc,yp);
			
			const double origin = img(0,0,0,0);
			img(1,0,0,0) = origin;
			//image(dc,yp,0,0)=0.9;		// seg fault here 
			//image(dc,yp,0,1)=gsl_matrix_get(Ymat,dc,yp);
			//image(dc,yp,0,2)=gsl_matrix_get(Zmat,dc,yp);
		}
	}

	//CImgDisplay main_disp(image.XYZtoRGB());
	//while (!main_disp.is_closed()) {
      	//	main_disp.wait();
	//}
	

	gsl_matrix_free(Xmat);
	gsl_matrix_free(Ymat);
	gsl_matrix_free(Zmat);

	return 0;
}
