#include "adc.h"
#include "simpleFunctions.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;


adc::adc()
{	
	//error in lsbs
	double errp[12]={-10.0,-8.4,13.1,-15.3,8.6,4.9,1.1,6.2,2.6,4.3,2.9,2.3};
	double errm[12]={2.5,10.6,17.2,-7.0,9.1,2.8,5.4,4.5};
	for(int iter=0;iter<12;iter++)
	{
		errp[iter]/=2048.0;
		errm[iter]/=2048.0;
		threshErr[iter]=0;
		w[0][iter]=0;
		w[1][iter]=0;

	}
	enableThreshCal=0;
	threshErr[0]=(refp-refm)/16;
	cdacM=new cdac();
	cdacP=new cdac(12,errp);
}

void adc::sample(double vinp,double vinm)
{
	cdacP->sample(vinp,refp,refm);
	cdacM->sample(vinm,refp,refm);
	for(int iter=0;iter<12;iter++)
	{
		add[iter]=0;
		sub[iter]=0;
	}
}

void adc::calibSample(int prcode)
{
	cdacP->calibSample(refp,refm,prcode);
	cdacM->calibSample(refp,refm,0);
}

void adc::setCoe(int type,int pos,int val)
{
	if(type==0)
	{
		ea[pos]=val;
	}
	else
	{
		eb[pos]=val;
	}
}

void adc::printCoe()
{
	for (int iter=0;iter<6;iter++)
	{
		cout<<ea[iter]<<"\t"<<eb[iter]<<endl;
	}
}

int adc::convert()
{
	int iter;
	int errCo;
	double ainp,ainm;
	int capPosition=0;
	double vthresh=0.0;
	int code=0;
	double estThresh;
	//ofstream cnvfile("cnvinfo.dat",ios::out);

	for(iter=0;iter<11;iter++)
	{
		if(iter<5)
		{
			capPosition=iter;
		}
		else
		{
			capPosition=iter+1;
		}
		ainp=cdacP->changeBotPlateVoltage(add[iter]*refp+(1-add[iter])*refm,2*capPosition);
		ainp=cdacP->changeBotPlateVoltage(sub[iter]*refm+(1-sub[iter])*refp,2*capPosition+1);
		ainm=cdacM->changeBotPlateVoltage(add[iter]*refm+(1-add[iter])*refp,2*capPosition+1);
		ainm=cdacM->changeBotPlateVoltage(sub[iter]*refp+(1-sub[iter])*refm,2*capPosition);
	}
	//bal cap
	ainp=cdacP->changeBotPlateVoltage(refm,2*5);
	ainp=cdacP->changeBotPlateVoltage(refp,2*5+1);
	ainm=cdacM->changeBotPlateVoltage(refp,2*5+1);
	ainm=cdacM->changeBotPlateVoltage(refm,2*5);

	for(iter=0;iter<12;iter++)
	{
		
		if(iter==0)
		{
			estThresh=w[0][0];
		}
		else
		{
			estThresh=w[0][iter]+(add[iter]-sub[iter])*w[1][iter];
		}
		vthresh=(refp-refm)/double(pow(2,iter+2))+threshErr[iter]-estThresh;
		//cnvfile<<ainp-ainm<<endl;
		//cout<<ainp<<"\t\t\t"<<ainm<<"\t\t\t"<<ainp-ainm<<endl;
		if(ainp-ainm>vthresh)
		{
			sub[iter]=1;
		}
		else
		{
			sub[iter]=0;
		}
		if(ainp-ainm<-vthresh)
		{
			add[iter]=1;
		}
		else
		{
			add[iter]=0;
		}

		if(iter<5)
		{
			capPosition=iter;
		}
		else
		{
			capPosition=iter+1;
		}
		if(iter<10)
		{
			ainp=cdacP->changeBotPlateVoltage(add[iter]*refp+(1-add[iter])*refm,2*capPosition);
			ainp=cdacP->changeBotPlateVoltage(sub[iter]*refm+(1-sub[iter])*refp,2*capPosition+1);
			ainm=cdacM->changeBotPlateVoltage(add[iter]*refm+(1-add[iter])*refp,2*capPosition+1);
			ainm=cdacM->changeBotPlateVoltage(sub[iter]*refp+(1-sub[iter])*refm,2*capPosition);
		}
		else if(iter==10)
		{
			ainp=cdacP->changeBotPlateVoltage(add[iter]*refp+(1-add[iter])*refm,2*capPosition);
			ainp=cdacP->changeBotPlateVoltage(sub[iter]*refm+(1-sub[iter])*refp,2*capPosition+1);
			ainm=cdacM->getTopPlateVoltage();
		}
		if(iter>=7)
		{	
			errCo=0;
			double resErr;
			for(int iter1=0;iter1<5;iter1++)
			{
				errCo+=add[iter1]*ea[iter1]+(1-sub[iter1])*eb[iter1];
			}
			resErr=double(errCo-(errCo/4)*4)/4.0*(refp-refm)/2048;
			ainp-=resErr;
		}
	}

	for(iter=0;iter<12;iter++)
	{
		code+=pow(2,12-iter-1)*(add[iter]-sub[iter]);
	}
	code+=pow(2,12);
	code/=2;
	code+=errCo/4;

	int idealDec;
	idealDec=code>(2048+512)?1:0;
	if(add[0]!=idealDec && enableThreshCal==1)
	{
		w[0][0]+=double(code-(2048+512))/512.0;	
	}
	return code;
}

