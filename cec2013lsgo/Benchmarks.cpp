#include "Benchmarks.h"

Benchmarks::Benchmarks(){
  dimension = 1000;		
  nonSeparableGroupSize = 50;
  MASK = ((L(1)) << (L(48))) - (L(1));
  m_havenextGaussian = false;

  if (dimension<nonSeparableGroupSize){
    cerr<<"ERROR: In configuration file, dimension is smaller than the non-separable part"<<endl;
    exit(-1);
  }

  // // allocate the memory
  // anotherz = new double[dimension];
  // anotherz1= new double[nonSeparableGroupSize];
  // anotherz2= new double[dimension - nonSeparableGroupSize];

  // Runtime Parameters setting
  setOvectorToZero = false;

  // functionInitRandomSeed = L(0);
  // m_seed= functionInitRandomSeed;
  // M  = 0x5DEECE66D;
  // A  = 0xB;

  // minX = -100;
  // maxX = 100;
}

void Benchmarks::setMinX(int inVal){
  minX = inVal;	
}
void Benchmarks::setMaxX(int inVal){
  maxX = inVal;
}

void Benchmarks::setSeed(int64_t inVal){
  functionInitRandomSeed = inVal;
}

void Benchmarks::setDimension(int inVal){
  dimension = inVal;
}

void Benchmarks::setNonSeparableGroupSize(int inVal){
  nonSeparableGroupSize = inVal;
}

Benchmarks::~Benchmarks(){
  // delete[] anotherz;
  // delete[] anotherz1;
  // delete[] anotherz2;
}

int Benchmarks::next(int bits) {
  int64_t s;
  int64_t result;
  m_seed = s = (((m_seed * M) + A) & MASK);
  result = (s >> (L(48 - bits)));
  return((int)result);
}

int Benchmarks::nextInt(int n) {
  int bits, val;

  if ((n & (-n)) == n) {
    return((int) ((n * L(next(31))) >> L(31)));
  }

  do {
    bits = next(31);
    val  = bits % n;
  } while (bits - val + (n - 1) < 0);

  return(val);
}

double Benchmarks::nextDouble(){
  return ((((L(next(26))) <<(L(27))) + (L(next(27)))) / (double) ((L(1)) << (L(53))));
}

double Benchmarks::nextGaussian(){
  double multiplier, v1, v2, s;

  if (m_havenextGaussian) {
    m_havenextGaussian = false;
    return(m_nextGaussian) ;
  }

  do {
    v1 = ((D(2.0) * nextDouble()) - D(1.0));
    v2 = ((D(2.0) * nextDouble()) - D(1.0));
    s  = ((v1 * v1) + (v2 * v2));
  } while ((s >= D(1.0)) || (s <= D(0.0)));

  multiplier = sqrt(D(-2.0) * log(s) / s);
  m_nextGaussian    = (v2 * multiplier);
  m_havenextGaussian = true;
  return (v1 * multiplier);
}

double* Benchmarks::createShiftVector(int dim, double min,double max) {

  double* d;
  double  hw, middle;
  double  s;
  int     i;
  hw     = (D(0.5) * (max - min));
  middle = (min + hw);
  //d      = (double*)malloc(sizeof(double) * dim);
  d      = new double[dim];

  //	printf("middle = %1.20E\n",middle);
  //	printf("hw = %1.20E\n",hw);

  for (i = (dim - 1); i >= 0; i--) {
    if (setOvectorToZero == true){
      d[i] = 0;
    }else{
      do {
        double tempGauss = nextGaussian();
        s = (middle + (tempGauss * hw));
      } while ((s < min) || (s > max));
      d[i] = s;
      //			printf("shift vector = %1.20E\n", d[i]);
    }
  }
  return(d);
}

