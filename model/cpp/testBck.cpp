#include "nlvsar.h"
#include "xnlCompute.h"
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
	ofstream data1Out("data1.dat",ios::out);
	ofstream data2Out("data2.dat",ios::out);
	ofstream resaOut("resa.dat",ios::out);
	ofstream resbOut("resb.dat",ios::out);
	ofstream rescOut("resc.dat",ios::out);
	ofstream resdOut("resd.dat",ios::out);
	ofstream reseOut("rese.dat",ios::out);
	ofstream cnvTimeOut("cnvTime.dat",ios::out);
	xnlCompute *linTest = new xnlCompute(7);
	nlvsar *adc = new nlvsar(0.9,0);
	double input,residue;
	long code;
	int rampTest=1;
	if(rampTest==1)
	{
		adc->setGCor(0);
		for(input=-1.0*0.9/pow(2,6);input<1.0*0.9/pow(2,6);input+=0.000001)
		{
			adc->sample(0.45+input/2,0.45-input/2);
			//printf("%1.6f\t",input);
			code=adc->convert();	
			dataOut<<(int(code+1))/2+64<<endl;
			resaOut<<input<<endl;
			resbOut<<adc->getResidue(0)<<endl;
			rescOut<<adc->getOffCorr()<<endl;
			reseOut<<adc->getSgn()<<endl;
		}
		adc->setGEst(1);
		for(int count=0;count<1000000;count++)
		{
			input=0.9/pow(2,6)*sin(2*3.14/997.0*count);
			adc->sample(0.45+input/2,0.45-input/2);
			code=adc->convert();	
			data1Out<<code<<endl;
			resaOut<<input<<endl;
			resbOut<<adc->getIEst()<<endl;
			rescOut<<adc->getOffCorr()<<endl;
			reseOut<<adc->getSgn()<<endl;
		}
		for(input=-1.0*0.9/pow(2,6);input<1.0*0.9/pow(2,6);input+=0.000001)
		{
			adc->sample(0.45+input/2,0.45-input/2);
			//printf("%1.6f\t",input);
			code=adc->convert();	
			data2Out<<(int(code+1))/2+64<<endl;
			resaOut<<input<<endl;
			resbOut<<adc->getResidue(0)<<endl;
			rescOut<<adc->getOffCorr()<<endl;
			reseOut<<adc->getSgn()<<endl;
		}

		linTest->computeLin("data.dat","1");
		linTest->computeLin("data2.dat","2");

	}
	else if(rampTest==2)
	{
		for(int count=0;count<1000000;count++)
		{
			input=0.9/pow(2,6)*sin(2*3.14/997.0*count);
			adc->sample(0.45+input/2,0.45-input/2);
			code=adc->convert();	
			dataOut<<code<<endl;
			resaOut<<input<<endl;
			resbOut<<adc->getIEst()<<endl;
			rescOut<<adc->getOffCorr()<<endl;
			reseOut<<adc->getSgn()<<endl;
		}
	}
	else
	{
		adc->sample(0.450-.000108/2.0,0.45+.000108/2.0);//+0.9/pow(2,10)*1.5,0.45-0.9/pow(2,10)*1.5);
		//adc->sample(0.025,-0.025);
		adc->convert();	
	}
	//delete adc;
	return 1;
}
