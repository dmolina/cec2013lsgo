#ifndef _F5_H
#define _F5_H

#include "Benchmarks.h"

class F5:public Benchmarks{
protected:
public:
	F5();
	double compute(double* x) ;
	/* double compute(vector<double> x) ; */
	~F5();
};

#endif
