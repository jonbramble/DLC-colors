#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../include/DLC_plot.h"
#include "CImg.h"

int main(int argc, char* argv[]) {

	double Dn = 0.12;
	int dlen = 750;
	int dstart = 200;
	int dend = 5000;
	int ypix = 125;
        int xpix = 750;
        int d = 1200;
	
	DLCPlot *Plot = new DLCPlot();
	//Plot->Plot_Full(Dn,dstart,dend,dlen,ypix);
        //Plot->Plot_Bar(Dn,d,xpix,ypix);
	//Plot->Plot_Arc(Dn,d,2000,10);

	Plot->Plot_SimpleChart(Dn,dstart,dend,dlen,ypix);
	delete Plot;

	return 0;
}
