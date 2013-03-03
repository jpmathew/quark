#include "nlvsar.h"
#include <iostream>
using namespace std;

int main()
{
	nlvsar *adc = new nlvsar(0.9);
	adc->convert(-0.0125,0.0125);
	return 1;
}
