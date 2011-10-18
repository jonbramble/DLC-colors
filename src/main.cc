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
	int dlen = 5;
	int dstart = 100;
	int dend = 1000; 
	int ypix = 100;
	int yp,dc;

	gsl_matrix * Xmat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Ymat = gsl_matrix_alloc (dlen,ypix);
	gsl_matrix * Zmat = gsl_matrix_alloc (dlen,ypix);
	
	data.Michel_levy(Dn,dlen,dstart,dend,Xmat,Ymat,Zmat);

	CImg<double> image(dlen,ypix,0,3);

	//printf("%g\n",gsl_matrix_get(Xmat,5,5));

	for(dc=0;dc<dlen;dc++)
	{
	for(yp=0;yp<ypix;yp++) //will loop on rho for this
		{
			image(dc,yp,0,0)=gsl_matrix_get(Xmat,dc,yp);
			image(dc,yp,0,1)=gsl_matrix_get(Ymat,dc,yp);
			image(dc,yp,0,2)=gsl_matrix_get(Zmat,dc,yp);
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