double* Benchmarks::readOvector()
{
  // read O vector from file in csv format
  double* d = new double[dimension];
  stringstream ss;
  ss<< "cdatafiles/" << "F" << ID << "-xopt.txt";
  ifstream file (ss.str());
  string value;
  string line;
  int c=0;
  
  if (file.is_open())
    {
      stringstream iss;
      while ( getline(file, line) )
        {
          iss<<line;
          while (getline(iss, value, ','))
            {
              d[c++] = stod(value);
            }
          iss.clear();
          // if (c==dimension)
          //   {
          //     break;
          //   }
          // printf("%d\n",c);
        }
      file.close();
    }
  else
    {
      cout<<"Cannot open datafiles"<<endl;
    }
  return d;
}

double** Benchmarks::readOvectorVec()
{
  // read O vector from file in csv format, seperated by s_size groups
  double** d = (double**) malloc(s_size*sizeof(double*));
  stringstream ss;
  ss<< "cdatafiles/" << "F" << ID << "-xopt.txt";
  ifstream file (ss.str());
  string value;
  string line;
  int c = 0;                      // index over 1 to dim
  int i = -1;                      // index over 1 to s_size
  int up = 0;                   // current upper bound for one group
  
  if (file.is_open())
    {
      stringstream iss;
      while ( getline(file, line) )
        {
          if (c==up)             // out (start) of one group
            {
              // printf("=\n");
              i++;
              d[i] =  (double*) malloc(s[i]*sizeof(double));
              up += s[i];
            }
          iss<<line;
          while (getline(iss, value, ','))
            {
              // printf("c=%d\ts=%d\ti=%d\tup=%d\tindex=%d\n",c,s[i],i,up,c-(up-s[i]));
              d[i][c-(up-s[i])] = stod(value);
              // printf("1\n");
              c++;
            }
          iss.clear();
          // printf("2\n");
        }
      file.close();
    }
  else
    {
      cout<<"Cannot open datafiles"<<endl;
    }
  return d;  
}


void Benchmarks::transform_osz(double* z, int dim)
{
  // apply osz transformation to z
  for (int i = 0; i < dim; ++i)
    {
      z[i] = sign(z[i]) * exp( hat(z[i]) + 0.049 * ( sin( c1(z[i]) * hat(z[i]) ) + sin( c2(z[i])* hat(z[i]) )  ) ) ;
    }
}

void Benchmarks::transform_asy(double* z, double beta, int dim)
{
  for (int i = 0; i < dim; ++i)
    {
      if (z[i]>0)
        {
          z[i] = pow(z[i], 1 + beta * i/((double) (dim-1)) * sqrt(z[i]) );
        }
    }
}

void Benchmarks::Lambda(double* z, double alpha, int dim)
{
  for (int i = 0; i < dim; ++i)
    {
      z[i] = z[i] * pow(alpha, 0.5 * i/((double) (dim-1)) );
    }
}


int* Benchmarks::createPermVector(int dim){
  int* d;
  int  i, j, k, t;
  //d = (int*)malloc(sizeof(int) * dim);
  d = new int[dim];

  for (i = (dim - 1); i >= 0; i--) {
    d[i] = i;
  }

  for (i = (dim << 3); i >= 0; i--) {
    j = nextInt(dim);

    do {
      k = nextInt(dim);
    } while (k == j);

    t    = d[j];
    d[j] = d[k];
    d[k] = t;
  }

  return(d);
}

int* Benchmarks::readPermVector(){
  int* d;

  d = new int[dimension];

  stringstream ss;
  ss<< "cdatafiles/" << "F" << ID << "-p.txt";
  ifstream file (ss.str());
  int c=0;
  string value;

  if (file.is_open())
    {
      while (getline(file,value,','))
        {
          d[c++] = stod(value) - 1;
        }
    }

  // for (int i = 0; i < dimension; ++i)
  //   {
  //     printf("%d\n", d[i]);
  //   }

  return(d);
}




