#include "vernierRing.h"
vernierRing::vernierRing(double ts,double tf)
{
	this->ts=ts;
	this->tf=tf;
	COUNTMAX=65536;
}
void vernierRing::resolveTime(double tIn,int *add,int *sub,int length)
{
	int vLen=pow(2,length-1);
	int iAdd,iSub;
	iAdd=0;
	iSub=0;
	if(tIn>0)
	{
		add[length-1]=1;
		for(int count=1;count<vLen;count++)
		{
			if(tIn-count*(ts-tf)>0)
			{
				iAdd++;
			}
		}
	}
	else
	{
		sub[length-1]=1;
		for(int count=1;count<vLen;count++)
		{
			if(tIn-count*(tf-ts)<0)
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

