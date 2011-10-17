AC_INIT([DLC_colors], [0.1], [phyjpb@leeds.ac.uk], [DLC_colors], [http://mpi.leeds.ac.uk/])
AC_PREREQ([2.68])
AM_INIT_AUTOMAKE([1.11 -Wall no-define])
AC_CONFIG_HEADERS([config.h])

AM_PATH_GSL(1.13,[],AC_MSG_ERROR(could not find required version of GSL))

[CIMG_LIBS="-L/usr/X11R6/lib -lpthread -lX11"] 

AC_SUBST([CIMG_LIBS])

AC_PROG_CXX
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
