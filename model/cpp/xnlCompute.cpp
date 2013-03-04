#include "xnlCompute.h"
#include <string>

xnlCompute::xnlCompute(int resolution)
{
	numCode=pow(2,resolution);
	codeHits=new int [numCode];
}

xnlCompute::~xnlCompute()
{
	delete [] codeHits;
}

void xnlCompute::computeLin(const char *fileName,const char *resName)
{
	string dnlFile,inlFile;
	dnlFile+="dnl";
	dnlFile+=resName;
	dnlFile+=".dat";
	inlFile+="inl";
	inlFile+=resName;
	inlFile+=".dat";
	codeData.open(fileName,ios::in);
	dnlData.open(dnlFile.c_str(),ios::out);
	inlData.open(inlFile.c_str(),ios::out);

	for(code=0;code<numCode;code++)
	{
		codeHits[code]=0;
	}
	while(!codeData.eof())
	{
		codeData>>code;
		codeHits[code]++;
	}
	codeData.close();


	//get codeHits
	minCode=0;
	maxCode=numCode-1;
	for(code=0;code<numCode;code++)
	{
		if(codeHits[code]!=0)
		{
			break;
		}
		else
		{
			minCode=code;
		}
	}

	for(code=numCode-1;code>0;code--)
	{
		if(codeHits[code]!=0)
		{
			break;
		}
		else
		{
			maxCode=code;
		}
	}
	
	//cout<<minCode<<"\t"<<maxCode<<endl;
	totalHit=0.0;
	for(code=minCode+5;code<=maxCode-5;code++)
	{
		totalHit+=codeHits[code];
		if(code >= 505 && code <= 516)
		{
			//cout<<codeHits[code]<<endl;
		}
	}
	//linearity computation Begins
	avgHit=totalHit/double(maxCode-minCode-9);
	//cout<<avgHit<<endl;
	for(code=0;code<minCode+5;code++)
	{
		dnlData<<0.0<<endl;
		inlData<<0.0<<endl;
	}

	inl=0.0;
	dnl=0.0;
	for(code=minCode+5;code<=maxCode-5;code++)
	{
		//cout<<codeHits[code]<<endl;
		dnl=double(codeHits[code]-avgHit)/avgHit;
		inl+=dnl;
		dnlData<<dnl<<endl;
		inlData<<inl<<endl;
	}

	for(code=maxCode-4;code<numCode;code++)
	{
		dnlData<<0.0<<endl;
		inlData<<0.0<<endl;
	}
	//lineraity computation ends

	dnlData.close();
	inlData.close();

}




	



