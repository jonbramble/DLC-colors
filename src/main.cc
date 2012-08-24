#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../include/DLC_plot.h"
#include "CImg.h"

int main(int argc, char* argv[]) {

	const double Dn = 0.12;
	const int dlen = 1000;
	const int dstart = 1;	//dont set to zero - unknown bug
	const int dend = 10000;
	const int ypix = 125;
        const int xpix = 750;
        const int d = 1000;

	const bool inccomp = true;
	
	DLCPlot *Plot = new DLCPlot();
	//Plot->Plot_Full(Dn,dstart,dend,dlen,ypix);
        //Plot->Plot_Bar(Dn,d,xpix,ypix,comp);
	Plot->Plot_Arc(Dn,d,2000,10,inccomp);

	//Plot->Plot_SimpleChart(Dn,dstart,dend,dlen,ypix,inccomp);
	delete Plot;

	return 0;
}
