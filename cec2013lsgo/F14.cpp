#include "F14.h"
#include <stdio.h>

F14::F14():Benchmarks(){
  OvectorVec = NULL;
  minX = -100;
  maxX = 100;
  ID = 14;
  s_size = 20;
  dimension = 905;        // because of overlapping
  overlap = 5;
}

F14::~F14(){
  for (int i = 0; i < s_size; i++)
    {
      free(OvectorVec[i]);
    }
  free(OvectorVec);
  
  delete[] Pvector;
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

double F14::compute(double*x){
  int i;
  double result=0.0;
  
  if(OvectorVec == NULL)
    {
      s = readS(s_size);
      OvectorVec = readOvectorVec();
      // // inspect OvectorVec
      // for (int i = 0; i < s_size; ++i)
      //   {
      //     for (int j=0; j< s[i]; j++)
      //       {
      //         printf("%.1f\t",OvectorVec[i][j]);
      //       }
      //     printf("\n");
      //   }
      Pvector = readPermVector();
      r25 = readR(25);
      r50 = readR(50);
      r100 = readR(100);
      w = readW(s_size);
    }

  // s_size non-separable part with rotation
  int c = 0;
  for (i = 0; i < s_size; i++)
    {
      // cout<<"c="<<c<<", i="<<i<<endl;
      anotherz1 = rotateVectorConflict(i, c, x);
      // cout<<"done rot"<<endl;
      result += w[i] * schwefel(anotherz1, s[i]);
      delete []anotherz1;
      // cout<<result<<endl;
    }
  
  return(result);
}