//Create a random rotation matrix
double** Benchmarks::createRotMatrix(int dim){
  double** m;
  int      i, j, k;
  double   dp, t;
  //m = (double**)malloc(sizeof(double*) * dim);
  m = new double*[dim];

  for(i = 0; i < dim; i++) {
    //m[i] = (double*)malloc(sizeof(double) * dim);
    m[i] = new double[dim];
  }

 loop:
  for (;;) {
    for (i = (dim - 1); i >= 0; i--) {
      for (j = (dim - 1); j >= 0; j--) {
        m[i][j] = nextGaussian();
      }
    }

    // main loop of gram/schmidt
    for (i = (dim - 1); i >= 0; i--) {
      for (j = (dim - 1); j > i; j--) {
        // dot product
        dp = 0;

        for (k = (dim - 1); k >= 0; k--) {
          dp += (m[i][k] * m[j][k]);
        }

        // subtract
        for (k = (dim - 1); k >= 0; k--) {
          m[i][k] -= (dp * m[j][k]);
        }
      }

      // normalize
      dp = 0;

      for (k = (dim - 1); k >= 0; k--) {
        t   = m[i][k];
        dp += (t * t);
      }

      // linear dependency -> restart
      if (dp <= 0) {
        goto loop;
      }

      dp = (1 / sqrt(dp));

      for (k = (dim - 1); k >= 0; k--) {
        m[i][k] *= dp;
      }
    }

    return(m) ;
  }
}


/**
 * Create a random rotation matrix
 */
double* Benchmarks::createRotMatrix1D(int dim){
  double** a;
  double*  b;
  int      i, j, k;
  a = createRotMatrix(dim);
  //b = (double*)malloc(sizeof(double) * (dim * dim));
  b = new double[dim*dim];
  k = 0;

  for (i = 0; i < dim; i++) {
    for (j = 0; j < dim; j++) {
      b[k++] = a[i][j];
    }
  }

  // release the memory that previously allocated for storing a 2D array
  for(i=0;i<dim;i++){
    delete []a[i];
  }
  delete[] a;

  return(b);
}

/*
 * create several 1-D rotation matrix randomly
 */
double** Benchmarks::createMultiRotateMatrix1D(int dim, int num){
  double** a;
  int i;

  /*  allocate storage for an array of pointers */
  //a =(double **) malloc(num * sizeof(double *));
  a = new double*[num];

  /* for each pointer, allocate storage for an array of ints */
  for (i = 0; i < num; i++) {
    a[i] = createRotMatrix1D(dim);
  }

  return (a);
}

double** Benchmarks::readR(int sub_dim)
{
  double** m;

  m = new double*[sub_dim];
  for (int i = 0; i< sub_dim; i++)
    {
      m[i] = new double[sub_dim];
    }

  stringstream ss;
  ss<< "cdatafiles/" << "F" << ID << "-R"<<sub_dim<<".txt";
  // cout<<ss.str()<<endl;

  ifstream file (ss.str());
  string value;
  string line;
  int i=0;
  int j;

  if (file.is_open())
    {
      stringstream iss;
      while ( getline(file, line) )
        {
          j=0;
          iss<<line;
          while (getline(iss, value, ','))
            {
              // printf("%d,%d\t%f\n", i,j, stod(value));
              m[i][j] = stod(value);
              // printf("done\n");
              j++;
            }
          iss.clear();
          i++;
        }
      file.close();
    }
  else
    {
      cout<<"Cannot open datafiles"<<endl;
    }
  return m;
}

int* Benchmarks::readS(int num)
{
  s = new int[num];

  stringstream ss;
  ss<< "cdatafiles/" << "F" << ID << "-s.txt";
  ifstream file (ss.str());
  int c=0;
  string value;
  if (file.is_open())
    {
      while (getline(file,value))
        {
          // cout<<stod(value)<<endl;
          s[c++] = stod(value);
        }
    }
  return s;
}

double* Benchmarks::readW(int num)
{
  w = new double[num];

  stringstream ss;
  ss<< "cdatafiles/" << "F" << ID << "-w.txt";
  ifstream file (ss.str());
  int c=0;
  string value;
  if (file.is_open())
    {
      while (getline(file,value))
        {
          // cout<<stod(value)<<endl;
          w[c++] = stod(value);
        }
    }

  return w;
}

