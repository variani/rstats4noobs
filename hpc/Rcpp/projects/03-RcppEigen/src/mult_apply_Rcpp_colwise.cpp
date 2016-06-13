# include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export()]]
NumericVector mult_apply_Rcpp_colwise(const NumericMatrix & mat, 
  const NumericVector & vec, bool vectorize = false)
{
  // variables
  int N = mat.nrow(), K = mat.ncol();
  
  // allocate memory for the output
  NumericVector rvec(N);
  
  // allocate memory for the output
  for(int i = 0; i < N; i++) {
    rvec[i] = 0;
  }

  // do multiplication
  if(vectorize) {
    // vectorized version
    for(int i = 0; i < K; i++) {
      rvec += vec(i) * mat(_, i);
    }
  } else {
    // `for` version
    for(int j = 0; j < K; j++) {
      double a = vec(j);
      for(int i = 0; i < N; i++) {
        rvec(i) +=  a * mat(i, j);
      }
    }
  }
  
  return(rvec);
}
