#include "F4.h"
#include <stdio.h>

F4::F4():Benchmarks(){
	Ovector = NULL;
	minX = -100;
	maxX = 100;
	ID = 4;
        s_size = 7;
	// lookup2 = lookupprepare(nonSeparableGroupSize);
	// lookup = lookupprepare(dimension - nonSeparableGroupSize);
        anotherz = new double[dimension];
}

F4::~F4(){
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

double F4::compute(double*x){
  int    i;
  double result = 0.0;

  if(Ovector == NULL) {
    Ovector = readOvector();
    Pvector = readPermVector();
    r25 = readR(25);
    r50 = readR(50);
    r100 = readR(100);
    s = readS(s_size);
    w = readW(s_size);
  }
  
  for(i = 0; i < dimension; i++) {
    anotherz[i] = x[i] - Ovector[i];
  }

  // for (int i = 0; i < dimension; ++i)
  //   {
  //     cout<<anotherz[i]<<endl;
  //   }
  // cout<<endl;

  // // T_{osz}
  // transform_osz(anotherz);
  
  // s_size non-separable part with rotation
  int c = 0;
  for (i = 0; i < s_size; i++)
    {
      // cout<<"c="<<c<<", i="<<i<<endl;
      anotherz1 = rotateVector(i, c);
      // cout<<"done rot"<<endl;
      result += w[i] * elliptic(anotherz1, s[i]);
      delete []anotherz1;
      // cout<<result<<endl;
    }
  
  // one separable part without rotation
  double* z = new double[dimension-c];
  for (i = c; i < dimension; i++)
    {
      // cout<<i-c<<" "<<Pvector[i]<<" "<<anotherz[Pvector[i]]<<endl;
      z[i-c] = anotherz[Pvector[i]];
    }
  
  // cout<<"sep\n"<<elliptic(z, dimension-c)<<endl;
  
  result += elliptic(z, dimension-c);
  delete[] z;

//  printf("Rotated Part = %1.16E\n", rot_elliptic(anotherz1,nonSeparableGroupSize) * 1e6);
//  printf("Separable Part = %1.16E\n", elliptic(anotherz2,dimension - nonSeparableGroupSize));

  return(result);
}

// double F4::compute(vector<double> x){
//   int    i;
//   double result = 0.0;

//   if(Ovector == NULL) {
//     Ovector   = createShiftVector(dimension,minX,maxX);
// 	/*
// 	printf("\n\n\nO vector\n\n\n");
// 	for (i = 0; i<dimension; i++){
// 		printf("%lf\t",Ovector[i]);
// 	}
// 	*/

// 	Pvector   = createPermVector(dimension);
// 	/*
// 	printf("\n\n\nP vector\n\n\n");
// 	for (i = 0; i<dimension; i++){
// 		printf("%d\t",Pvector[i]);
// 	}
// 	*/
// /*
// 	  Pvector = (int*)malloc(sizeof(int) * dimension);
// 	  for (i = 0; i<dimension; i++){
// 		  Pvector[i] = i;	
// 	  }
// */
//     RotMatrix = createRotMatrix1D(nonSeparableGroupSize);
// 	/*
// 	printf("\n\n\nRot Matrix\n\n\n");
// 	for (i = 0; i<nonSeparableGroupSize; i++){
// 		for (j=0; j<nonSeparableGroupSize; j++){
// 			printf("%lf\t",RotMatrix[i*nonSeparableGroupSize+j]);
// 		}
// 		printf("\n");
// 	}
// 	*/
//   }

//   for(i = 0; i < dimension; i++) {
//     anotherz[i] = x[i] - Ovector[i];
//   }

//   for(i = 0; i < nonSeparableGroupSize; i++) {
//     anotherz1[i] = anotherz[Pvector[i]];
//   }

//   for(i = nonSeparableGroupSize; i < dimension; i++) {
//     anotherz2[i - nonSeparableGroupSize] = anotherz[Pvector[i]];
//   }

//   result = rot_elliptic(anotherz1,nonSeparableGroupSize) * 1e6 + elliptic(
//     anotherz2,dimension - nonSeparableGroupSize);

// //  printf("Rotated Part = %1.16E\n", rot_elliptic(anotherz1,nonSeparableGroupSize) * 1e6);
// //  printf("Separable Part = %1.16E\n", elliptic(anotherz2,dimension - nonSeparableGroupSize));

//   return(result);
// }
