#ifndef DLC_plot_H
#define DLC_plot_H

#include "CImg.h"
#include "../include/DLC_colors.h"

using namespace cimg_library;

class DLCPlot 
{
public:
 void Plot_Bar(const double Dn,const int d,const int xpix,const int ypix,const bool inccomp);
 void Plot_Arc(const double Dn,const int d,const int xpix,const int ypix,const bool inccomp);
 void Plot_Full(const double Dn,const int dstart,const int dend,const int dlen,const int ypix,const bool inccomp);
 void Plot_SimpleChart(const double Dn,const int dstart,const int dend,const int dlen,const int ypix,const bool inccomp);
};

#endif
