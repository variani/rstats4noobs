#include <RcppEigen.h>
// [[Rcpp::depends(RcppEigen)]]

using namespace Rcpp;

using Eigen::Map;           // 'maps' rather than copies 
using Eigen::MatrixXd;      // variable size matrix, double precision
using Eigen::VectorXd;      // variable size vector, double precision

// [[Rcpp::export()]]
VectorXd mult_apply_RcppEigen_colwise(const Map<MatrixXd> & mat, const Map<VectorXd> & vec)
{
  // variables
  int N = mat.rows(), K = mat.cols();
  
  // allocate memory for the output
  VectorXd rvec(N);
  rvec.fill(0);  
  
  for(int i = 0; i < K; i++) {
    rvec += vec(i) * mat.col(i);
  }

  return(rvec);
}
