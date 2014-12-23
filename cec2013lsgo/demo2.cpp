#include "eval_func.h"
#include <vector>
#include <sys/time.h>
#include <cstdio>
#include <unistd.h>

using std::vector;

int main(){
  /*  Test the basic benchmark function */
  double* X;
  unsigned dim = 1000;
  unsigned funToRun[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  // unsigned funToRun[] = {1};
  // unsigned funToRun[] = {15};
  unsigned funNum = 15;
  unsigned run = 1;

  vector<double> runTimeVec;
  struct timeval start, end;
  long seconds, useconds;    
  double mtime;
        
  X = new double[dim];
  for (unsigned i=0; i<dim; i++){
    X[i]=0;
  }

  for (unsigned i=0; i<funNum; i++){
     unsigned int funID = funToRun[i];
    set_func(funID); 
    printf("F %d value = %1.20E\n", funID, eval_sol(X));
    gettimeofday(&start, NULL);
    for (unsigned j=0; j < run; j++){
      eval_sol(X);
    }
    gettimeofday(&end, NULL);
    
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = (((seconds) * 1000 + useconds/1000.0) + 0.5)/1000;

    runTimeVec.push_back(mtime);
    printf ( "F %d, Running Time = %f s\n\n", funID, mtime);
    
    free_func();
  }

  delete []X;

  // for (unsigned i=0; i<runTimeVec.size(); i++){
  // 	printf ( "%f\n", runTimeVec[i] );
  // }

  return 0;
}
