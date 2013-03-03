#include "nlvco.h"

nlvco::nlvco()
{
	t0=50;
	tau=8;
	gain=4;
	vThresh=0.45;
}
nlvco::nlvco(double t0,double tau,double gain,double vThresh)
{
	this->t0=t0;
	this->tau=tau;
	this->gain=gain;
	this->vThresh=vThresh;
}
double nlvco::getKEdge(int K,double vControl)
{
	return K*(t0-tau*log(gain*abs(vControl)/vThresh));
}


