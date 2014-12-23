
#ifndef _F7_H
#define _F7_H

#include "Benchmarks.h"

class F7:public Benchmarks{
protected:
public:
	F7();
	double compute(double* x) ;
	~F7();
};

#endif