double* Benchmarks::rotateVector(int i, int &c)
{
  double* z = new double[s[i]];
  // cout<<"s "<<s[i]<<endl;

  // copy values into the new vector
  for (int j = c; j < c+s[i]; ++j)
    {
      // cout<<"j-c "<<j-c<<" p "<<Pvector[j]<<endl;
      z[j-c] = anotherz[Pvector[j]];
    }
  // cout<<"copy done"<<endl;

  if (s[i]==25)
    {
      anotherz1 = multiply( z, r25, s[i]);
    }
  else if (s[i] == 50)
    {    
      anotherz1 = multiply( z, r50, s[i]);
    }
  else if (s[i] == 100) 
    {
      anotherz1 = multiply( z, r100, s[i]);
    }
  else
    {
      cout<< "size of rotation matrix out of range" <<endl;
    }
  delete []z;
  c = c + s[i];
  return anotherz1;
}

double* Benchmarks::rotateVectorConform(int i, int &c)
{
  double* z = new double[s[i]];
  // printf("i=%d\tc=%d\tl=%d\tu=%d\n", i, c, c - (i)*overlap, c +s[i] - (i)*overlap);
  
  // copy values into the new vector
  for (int j = c - i*overlap; j < c +s[i] - i*overlap; ++j)
    {
      // cout<<"j-c "<<j-c<<" p "<<Pvector[j]<<endl;
      z[j-(c - i*overlap)] = anotherz[Pvector[j]];
    }
  // cout<<"copy done"<<endl;
  if (s[i]==25)
    {
      anotherz1 = multiply( z, r25, s[i]);
    }
  else if (s[i] == 50)
    {    
      anotherz1 = multiply( z, r50, s[i]);
    }
  else if (s[i] == 100) 
    {
      anotherz1 = multiply( z, r100, s[i]);
    }
  else
    {
      cout<< "size of rotation matrix out of range" <<endl;
    }
  delete []z;
  c = c + s[i];
  return anotherz1;
}

double* Benchmarks::rotateVectorConflict(int i, int &c, double* x)
{
  double* z = new double[s[i]];
  // printf("i=%d\tc=%d\tl=%d\tu=%d\n", i, c, c - (i)*overlap, c +s[i] - (i)*overlap);
  
  // copy values into the new vector
  for (int j = c - i*overlap; j < c +s[i] - i*overlap; ++j)
    {
      // cout<<"j-c "<<j-c<<" p "<<Pvector[j]<<endl;
      z[j-(c - i*overlap)] = x[Pvector[j]] - OvectorVec[i][j-(c - i*overlap)];  
    }

  // cout<<"copy done"<<endl;
  if (s[i]==25)
    {
      anotherz1 = multiply( z, r25, s[i]);
    }
  else if (s[i] == 50)
    {    
      anotherz1 = multiply( z, r50, s[i]);
    }
  else if (s[i] == 100) 
    {
      anotherz1 = multiply( z, r100, s[i]);
    }
  else
    {
      cout<< "size of rotation matrix out of range" <<endl;
    }
  delete []z;
  c = c + s[i];
  return anotherz1;
}


// double* Benchmarks::lookupprepare(int dim) {
//   double pownum;
//   int    i;
//   double* lookup;
//   i         = (dim - 1);
//   pownum    = (1.0 / i);
//   //lookup    = (double*)malloc(dim * sizeof(double));
//   lookup    = new double[dim];
//   lookup[i] = 1.0e6;
//   lookup[0] = 1.0;

//   for (--i; i > 0; i--) {
//     lookup[i] = pow(1.0e6, i * pownum);
//   }
//   return lookup;
// }



/* 
 * Basic Mathematical Functions' Implementation
 */
// // elliptic function for F1 ~ F8
// double Benchmarks::elliptic(double*x,int dim) {
//   double result = 0.0;
//   int    i;

//   for(i = dim - 1; i >= 0; i--) {
//     if (dim == nonSeparableGroupSize){
//       result += lookup2[i] * x[i] * x[i];
//     }else{
//       result += lookup[i] * x[i] * x[i];
//     }
//   }
//   return(result);
// }

