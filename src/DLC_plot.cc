#include "../include/DLC_plot.h"

void DLCPlot::Plot_Bar(double Dn, int d, int xpix, int ypix )
{
	DLC *data = new DLC();	
	// xpix and ypix are now swapped, we are plotting the change in angle on x, no change in y
	gsl_matrix * Xmat = gsl_matrix_alloc (1,xpix);
	gsl_matrix * Ymat = gsl_matrix_alloc (1,xpix);
	gsl_matrix * Zmat = gsl_matrix_alloc (1,xpix);

	int ypm,xpm;
	double X, Y, Z;

       data->Michel_levy(Dn,xpix,d,d,1,Xmat,Ymat,Zmat);

	CImg<float> img(xpix,ypix,1,3);
	
	for(ypm=0;ypm<ypix;ypm++)
	{
		for(xpm=0;xpm<xpix;xpm++)
		{
			X = (float) gsl_matrix_get(Xmat,0,xpm);
			Y = (float) gsl_matrix_get(Ymat,0,xpm);
			Z = (float) gsl_matrix_get(Zmat,0,xpm);
			
			img.set_linear_atXY(X,xpm,ypm,0,0,false);
			img.set_linear_atXY(Y,xpm,ypm,0,1,false);
			img.set_linear_atXY(Z,xpm,ypm,0,2,false);		
		}
	}

  	gsl_matrix_free(Xmat);
	gsl_matrix_free(Ymat);
	gsl_matrix_free(Zmat); 

        img.XYZtoRGB();

	CImgDisplay main_disp(img, "Michel Levy Chromatic Scale");
	while (!main_disp.is_closed()) {
      		main_disp.wait();
	}

}

void DLCPlot::Plot_Arc(double Dn, int d, int xpix, int ypix)
{
	DLC *data = new DLC();	
	// xpix and ypix are now swapped, we are plotting the change in angle on x, no change in y
	gsl_matrix * Xmat = gsl_matrix_alloc (1,xpix);
	gsl_matrix * Ymat = gsl_matrix_alloc (1,xpix);
	gsl_matrix * Zmat = gsl_matrix_alloc (1,xpix);

        int ypm,xpm;
	double X, Y, Z;

        data->Michel_levy(Dn,xpix,d,d,1,Xmat,Ymat,Zmat);


	CImg<float> img(xpix,ypix,1,3);

	// tricky bit goes here

	// co-ords of elements on arc

	int rpm;
	double x,y;
	const int rin = 150;
	const int rout = 175;

	int xlim = (int)round(rout*0.707106781);

	std::cout << xlim << std::endl;

	double rho_max = M_PI/4;	// we rotate by 45 each way
	double rhostep = (2*rho_max)/(double)xpix; // in steps of xpix

	for(rpm=rin;rpm<rin+2;rpm++)
	{
		for(xpm=0;xpm<xpix/2;xpm++)
		{
			x = rpm*cos((rho_max)+rhostep*xpm); 
			y = rpm*sin((rho_max)+rhostep*xpm); 
			std::cout << x << "," << y << std::endl;
		}
	}


	gsl_matrix_free(Xmat);
	gsl_matrix_free(Ymat);
	gsl_matrix_free(Zmat); 

      /*  img.XYZtoRGB();

	CImgDisplay main_disp(img, "Michel Levy Chromatic Scale");
	while (!main_disp.is_closed()) {
      		main_disp.wait();
	}
*/
 
}

void DLCPlot::Plot_Full(double Dn, int dlen, int dstart, int dend, int ypix )
{
	DLC *data = new DLC();	

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

	gsl_matrix_free(Xmat);
	gsl_matrix_free(Ymat);
	gsl_matrix_free(Zmat);

	img.XYZtoRGB();

	CImgDisplay main_disp(img, "Michel Levy Chromatic Scale");
	while (!main_disp.is_closed()) {
      		main_disp.wait();
	}

	//save file
	img.save_tiff("10_5000_dn0p12.tif");
}

