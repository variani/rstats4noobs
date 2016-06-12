# include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export()]]
NumericVector mult_apply_Rcpp_rowwise(const NumericMatrix & mat, const NumericVector & vec)
{
  // variables
  int N = mat.nrow(), K = mat.ncol();
  
  // allocate memory for the output
  NumericVector rvec(N);
  
  for(int i = 0; i < N; i++) {
    double prod = 0;
    for(int j = 0; j < K; j++) {
      prod += mat(i, j) * vec(j);
    }
    rvec[i] = prod;
  }

  return(rvec);
}
