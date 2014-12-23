#ifndef _F11_H
#define _F11_H

#include "Benchmarks.h"

class F11:public Benchmarks{
protected:
public:
	F11();
	double compute(double* x) ;
	~F11();
};

#endif
