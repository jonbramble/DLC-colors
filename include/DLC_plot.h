#ifndef DLC_plot_H
#define DLC_plot_H

#include "CImg.h"
#include "../include/DLC_colors.h"

using namespace cimg_library;

class DLCPlot 
{
public:
 void Plot_Bar(double Dn, int d, int xpix, int ypix);
 void Plot_Arc(double Dn, int d, int xpix, int ypix); // this will change to include r
 void Plot_Full(double Dn, int dstart, int dend, int dlen, int ypix);
 void Plot_SimpleChart(double Dn, int dstart, int dend, int dlen, int ypix );

};

#endif
