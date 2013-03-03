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
	rring=new vernierRing(10,8);
	voff=new double[3];
	numBits=new int[3];
	startPos=new int[3];
	numLoops=new int[3];
	add=new int[16];
	sub=new int[16];
	bitWeight=new int[15];
	residue=new double[3];
	voff[0]=4.0*(refp-refm)/double(pow(2,5));
	startPos[0]=5;
	numBits[0]=3;
	numLoops[0]=1;
	voff[1]=4.0*(refp-refm)/double(pow(2,7));
	startPos[1]=8;
	numBits[1]=3;
	numLoops[1]=1;
	voff[2]=6.0*(refp-refm)/double(pow(2,9));
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
	est=0;
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

void nlvsar::rstEst()
{
	est=0.0;
	iEst=0;
}

int nlvsar::convert(int dith)
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
	//cout<<ainp<<"\t"<<ainm<<endl;
	//cout<<"-----------------------------------"<<endl;
	code=0;
	code16=0;
	th=0;
	double dadd;
	dither=(rand()%129-dith)*(refp-refm)/double(pow(2,17)); 
	for(iter=0;iter<3;iter++)
	{
		if(iter==1)
		{
			dadd=1.0*dither;//+4.0/double(pow(2,12))*(refp-refm);
		}
		else
		{
			dadd=0.0;
		}
		tVcoP=vcoP->getKEdge(numLoops[iter],cdacP->getTopPlateVoltage()-cdacM->getTopPlateVoltage()+voff[iter]+dadd);
		tVcoM=vcoM->getKEdge(numLoops[iter],cdacP->getTopPlateVoltage()-cdacM->getTopPlateVoltage()-voff[iter]+dadd);
		rring->resolveTime(tVcoP-tVcoM,add+startPos[iter],sub+startPos[iter],numBits[iter]);
		for(count=0;count<numBits[iter];count++)
		{
			pos=startPos[iter]+count;
			ainp=cdacP->changeBotPlateVoltage(add[pos]*refp+(1-add[pos])*refm,2*pos);
			ainp=cdacP->changeBotPlateVoltage(sub[pos]*refm+(1-sub[pos])*refp,2*pos+1);
			ainm=cdacM->changeBotPlateVoltage(add[pos]*refm+(1-add[pos])*refp,2*pos+1);
			ainm=cdacM->changeBotPlateVoltage(sub[pos]*refp+(1-sub[pos])*refm,2*pos);
			code+=(add[pos]-sub[pos])*bitWeight[pos];
			//cout<<add[pos]<<"\t"<<sub[pos]<<endl;
			if(iter==1)
			{
				code16+=(add[pos]-sub[pos])*bitWeight[pos]*2;
			}
			else if(iter==2)
			{
				code16+=(add[pos]-sub[pos])*bitWeight[pos];
			}


			if(iter==1)
			{
				th+=(add[pos])*pow(2,2-count);
			}
		}
		//cout<<"-----------------------------------"<<endl;
		//cout<<code<<"\t"<<ainp<<"\t"<<ainm<<endl;
		residue[iter]=ainp-ainm;
	}
	if(th>2)
	{
		sgn=1;
	}
	else
	{
		sgn=0;
	}
	//code+=2048;
	//est+=.002*(sign(code16-est)-sgn)*(abs((code16-est))+1);
	//double tmp=.001*(sign(code16-est)-sgn);//(code16-est);
	//iEst+=(sign(code16-iEst)-sgn);
	//est=0.9999*est+.0001*iEst;
	//cout<<code16<<"\t"<<iEst<<"\t"<<sgn<<endl;
	//cout<<"-----------------------------------"<<endl;
	//printf("%1.6f\t%1.6f",residue[0],residue[1]);
	//cout<<"\t"<<code16<<"\t"<<est<<"\t"<<add[9]<<endl;
	return code;
}

double nlvsar::getResidue(int stage)
{
	return residue[stage];
}

double nlvsar::getEst()
{ 
	return est;
}

int nlvsar::getIEst()
{ 
	return iEst;
}

double nlvsar::getDither()
{ 
	return dither;
}
int nlvsar::getCode()
{
	return code16;
}


int nlvsar::getAdd(int pos)
{
	return add[pos];
}

int nlvsar::getTh()
{
	return th;
}

int nlvsar::getSgn()
{
	return sgn;
}
