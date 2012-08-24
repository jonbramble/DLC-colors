tif <- readTiff("/home/DS/phyjpb/Programming/C/DLC-colors/bar_d1200_dn0p12.tiff")
plot(tif,xlab="Thickness (um)")
cat=c(0,200,400,600,800,1000);
dog=c("0","2","4","6","8","10");
axis(1,pos=-10,at=cat,labels=dog)