double Benchmarks::elliptic(double*x,int dim) {
  double result = 0.0;
  int    i;
  
  transform_osz(x, dim);

  // for(i = dim - 1; i >= 0; i--) {
  for(i=0; i<dim; i++)
    {
      // printf("%f\n", pow(1.0e6,  i/((double)(dim - 1)) ));
      result += pow(1.0e6,  i/((double)(dim - 1)) ) * x[i] * x[i];
    }
  
  return(result);
}


unsigned Benchmarks::getID(){
  return ID;
}

// // elliptic function for F9 ~ 
// double Benchmarks::elliptic(double*x, int dim, int k) {
//   double result = 0.0;
//   int    i;

//   for(i=dim/k-1;i>=0;i--)
//     {
//       result+=lookup2[i]*x[Pvector[dim/k+i]]*x[Pvector[dim/k+i]];
//     }

//   return(result);
// }

// rastrigin function for F1~F8
double Benchmarks::rastrigin(double*x,int dim){
  double sum = 0;
  int    i;
  
  // T_{osz}
  transform_osz(x, dim);
  
  // T_{asy}^{0.2}
  transform_asy(x, 0.2, dim);

  // lambda
  Lambda(x, 10, dim);

  for(i = dim - 1; i >= 0; i--) {
    sum += x[i] * x[i] - 10.0 * cos(2 * PI * x[i]) + 10.0;
  }
  
  return(sum);
}

// rastrigin function for F9 ~
double Benchmarks::rastrigin(double *x, int dim, int k)
{
  double result=0.0;
  int i;
  for(i=dim/k-1;i>=0;i--)
    {
      result+=x[Pvector[dim/k+i]]*x[Pvector[dim/k+i]]-10.0*cos(2*PI*x[Pvector[dim/k+i]])+10.0;
    }
  return(result);
}

// ackley function for single group non-separable 
double Benchmarks::ackley(double*x,int dim){
  double sum1 = 0.0;
  double sum2 = 0.0;
  double sum;
  int    i;

  // T_{osz}
  transform_osz(x,dim);
  
  // T_{asy}^{0.2}
  transform_asy(x, 0.2, dim);

  // lambda
  Lambda(x, 10, dim);

  for(i = dim - 1; i >= 0; i--) {
    sum1 += (x[i] * x[i]);
    sum2 += cos(2.0 * PI * x[i]);
  }

  sum = -20.0 * exp(-0.2 * sqrt(sum1 / dim)) - exp(sum2 / dim) + 20.0 + E;
  return(sum);
}

// ackley function for m-group non-separable 
double Benchmarks::ackley(double *x, int dim, int k)
{
  double sum1=0.0;
  double sum2=0.0;
  double result=0.0;
  int i;

  for(i=dim/k-1;i>=0;i--)
    {
      sum1+=x[Pvector[dim/k+i]]*x[Pvector[dim/k+i]];
      sum2+=cos(2.0*PI*x[Pvector[dim/k+i]]);
    }

  result=-20.0*exp(-0.2*sqrt(sum1/(dim/k)))-exp(sum2/(dim/k))+20.0+E;

  return(result);
}

double* Benchmarks::multiply(double*vector, double*matrix,int dim){
  int    i,j;
  //double*result = (double*)malloc(sizeof(double) * dim);
  double*result = new double[dim];

  for(i = dim - 1; i >= 0; i--) {
    result[i] = 0;

    for(j = dim - 1; j >= 0; j--) {
      result[i] += vector[j] * matrix[dim * j + i];
    }
  }

  return(result);
}

double* Benchmarks::multiply(double*vector, double**matrix,int dim){
  int    i,j;
  //double*result = (double*)malloc(sizeof(double) * dim);
  double*result = new double[dim];

  for(i = dim - 1; i >= 0; i--) {
    result[i] = 0;

    for(j = dim - 1; j >= 0; j--) {
      result[i] += vector[j] * matrix[i][j];
    }
  }

  return(result);
}