int adc::calibConvert(int bitUnderCal,int config)
{
	int iter;
	double ainp,ainm;
	int capPosition=0;
	double vthresh=0.0;
	int code=0;
	int calVal[4]={0,0,1,1};
	int bal[3]={0,0,0};
	int prcode;
	prcode=rand()%32-16;
	this->calibSample(prcode);
	switch(config)
	{
		case 1:
			calVal[0]=0;calVal[1]=0;calVal[2]=1;calVal[3]=1;break;
		case 2:
			calVal[0]=1;calVal[1]=0;calVal[2]=0;calVal[3]=1;break;
		case 3:
			calVal[0]=1;calVal[1]=1;calVal[2]=0;calVal[3]=0;break;
		case 4:
			calVal[0]=0;calVal[1]=1;calVal[2]=1;calVal[3]=0;break;
		case 5:
			calVal[0]=0;calVal[1]=1;calVal[2]=0;calVal[3]=1;break;
		case 6:
			calVal[0]=1;calVal[1]=0;calVal[2]=1;calVal[3]=0;break;
		case 7: 
			calVal[0]=0;calVal[1]=0;calVal[2]=1;calVal[3]=1;break;
	}

	for(iter=0;iter<bitUnderCal;iter++)
	{
		add[iter]=0;
		sub[iter]=0;
	}
	add[bitUnderCal]=calVal[0];
	sub[bitUnderCal]=1-calVal[2];
	for(iter=bitUnderCal+1;iter<5;iter++)
	{
		add[iter]=calVal[1];
		sub[iter]=1-calVal[3];
	}
	bal[0]=calVal[1];
	bal[2]=1-calVal[3];
	if(config==7)
	{
		bal[1]=1;
	}
	else
	{
		bal[1]=1-calVal[3];
	}
	for(iter=5;iter<12;iter++)
	{
		add[iter]=0;
		sub[iter]=0;
	}
	
	//cdacP->printBotstat();
	//cdacM->printBotstat();
		
	for(iter=0;iter<11;iter++)
	{
		if(iter<5)
		{
			capPosition=iter;
		}
		else
		{
			capPosition=iter+1;
		}
		ainp=cdacP->changeBotPlateVoltage(add[iter]*refp+(1-add[iter])*refm,2*capPosition);
		ainp=cdacP->changeBotPlateVoltage(sub[iter]*refm+(1-sub[iter])*refp,2*capPosition+1);
		ainm=cdacM->changeBotPlateVoltage(add[iter]*refm+(1-add[iter])*refp,2*capPosition+1);
		ainm=cdacM->changeBotPlateVoltage(sub[iter]*refp+(1-sub[iter])*refm,2*capPosition);
		//cout<<ainp<<"\t\t\t"<<ainm<<"\t\t\t"<<ainp-ainm<<endl;
	}
	//bal cap
	ainp=cdacP->changeBotPlateVoltage(bal[0]*refp+(1-bal[0])*refm,2*5);
	ainp=cdacP->changeBotPlateVoltage(bal[1]*refm+(1-bal[1])*refp,2*5+1);
	ainm=cdacM->changeBotPlateVoltage(bal[0]*refm+(1-bal[0])*refp,2*5+1);
	ainm=cdacM->changeBotPlateVoltage(bal[2]*refp+(1-bal[2])*refm,2*5);
	
	//cdacP->printBotstat();
	//cdacM->printBotstat();
	for(iter=5;iter<12;iter++)
	{
		vthresh=(refp-refm)/double(pow(2,iter+2))+threshErr[iter];
		//cout<<ainp<<"\t\t\t"<<ainm<<"\t\t\t"<<ainp-ainm<<endl;
		if(ainp-ainm>vthresh)
		{
			sub[iter]=1;
		}
		else
		{
			sub[iter]=0;
		}
		if(ainp-ainm<-vthresh)
		{
			add[iter]=1;
		}
		else
		{
			add[iter]=0;
		}

		if(iter<5)
		{	
			capPosition=iter;
		}
		else
		{
			capPosition=iter+1;
		}
		if(iter<10)
		{
			ainp=cdacP->changeBotPlateVoltage(add[iter]*refp+(1-add[iter])*refm,2*capPosition);
			ainp=cdacP->changeBotPlateVoltage(sub[iter]*refm+(1-sub[iter])*refp,2*capPosition+1);
			ainm=cdacM->changeBotPlateVoltage(add[iter]*refm+(1-add[iter])*refp,2*capPosition+1);
			ainm=cdacM->changeBotPlateVoltage(sub[iter]*refp+(1-sub[iter])*refm,2*capPosition);
		}
		else if(iter==10)
		{
			ainp=cdacP->changeBotPlateVoltage(add[iter]*refp+(1-add[iter])*refm,2*capPosition);
			ainp=cdacP->changeBotPlateVoltage(sub[iter]*refm+(1-sub[iter])*refp,2*capPosition+1);
			ainm=cdacM->getTopPlateVoltage();
		}
	}
	for(iter=0;iter<12;iter++)
	{
		code+=pow(2,12-iter-1)*(add[iter]-sub[iter]);

	}
	code+=pow(2,7)*(bal[0]-bal[1]);
	code+=pow(2,12);
	code/=2;
	return code;
}

