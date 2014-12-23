#include "F15.h"
#include <stdio.h>

F15::F15():Benchmarks(){
  Ovector = NULL;
  minX = -100;
  maxX = 100;
  ID = 15;
  anotherz = new double[dimension];
}

F15::~F15(){
  delete[] Ovector;
  delete[] anotherz;
}

double F15::compute(double*x){
  int i;
  double result=0.0;

  if(Ovector==NULL)
    {
      Ovector = readOvector();
    }

  for(i=0;i<dimension;i++)
    {
      anotherz[i]=x[i]-Ovector[i];
    }

  result=schwefel(anotherz, dimension);

  return(result);
}

