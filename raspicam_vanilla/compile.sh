#!/bin/sh
INCDIR="/opt/vc/include"
EXTRAINCDIR1="$INCDIR/interface/vcos/pthreads"
EXTRAINCDIR2="$INCDIR/interface/vmcs_host/linux"
EXTRAINCDIR3="../../../../"
LIBDIR="/opt/vc/lib"
LDFLAGS="$(pkg-config --libs opencv) -lpthread -lmmal -lmmal_core -lmmal_util -lvcos -lbcm_host -Wl,-rpath-link=$LIBDIR"
CC=gcc

$CC $CFLAGS -I$INCDIR -I$EXTRAINCDIR1 -I$EXTRAINCDIR2 -I$EXTRAINCDIR3 -L$LIBDIR RaspiCamControl.c RaspiCLI.c RaspiPreview.c RaspiStill.c $LDFLAGS -o raspicam