double adc::residue(int bitPos)
{
	double residue=0.0;
	for(int iter=bitPos+1;iter<12;iter++)
	{
		residue+=(add[iter]-sub[iter])*(refp-refm)/double(pow(2,iter+1));
	}
	return residue;
}

void adc::autoCapCalib()
{
	for(int iter=5;iter>=0;iter--)
	{
		autoIndCapCalibDither(iter,1);
		autoIndCapCalibDither(iter,0);
	}
	for(int iter=0;iter<=5;iter++)
	{
		ea[iter]=(ea[iter]+512)/1024;
		eb[iter]=(eb[iter]+512)/1024;
		cout<<ea[iter]/4.0<<"\t"<<eb[iter]/4.0<<endl;
	}
	ebal[0]=(ebal[0]+512)/1024;
	ebal[1]=(ebal[1]+512)/1024;
	cout<<ebal[0]/4.0<<"\t"<<ebal[1]/4.0<<endl;
}

//type 0 => a type 1 => b 
void adc::autoIndCapCalibDither(int cap,int type)
{
	int code[2];
	int count;
	int e4,e2;
	int errAcc=0;
	string errfile;
	stringstream nums;
	errfile+="err";
	nums<<cap<<type;
	errfile+=nums.str();
	errfile+=".dat";
	ofstream eafile(errfile.c_str(),ios::out);
	
	for(count=0;count<4096;count++)
	{
		code[0]=calibConvert(cap,1);
		if(cap==5 && type==1)
		{
			code[1]=calibConvert(cap,7);
			errAcc+=code[1]-code[0]+32;
			eafile<<code[1]-code[0]+32<<endl;
		}
		else if(cap==5 && type==0)
		{
			code[1]=calibConvert(cap,4);
			errAcc+=(code[1]-code[0]);
			eafile<<(code[1]-code[0])<<endl;
		}
		else if(type==0)
		{
			code[1]=calibConvert(cap,6);
			errAcc+=(code[1]-code[0]);
			eafile<<(code[1]-code[0])<<endl;
		}
		else
		{
			code[1]=calibConvert(cap,5);
			errAcc+=(code[1]-code[0]);
			eafile<<(code[1]-code[0])<<endl;
		}

	}

	if(cap==5 && type==1)
	{
		ebal[type]=errAcc;
	}
	else if(cap==5 && type==0)
	{
		ebal[type]=ebal[1]-errAcc;
	}
	else
	{
		if(type==0)
		{
			e4=ebal[1];
			for(int iter=cap+1;iter<12;iter++)
			{
				e4+=eb[iter];
			}
			ea[cap]=e4-errAcc;
		}
		else
		{	
			e2=ebal[0];
			for(int iter=cap+1;iter<12;iter++)
			{
				e2+=ea[iter];
			}
			eb[cap]=errAcc+e2;
		}
	}
}

double adc::threshEst(int bitPos)
{
	return w[0][bitPos];
}

void adc::setThreshCal(int enable)
{
	enableThreshCal=enable;
}

adc::~adc()
{
	delete cdacP;
	delete cdacM;
}
		






