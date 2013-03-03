#include "dac.h"
#include <cmath>

dac::dac(double VREF,int bitCount,int* bitWeight)
{
	this->VREF=VREF;
	this->bitCount=bitCount;
	if(bitCount<=0)
	{
		butCount=0;
	}
	else
	{
		bitReg= new int[bitCount];
		smplReg= new int[bitCount];
		bitWeight= new int[bitCount];
		totWeight=0;
		for(int count =0;count<bitCount;count++)
		{
			this->bitWeight[count]=bitWeight[count];
			this->bitReg[count]=0;
			this->smplReg[count]=1;
			totWeight+=pow(2,bitWeight[count]);
		}
	}
}

void dac::sample(double vin)
{
	this->vin=0.0;
	for(int count=0;count<bitCount;count++)
	{	
		this->vin=vin*smplReg[count]*pow(2,bitWeight);
	}
	this->vin/=totWeight;
}

void dac::setReg(int pos,int val)
{
	bitReg[pos]=val;
}

void dac::setSamplingCapPos(int* smplReg)
{
	for(int count=0;count<bitCount;count++)
	{	
		this->smplReg[count]=smplReg[count];
	}
}

double dac::getVal()
{
	double top=0.0;
	for(int count=0;count<bitCount;count++)
	{
		top+=2*bitReg[count]*pow(2,bitWeight[count])
	}
	top-=1;
	top=double(top)/double(pow(2,resolution));
	top=top-vin;
	return top;
}


dac::~dac()
{
	delete bitReg;
}
