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
	
	DLCPlot *Plot = new DLCPlot();
	Plot->Plot_Full(Dn,dlen,dstart,dend,ypix);

	return 0;
}
