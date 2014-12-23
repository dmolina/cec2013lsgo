#ifndef _F14_H
#define _F14_H

#include "Benchmarks.h"

class F14:public Benchmarks{
 public:
  F14();
  double compute(double* x) ;
  ~F14();
};

#endif

