#ifndef NLVCO_H
#define NLVCO_H
#include <cmath>
using namespace std;
class nlvco
{
	private:
		double t0,tau;
		double gain;
		double vThresh;
	public:
		nlvco();
		nlvco(double t0,double tau,double gain,double vThresh);
		double getKEdge(int k,double vControl);
};
#endif
