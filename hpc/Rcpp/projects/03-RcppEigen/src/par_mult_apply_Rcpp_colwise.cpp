#include <Rcpp.h>

// [[Rcpp::depends(RcppParallel)]]
#include <RcppParallel.h>

using namespace RcppParallel;
using namespace Rcpp;

struct ParMultApplyRcppColwise : public Worker
{
  // source matrix & vector
  const NumericMatrix & mat;
  const NumericVector & vec;
  
  // destination vector
  NumericVector & rvec;

  unsigned int nchunks; 
     
  // initialize 
  ParMultApplyRcppColwise(const NumericMatrix & mat, const NumericVector & vec, NumericVector & rvec)
    : mat(mat), vec(vec), rvec(rvec), nchunks(0) {}
   
  // do multiplication using loops
  void operator()(std::size_t begin, std::size_t end) 
  {
    int N = mat.nrow();
    
    // `for` version
    for(int j = begin; j < end; j++) {
      double a = vec(j);
      for(int i = 0; i < N; i++) {
        rvec(i) += a * mat(i, j);
      }
    }
    
    ++nchunks;
  }
};


// [[Rcpp::export]]
NumericVector par_mult_apply_Rcpp_colwise(const NumericMatrix & mat, 
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
  ParMultApplyRcppColwise mult(mat, vec, rvec);
     
  // call it with parallelFor
  parallelFor(0, K, mult, chunkSize);
  
  // print diagnostics
  if(verbose > 0) {
    Rcout << "#chunks: " << mult.nchunks << std::endl;
  }
  
  // return  
  return(rvec);
  
}
