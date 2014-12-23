#ifndef _F8_H
#define _F8_H

#include "Benchmarks.h"

class F8:public Benchmarks{
protected:
public:
	F8();
	double compute(double* x) ;
	~F8();
};

#endif
