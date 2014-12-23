#include "F13.h"
#include <stdio.h>

F13::F13():Benchmarks(){
  m_havenextGaussian=0;
  Ovector = NULL;
  minX = -100;
  maxX = 100;
  ID = 13;
  s_size = 20;
  dimension = 905;        // because of overlapping
  overlap = 5;
  anotherz = new double[dimension];
}

F13::~F13(){
  delete[] Ovector;
  delete[] Pvector;
  delete[] anotherz;
  for (int i = 0; i < 25; ++i)
    {
      delete[] r25[i];
    }
  for (int i = 0; i < 50; ++i)
    {
      delete[] r50[i];
    }
  for (int i = 0; i < 100; ++i)
    {
      delete[] r100[i];
    }
  delete[] r25;
  delete[] r50;
  delete[] r100;
  delete[] s;
  delete[] w;

}

double F13::compute(double*x){
  int i;
  double result=0.0;
        
  if(Ovector==NULL)
    {
      Ovector = readOvector();
      Pvector = readPermVector();
      r25 = readR(25);
      r50 = readR(50);
      r100 = readR(100);
      s = readS(s_size);
      w = readW(s_size);
    }
  
  for(i=0;i<dimension;i++)
    {
      anotherz[i]=x[i]-Ovector[i];
    }
  // cout<<"o"<<endl;

  // s_size non-separable part with rotation
  int c = 0;
  for (i = 0; i < s_size; i++)
    {
      // cout<<"c="<<c<<", i="<<i<<endl;
      anotherz1 = rotateVectorConform(i, c);
      // cout<<"done rot"<<endl;
      result += w[i] * schwefel(anotherz1, s[i]);
      delete []anotherz1;
      // cout<<result<<endl;
    }
  
  return(result);
}

