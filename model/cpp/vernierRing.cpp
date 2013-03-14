#include "vernierRing.h"
vernierRing::vernierRing(double ts,double tf,int delLength)
{
	this->delLength=delLength;
	this->ts=ts;
	this->tf=tf;
	COUNTMAX=65536;
	delp=new double[delLength];
	corrp=new double[delLength];
	delm=new double[delLength];
	corrm=new double[delLength];
	for(int count=0;count<delLength;count++)
	{
		delp[count]=ts-tf;
		corrp[count]=0.0;
		delm[count]=tf-ts;
		corrm[count]=0.0;
	}
	delp[0]=3;
	delp[1]=1;
}
vernierRing::vernierRing(int delLength)
{
	this->delLength=delLength;
	this->ts=ts;
	this->tf=tf;
	COUNTMAX=65536;
	delp=new double[delLength];
	corrp=new double[delLength];
	delm=new double[delLength];
	corrm=new double[delLength];
	for(int count=0;count<delLength;count++)
	{
		delp[count]=2;//0.25+rand()%4;
		corrp[count]=0.0;
		delm[count]=-2;//-0.25-rand()%4;
		corrm[count]=0.0;
	}
	delp[0]=0.25+(rand()%400)/100.0;
	//delp[1]=0.25+(rand()%400)/100.0;
	//delp[2]=0.25+(rand()%400)/100.0;
	//std::cout <<delp[0]<<std::endl;
	//std::cout <<delp[1]<<std::endl;
	//std::cout <<delp[2]<<std::endl;
}
void vernierRing::resolveTime(double tIn,int *add,int *sub,int length)
{
	int vLen=pow(2,length-1);
	int iAdd,iSub;
	double dp,dm;
	iAdd=0;
	iSub=0;
	if(tIn>0)
	{
		add[length-1]=1;
		dp=0;
		for(int count=1;count<vLen;count++)
		{
			dp+=delp[count-1]+corrp[count-1];
			if(tIn-dp>0)
			{
				iAdd++;
			}
		}
	}
	else
	{
		sub[length-1]=1;
		dm=0;
		for(int count=1;count<vLen;count++)
		{
			dm+=delm[count-1]+corrm[count-1];
			if(tIn-dm<0)
			{
				iSub++;
			}
		}
	}
	if(iAdd>=vLen)
	{
		iAdd=vLen-1;
	}
	if(iSub>=vLen)
	{
		iSub=vLen-1;
	}
	//std::cout<<iAdd<<"\t"<<iSub<<std::endl<<std::endl;
	for(int count=1;count<length;count++)
	{
		add[length-count-1]=iAdd%2;
		//std::cout<<iAdd<<"\t"<<add[length-count-1]<<std::endl;
		iAdd/=2;
		//std::cout<<iAdd<<"\t"<<add[length-count-1]<<std::endl;
		//std::cout<<"-----------------------------------"<<std::endl;
		sub[length-count-1]=iSub%2;
		iSub/=2;
	}
}

void vernierRing::setTime(double ts,double tf)
{
	this->ts=ts;
	this->tf=tf;
}

void vernierRing::setCorr(double corr,int pm,int pos)
{
	if(pm==0)
	{
		corrp[pos]=corr;
	}
	else
	{
		corrm[pos]=corr;
	}
}
 double vernierRing::getCorr(int pm,int pos)
 {
	 if(pm==0)
	 {
		 return corrp[pos]+delp[pos];
	 }
	 else
	 {
		 return corrm[pos]+delm[pos];
	}
}

vernierRing::~vernierRing()
{
	delete [] delp;
	delete [] corrp;
	delete [] delm;
	delete [] corrm;
};


		 


