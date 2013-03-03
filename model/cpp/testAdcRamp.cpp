#include "nlvsar.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdlib>
using namespace std;

int main()
{
	srand(time(NULL));
	ofstream dataOut("data.dat",ios::out);
	ofstream resaOut("resa.dat",ios::out);
	ofstream resbOut("resb.dat",ios::out);
	ofstream rescOut("resc.dat",ios::out);
	ofstream resdOut("resd.dat",ios::out);
	ofstream reseOut("rese.dat",ios::out);
	ofstream cnvTimeOut("cnvTime.dat",ios::out);
	nlvsar *adc = new nlvsar(0.9,0);
	double input,residue;
	long code;
	int rampTest=3;
	if(rampTest==1)
	{
		for(input=-1.0*0.9/pow(2,6);input<1.0*0.9/pow(2,6);input+=0.00001)
		{
			adc->sample(0.45+input/2,0.45-input/2);
			//printf("%1.6f\t",input);
			code=adc->convert(64);	
			dataOut<<(int(code+1))/2+2048<<endl;
			resaOut<<input<<endl;
			resbOut<<adc->getResidue(0)<<endl;
			rescOut<<adc->getEst()<<endl;
			resdOut<<adc->getCode()<<endl;
			reseOut<<adc->getSgn()<<endl;
		}
		//linTest->computeLin("data.dat");
	}
	else if(rampTest==2)
	{
		for(int dith=0;dith<=128;dith+=16)
		{
			for(int count=0;count<100000;count++)
			{
				input=0.9/pow(2,6)*sin(2*3.14/997.0*count);
				adc->sample(0.45+input/2,0.45-input/2);
				//printf("%1.6f\t",input);
				code=adc->convert(dith);	
				dataOut<<code<<endl;
				resaOut<<input<<endl;
				resbOut<<adc->getDither()<<endl;
				rescOut<<adc->getEst()<<endl;
				resdOut<<adc->getCode()<<endl;
				reseOut<<adc->getSgn()<<endl;
			}
			adc->rstEst();
			cout<<dith<<endl;
		}
	}
	else
	{
		adc->sample(0.450-.000108/2.0,0.45+.000108/2.0);//+0.9/pow(2,10)*1.5,0.45-0.9/pow(2,10)*1.5);
		//adc->sample(0.025,-0.025);
		adc->convert(64);	
	}
	//delete adc;
	return 1;
}
