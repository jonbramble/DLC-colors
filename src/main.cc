#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../include/DLC_plot.h"
#include "CImg.h"

int main(int argc, char* argv[]) {

	double Dn = 0.12;
	int dlen = 750;
	int dstart = 10;
	int dend = 5000;
	int ypix = 250;
        int xpix = 750;
        int d = 500;
	
	DLCPlot *Plot = new DLCPlot();
	//Plot->Plot_Full(Dn,dlen,dstart,dend,ypix);
        //Plot->Plot_Bar(Dn,d,xpix,ypix);
	Plot->Plot_Arc(Dn,d,750,10);

	return 0;
}
