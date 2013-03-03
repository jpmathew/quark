#include "cdac.h"
#include "simpleFunctions.h"

cdac::cdac()
{
	capCount=32;
	samplingCapCount=12;
	capWeight=new double[capCount];
	//Flash
	capWeight[0]= 1.0/double(pow(2,2));
	capWeight[1]= 1.0/double(pow(2,2));
	capWeight[2]= 1.0/double(pow(2,3));
	capWeight[3]= 1.0/double(pow(2,3));
	capWeight[4]= 1.0/double(pow(2,4));
	capWeight[5]= 1.0/double(pow(2,4));
	capWeight[6]= 1.0/double(pow(2,5));
	capWeight[7]= 1.0/double(pow(2,5));
	capWeight[8]= 1.0/double(pow(2,6));  
	capWeight[9]= 1.0/double(pow(2,6));  
	//SAR
	capWeight[10]= 1.0/double(pow(2,6));  //5R
	capWeight[11]= 1.0/double(pow(2,6));
	capWeight[12]= 1.0/double(pow(2,7));  //6
	capWeight[13]= 1.0/double(pow(2,7));
	capWeight[14]= 1.0/double(pow(2,8));  //7
	capWeight[15]= 1.0/double(pow(2,8));
	capWeight[16]= 1.0/double(pow(2,8));  //7R
	capWeight[17]= 1.0/double(pow(2,8));
	capWeight[18]= 1.0/double(pow(2,9));  //8
	capWeight[19]= 1.0/double(pow(2,9));
	capWeight[20]= 1.0/double(pow(2,10)); //9
	capWeight[21]= 1.0/double(pow(2,10));
	capWeight[22]= 1.0/double(pow(2,10)); //9R
	capWeight[23]= 1.0/double(pow(2,10));
	capWeight[24]= 1.0/double(pow(2,11)); //10
	capWeight[25]= 1.0/double(pow(2,11));
	capWeight[26]= 1.0/double(pow(2,12)); //11
	capWeight[27]= 1.0/double(pow(2,12));
	capWeight[28]= 1.0/double(pow(2,13)); //12
	capWeight[29]= 1.0/double(pow(2,13));
	capWeight[30]= 1.0/double(pow(2,14)); //13
	capWeight[31]= 1.0/double(pow(2,14));
	//bottompaltes
	totSamplingCap=0;
	capVoltage=new double[capCount];
	for(int iter=0;iter<capCount;iter++)
	{
		capVoltage[iter]=0.0;
		totSamplingCap+=capWeight[iter];
	}
}

cdac::cdac(int errCount,double* err)
{
	capCount=32;
	samplingCapCount=12;
	capWeight=new double[capCount];
	//Flash
	capWeight[0]= 1.0/double(pow(2,2));
	capWeight[1]= 1.0/double(pow(2,2));
	capWeight[2]= 1.0/double(pow(2,3));
	capWeight[3]= 1.0/double(pow(2,3));
	capWeight[4]= 1.0/double(pow(2,4));
	capWeight[5]= 1.0/double(pow(2,4));
	capWeight[6]= 1.0/double(pow(2,5));
	capWeight[7]= 1.0/double(pow(2,5));
	capWeight[8]= 1.0/double(pow(2,6));  
	capWeight[9]= 1.0/double(pow(2,6));  
	//SAR
	capWeight[10]= 1.0/double(pow(2,6));  //5R
	capWeight[11]= 1.0/double(pow(2,6));
	capWeight[12]= 1.0/double(pow(2,7));  //6
	capWeight[13]= 1.0/double(pow(2,7));
	capWeight[14]= 1.0/double(pow(2,8));  //7
	capWeight[15]= 1.0/double(pow(2,8));
	capWeight[16]= 1.0/double(pow(2,9));  //8
	capWeight[17]= 1.0/double(pow(2,9));
	capWeight[18]= 1.0/double(pow(2,9));  //8R
	capWeight[19]= 1.0/double(pow(2,9));
	capWeight[20]= 1.0/double(pow(2,10)); //9
	capWeight[21]= 1.0/double(pow(2,10));
	capWeight[22]= 1.0/double(pow(2,11)); //10
	capWeight[23]= 1.0/double(pow(2,11));
	capWeight[24]= 1.0/double(pow(2,12)); //11
	capWeight[25]= 1.0/double(pow(2,12));
	capWeight[26]= 1.0/double(pow(2,13)); //12
	capWeight[27]= 1.0/double(pow(2,13));
	capWeight[28]= 1.0/double(pow(2,14)); //12
	capWeight[29]= 1.0/double(pow(2,14));
	capWeight[30]= 1.0/double(pow(2,15)); //13
	capWeight[31]= 1.0/double(pow(2,15));
	//bottompaltes
	for(int iter=0;iter<errCount;iter++)
	{
		capWeight[iter]+=err[iter];
	}
	//bottompaltes
	capVoltage=new double[capCount];
	totSamplingCap=0;
	for(int iter=0;iter<capCount;iter++)
	{
		capVoltage[iter]=0.0;
		totSamplingCap+=capWeight[iter];
	}
}

cdac::cdac(int capCount,int samplingCapCount,double *capWeight)
{
	this->capCount=capCount;
	this->samplingCapCount=samplingCapCount;
	totSamplingCap=0;
	for(int iter=0;iter<capCount;iter++)
	{
		this->capWeight[iter]=capWeight[iter];
	}
	for(int iter=0;iter<capCount;iter++)
	{
		capVoltage[iter]=0.0;
		totSamplingCap+=capWeight[iter];
	}
}

void cdac::sample(double vin,double refp,double refm)
{
	topPlateVoltage=0.0;
	for(int iter=0;iter<samplingCapCount;iter++)
	{
		capVoltage[iter]=vin;
	}
	for(int iter=samplingCapCount;iter<capCount;iter++)
	{
		if((iter-samplingCapCount)%2==0)
		{
			capVoltage[iter]=refm;
		}
		else
		{
			capVoltage[iter]=refp;
		}
	}
}

void cdac::calibSample(double refp,double refm,int prcode)
{
	topPlateVoltage=0.0;
	for(int iter=0;iter<capCount;iter++)
	{
		if(iter%2==0)
		{
			capVoltage[iter]=refm;
		}
		else
		{
			capVoltage[iter]=refp;
		}
	}
	//capVoltage[22]=double(prcode)/32.0*(refp-refm);
}

double cdac::changeBotPlateVoltage(double botVoltage,int botPosition)
{
	topPlateVoltage+=(botVoltage-capVoltage[botPosition])*capWeight[botPosition];
	capVoltage[botPosition]=botVoltage;
	return topPlateVoltage;
}

double cdac::getTopPlateVoltage()
{
	return topPlateVoltage;
}

void cdac::printBotstat()
{
	for(int iter=0;iter<capCount;iter++)
	{
		cout<<capVoltage[iter]<<"\t";
		if(iter%2==1)
		{
			cout<<endl;
		}
	}
	cout<<endl;
}

cdac::~cdac()
{
	delete [] capVoltage;
	delete [] capWeight;
}

	






	





