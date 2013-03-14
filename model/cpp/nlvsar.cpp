#include "nlvsar.h"
#include <cstdio>
using namespace std;

int sign(int a)
{
	return a>0?1:0;
}

nlvsar::nlvsar(double refp,double refm)
{
	this->refp=refp;
	this->refm=refm;
	cdacP=new cdac();
	cdacM=new cdac();
	vcoP=new nlvco();
	vcoM=new nlvco();
	//rring=new vernierRing(10,8,16);
	rring=new vernierRing(16);
	tCorrP=new double[8];
	tCorrM=new double[8];
	voff=new double[3];
	numBits=new int[3];
	startPos=new int[3];
	numLoops=new int[3];
	add=new int[16];
	sub=new int[16];
	bitWeight=new int[16];
	residue=new double[3];
	voff[0]=4.0*(refp-refm)/double(pow(2,5));
	startPos[0]=5;
	numBits[0]=3;
	numLoops[0]=1;
	voff[1]=4.0*(refp-refm)/double(pow(2,7));
	startPos[1]=8;
	numBits[1]=3;
	numLoops[1]=1;
	voff[2]=8.0*(refp-refm)/double(pow(2,9));
	startPos[2]=11;
	numBits[2]=4;
	numLoops[2]=4;
	bitWeight[0]=pow(2,11);
	bitWeight[1]=pow(2,10);
	bitWeight[2]=pow(2,9);
	bitWeight[3]=pow(2,8);
	bitWeight[4]=pow(2,7);
	bitWeight[5]=pow(2,7);
	bitWeight[6]=pow(2,6);
	bitWeight[7]=pow(2,5);
	bitWeight[8]=pow(2,5);
	bitWeight[9]=pow(2,4);
	bitWeight[10]=pow(2,3);
	bitWeight[11]=pow(2,3);
	bitWeight[12]=pow(2,2);
	bitWeight[13]=pow(2,1);
	bitWeight[14]=pow(2,0);
	bitWeight[15]=pow(2,1);
	voffCor=0.0;
	enGEst=0;
	est=0;
	enGCor=0;
	for(int dcount=0;dcount<8;dcount++)
	{
		tCorrP[dcount]=0.0;
		tCorrM[dcount]=0.0;
	}
}

nlvsar::~nlvsar()
{
	delete cdacP;
	delete cdacM;
	delete vcoP;
	delete vcoM;
	delete rring;
	delete [] voff;
	delete [] residue;
	delete [] add;
	delete [] sub;
	delete [] numBits;
	delete [] bitWeight;
	delete [] startPos;
}

void nlvsar::sample(double vinp,double vinm)
{
	cdacP->sample(vinp,refp,refm);
	cdacM->sample(vinm,refp,refm);
	for(int iter=0;iter<16;iter++)
	{
		add[iter]=0;
		sub[iter]=0;
	}
}

int nlvsar::convert()
{
	double ainp,ainm;
	double tVcoP,tVcoM;
	int code;
	int icode;
	int iadd[5],isub[5];
	int iter,pos,count;
	for(pos=0;pos<14;pos++)
	{
		ainp=cdacP->changeBotPlateVoltage(add[pos]*refp+(1-add[pos])*refm,2*pos);
		ainp=cdacP->changeBotPlateVoltage(sub[pos]*refm+(1-sub[pos])*refp,2*pos+1);
		ainm=cdacM->changeBotPlateVoltage(add[pos]*refm+(1-add[pos])*refp,2*pos+1);
		ainm=cdacM->changeBotPlateVoltage(sub[pos]*refp+(1-sub[pos])*refm,2*pos);
	}
	code=0;
	int calPol;
	calPol=rand()%2;
	add[15]=calPol;
	sub[15]=1-calPol;
	sgn=add[15]-sub[15];
	iEst=0;
	double corr;
	for(iter=0;iter<3;iter++)
	{
		if(iter==2)
		{
			corr=enGCor*voffCor;
		}
		else
		{
			corr=0.0;
		}
		tVcoP=vcoP->getKEdge(numLoops[iter],cdacP->getTopPlateVoltage()-cdacM->getTopPlateVoltage()+voff[iter]+corr);
		tVcoM=vcoM->getKEdge(numLoops[iter],cdacP->getTopPlateVoltage()-cdacM->getTopPlateVoltage()-voff[iter]-corr);
		rring->resolveTime(tVcoP-tVcoM,add+startPos[iter],sub+startPos[iter],numBits[iter]);
		for(count=0;count<numBits[iter];count++)
		{
			pos=startPos[iter]+count;
			ainp=cdacP->changeBotPlateVoltage(add[pos]*refp+(1-add[pos])*refm,2*pos);
			ainp=cdacP->changeBotPlateVoltage(sub[pos]*refm+(1-sub[pos])*refp,2*pos+1);
			ainm=cdacM->changeBotPlateVoltage(add[pos]*refm+(1-add[pos])*refp,2*pos+1);
			ainm=cdacM->changeBotPlateVoltage(sub[pos]*refp+(1-sub[pos])*refm,2*pos);
			code+=(add[pos]-sub[pos])*bitWeight[pos];
			residue[iter]=ainp-ainm;
			if(iter==2)
			{
				iEst+=(add[pos]-sub[pos])*bitWeight[pos];
			}

		}
		if(iter==1 && enGEst==1)
		{
			pos=15;
			ainp=cdacP->changeBotPlateVoltage(add[pos]*refp+(1-add[pos])*refm,2*pos);
			ainp=cdacP->changeBotPlateVoltage(sub[pos]*refm+(1-sub[pos])*refp,2*pos+1);
			ainm=cdacM->changeBotPlateVoltage(add[pos]*refm+(1-add[pos])*refp,2*pos+1);
			ainm=cdacM->changeBotPlateVoltage(sub[pos]*refp+(1-sub[pos])*refm,2*pos);
			code+=(add[pos]-sub[pos])*bitWeight[pos];
		}
		if(iter==2 && enGEst==1)
		{
			for(int dcount=0;dcount<1;dcount++)
			{
				if(iEst==2*dcount+1 || iEst==2*dcount+3)
				{
					tCorrP[dcount]=tCorrP[dcount]+1e-4*(iEst-2*dcount-2);
					rring->setCorr(tCorrP[dcount],0,dcount);	
				}
			/*	if(iEst==-2*dcount-1 || iEst==-2*dcount-3)
				{
					tCorrM[dcount]=tCorrM[dcount]-1e-4*(iEst+2*dcount-2);
					rring->setCorr(tCorrM[dcount],1,dcount);	
				}*/
			}
			//iEst=iEst*(add[15]-sub[15]);
			//voffCor=voffCor-1e-7*(iEst+2);
		}
	}
	return code;
}

double nlvsar::getResidue(int stage)
{
	return residue[stage];
}

double nlvsar::getOffCorr()
{ 
	return voffCor;
}


int nlvsar::getIEst()
{ 
	return iEst;
}
double  nlvsar::getest()
{ 
	return est;
}
double nlvsar::getCorr(int pm,int count)
{
	return rring->getCorr(pm,count);

}


int nlvsar::getAdd(int pos)
{
	return add[pos];
}

int nlvsar::getSgn()
{
	return sgn;
}

void nlvsar::setGCor(int val)
{
	enGCor=val;
	if(val==0)
	{
		setGEst(0);
	}
}

void nlvsar::setGEst(int val)
{
	enGEst=val;
	if(val==1)
	{
		setGCor(1);
	}
}