// // Rotated Elliptic Function for F1 & F4
// double Benchmarks::rot_elliptic(double*x,int dim){
//   double result = 0.0;
//   double *z = multiply(x,RotMatrix,dim);

//   result = elliptic(z,dim);

//   delete[] z;
//   return(result);
// }

// // Rotated Elliptic Function for F9 & F14
// double Benchmarks::rot_elliptic(double*x,int dim, int k){
//   double result=0.0;

//   int i,j;
//   for(i=dim-1;i>=0;i--)
//     {
//       anotherz1[i]=0;
//       for(j=dim-1;j>=0;j--)
//         {
//           anotherz1[i]+=x[Pvector[(k-1)*dim+j]]*RotMatrix[dim*j+i];
//         }
//     }
//   for(i=dim-1;i>=0;i--)
//     {
//       result+=lookup[i]*anotherz1[i]*anotherz1[i];
//     }
//   return(result);
// }

// Rotated Rastrigin Function for F1~F8
double Benchmarks::rot_rastrigin(double*x,int dim){
  double result = 0.0;
  double*z      = multiply(x,RotMatrix,dim);
  result = rastrigin(z,dim);

  delete[] z;
  return(result);
}

// Rotated Rastrigin Function for F9 ~
double Benchmarks::rot_rastrigin(double *x,int dim,int k)
{
  double result=0.0;

  int i,j;
  for(i=dim-1;i>=0;i--)
    {
      anotherz1[i]=0;
      for(j=dim-1;j>=0;j--)
        {
          anotherz1[i]+=x[Pvector[(k-1)*dim+j]]*RotMatrix[dim*j+i];
        }
    }

  for(i=dim-1;i>=0;i--)
    {
      result+=anotherz1[i]*anotherz1[i]-10.0*cos(2*PI*anotherz1[i])+10.0;
    }
  return(result);
}

// rotated ackley function for single group non-separable
double Benchmarks::rot_ackley(double*x,int dim){
  double result = 0.0;
  double*z      = multiply(x,RotMatrix,dim);
  result = ackley(z,dim);
  delete[] z;
  return(result);
}

// rotated ackley function for m group non-separable
double Benchmarks::rot_ackley(double *x,int dim,int k)
{
  double result=0.0;
  double sum1=0.0;
  double sum2=0.0;

  int i,j;
  for(i=dim-1;i>=0;i--)
    {
      anotherz1[i]=0;
      for(j=dim-1;j>=0;j--)
        {
          anotherz1[i]+=x[Pvector[(k-1)*dim+j]]*RotMatrix[dim*j+i];
        }
    }
  for(i=dim-1;i>=0;i--)
    {
      sum1+=anotherz1[i]*anotherz1[i];
      sum2+=cos(2.0*PI*anotherz1[i]);
    }
  result=-20.0*exp(-0.2*sqrt(sum1/dim))-exp(sum2/dim)+20.0+E;
  return(result);
}

// for single group non-separable function
double Benchmarks::sphere(double*x,int dim){
  double sum = 0;
  int    i;

  for(i = dim - 1; i >= 0; i--) {
    sum += pow(x[i],2);
  }

  return(sum);
}

// for m groups non-separable function
double Benchmarks::sphere(double*x,int dim, int k){
  double result=0.0;
  int i;

  //	cout<<"partial sum of sphere function ="<<endl;
  for(i=dim/k-1;i>=0;i--)
    {
      result+=x[Pvector[dim/k+i]]*x[Pvector[dim/k+i]];
      //		printf("x = %1.16E\t", x[Pvector[dim/k+i]]);
      //		printf("partial sum = %1.16E\t", result);
    }

  return(result);
}

// for single group non-separable function
double Benchmarks::schwefel(double*x,int dim){
  int    j;
  double s1 = 0;
  double s2 = 0;
  
  // T_{osz}
  transform_osz(x,dim);
  
  // T_{asy}^{0.2}
  transform_asy(x, 0.2, dim);

  for (j = 0; j < dim; j++) {
    s1 += x[j];
    s2 += (s1 * s1);
  }

  return(s2);
}

