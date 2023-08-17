#include "ViewPort.h"

ViewPort::ViewPort(){
	currScale = 1.;
	sc=0;
	minsc = 10;
	msm = 60.;
	bPrint = false;

	double r = 0.68;
	//double r = 0.5;
	double ret = 1.;
	for(int i=0;i<4;i++) ret *=r;
	mnoj = 1. + ret;

	scaleFactor = 1.;
	bTrans = false;
}