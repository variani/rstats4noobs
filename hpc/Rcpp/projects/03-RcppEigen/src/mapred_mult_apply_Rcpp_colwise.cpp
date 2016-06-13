#include <Rcpp.h>

// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>

using namespace RcppParallel;
using namespace Rcpp;

struct MapRedMultApplyRcppColwise : public Worker
{
  // source matrix & vector
  const RMatrix<double> mat;
  const RVector<double> vec;
  
  // destination vector
  RVector<double> rvec;

  unsigned int nchunks; 
     
  // initialize 
  MapRedMultApplyRcppColwise(const NumericMatrix & mat, const NumericVector & vec, NumericVector rvec)
    : mat(mat), vec(vec), rvec(rvec), nchunks(0) {}
  MapRedMultApplyRcppColwise(const MapRedMultApplyRcppColwise & mult, Split)
    : mat(mult.mat), vec(mult.vec), rvec(mult.rvec), nchunks(0) {}
   
  // do multiplication using loops
  void operator()(std::size_t begin, std::size_t end) 
  {
    int N = mat.nrow();
    
    // `for` version
    for(int j = begin; j < end; j++) {
      double a = vec[j];
      RMatrix<double>::Column colj = mat.column(j);
      for(int i = 0; i < N; i++) {
        rvec[i] += a * colj[i];
      }
    }
    
    ++nchunks;
  }
  
  // join my value with that of another Sum
   void join(const MapRedMultApplyRcppColwise & mult) { 
      nchunks += mult.nchunks; 
   }
};


// [[Rcpp::export]]
NumericVector mapred_mult_apply_Rcpp_colwise(const NumericMatrix & mat, 
  const NumericVector & vec, unsigned int chunkSize = 1, unsigned int verbose = 0)
{
  // variables
  int N = mat.nrow(), K = mat.ncol();
  
  // allocate the vector to be returned
  NumericVector rvec(N);
    
  // fill with zeros
  for(int i = 0; i < N; i++) {
    rvec[i] = 0;
  }

  // create the worker
  MapRedMultApplyRcppColwise mult(mat, vec, rvec);
     
  // call it with parallelFor
  parallelReduce(0, K, mult, chunkSize);
  
  // print diagnostics
  if(verbose > 0) {
    Rcout << "#chunks: " << mult.nchunks << std::endl;
  }
  
  // return  
  return(rvec);
  
}
