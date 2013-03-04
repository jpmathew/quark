#ifndef NLVSAR_H
#define NLVSAR_H

#include "cdac.h"
#include "nlvco.h"
#include "vernierRing.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>

class nlvsar
{
	private:
		double refp,refm;
		cdac *cdacP,*cdacM;
		int *add,*sub;
		nlvco *vcoP,*vcoM;
		vernierRing* rring;
		double *voff;
		int *numBits,*startPos,*numLoops;
		int code;
		int *bitWeight;
		double *residue;
		int iEst;
		int sgn;
		double voffCor;
		int enGCor;
		int enGEst;
	public:
	nlvsar(double refp,double refm);
	void sample(double vinp,double vinm);
	int convert();
	double getResidue(int stage);
	int getIEst();
	double getOffCorr();
	int getCode();
	int getSgn();
	int getAdd(int pos);
	void setGCor(int val);
	void setGEst(int val);
	~nlvsar();
};
#endif
