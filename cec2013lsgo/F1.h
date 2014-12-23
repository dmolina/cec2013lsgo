#ifndef _F1_H
#define _F1_H

#include "Benchmarks.h"

class F1:public Benchmarks{
public:
	F1();
	double compute(double* x) ;
	/* double compute(vector<double> x) ; */
	~F1();
};
#endif
