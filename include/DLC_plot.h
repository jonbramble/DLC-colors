#ifndef DLC_plot_H
#define DLC_plot_H

#include "CImg.h"
#include "../include/DLC_colors.h"

using namespace cimg_library;

class DLCPlot 
{
public:
 void Plot_Bar();
 void Plot_Arc();
 void Plot_Full(double Dn, int dlen, int dstart, int dend, int ypix);

};

#endif