// for m groups non-separable function
double Benchmarks::schwefel(double*x,int dim, int k){
  double sum1=0.0;
  double sum2=0.0;
  int i;
  for(i=0;i<dim;i++)
    {
      sum1+=x[Pvector[(k-1)*dim+i]];
      sum2+=sum1*sum1;
    }
  return(sum2);
}

// single group non-separable function
double Benchmarks::rosenbrock(double*x,int dim){
  int    j;
  double oz,t;
  double s = 0.0;
  j = dim - 1;

  for (--j; j >= 0; j--) {
    oz = x[j + 1];
    t  = ((x[j] * x[j]) - oz);
    s += (100.0 * t * t);
    t  = (x[j] - 1.0);
    s += (t * t);
  }
  return(s);
}

// m groups non-separable function
double Benchmarks::rosenbrock(double*x,int dim, int k){
  int j;
  double oz,t;
  double result=0.0;
  j=dim-1;
  for (--j; j >= 0; j--) {
    oz = x[Pvector[(k-1)*dim+j+1]];
    t = ((x[Pvector[(k-1)*dim+j]] * x[Pvector[(k-1)*dim+j]]) - oz);
    result += (100.0 * t * t);
    t = (x[Pvector[(k-1)*dim+j]] - 1.0);
    result += (t * t);
  }
  return(result);
}

int Benchmarks::getMinX(){
  return minX;
}
	
int Benchmarks::getMaxX(){
  return maxX;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getInterArray
 *  Description:  get the variable interaction information in the representation of one
 *  				dimensional array
 * =====================================================================================
 */
vector<bool>
Benchmarks::getInterArray (  )
{
  return interArray;
}		/* -----  end of function getInterArray  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  convertMatrixToArrayIndex
 *  Description:  
 * =====================================================================================
 */
unsigned	
Benchmarks::convertMatrixToArrayIndex ( unsigned i, unsigned j )
{
  return ( i* (2*dimension-i-3) / 2 + j - 1);
}		/* -----  end of function convertMatrixToArrayIndex  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createIndexMapping
 *  Description:  
 * =====================================================================================
 */
void
Benchmarks::createIndexMapping (  )
{
  unsigned N = dimension, indexCounter = 0;

  indexMap = new struct IndexMap[arrSize];

  for (unsigned i=0; i<N; i++){
    for (unsigned j=i+1; j<N; j++){
      indexMap[indexCounter].arrIndex1 = i;
      indexMap[indexCounter].arrIndex2 = j;
      indexCounter++;
    }
  }
}		/* -----  end of function CCVIL::createIndexMapping  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ArrToMat
 *  Description:  
 * =====================================================================================
 */
void
Benchmarks::ArrToMat ( unsigned I1, unsigned I2, unsigned &matIndex )
{
  for (unsigned i=0; i<arrSize; i++){
    if (indexMap[i].arrIndex1 == I1 && indexMap[i].arrIndex2 == I2){
      matIndex = i;
      return ;
    }
  }
	
  printf ( "Cannot locate the matrix index from given array indices\n" );
  exit(EXIT_FAILURE);
}		/* -----  end of function Benchmarks::ArrToMat  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Benchmarks::MatToArr
 *  Description:  
 * =====================================================================================
 */
void
Benchmarks::MatToArr ( unsigned &I1, unsigned &I2, unsigned matIndex )
{
  I1 = indexMap[matIndex].arrIndex1;
  I2 = indexMap[matIndex].arrIndex2;
}		/* -----  end of function Benchmarks::MatToArr  ----- */

int Benchmarks::sign(double x)
{
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

double Benchmarks::hat(double x)
{
  if (x==0)
    {
      return 0;
    }
  else
    {
      return log(abs(x));
    }
}

double Benchmarks::c1(double x)
{
  if (x>0)
    {
      return 10;
    }
  else
    {
      return 5.5;
    }
}

double Benchmarks::c2(double x)
{
  if (x>0)
    {
      return 7.9;
    }
  else
    {
      return 3.1;
    }
}


