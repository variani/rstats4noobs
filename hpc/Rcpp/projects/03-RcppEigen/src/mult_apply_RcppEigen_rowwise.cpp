#include <RcppEigen.h>
// [[Rcpp::depends(RcppEigen)]]

using namespace Rcpp;

using Eigen::Map;           // 'maps' rather than copies 
using Eigen::MatrixXd;      // variable size matrix, double precision
using Eigen::VectorXd;      // variable size vector, double precision

// [[Rcpp::export()]]
VectorXd mult_apply_RcppEigen_rowwise(const Map<MatrixXd> & mat, const Map<VectorXd> & vec)
{
  // variables
  int N = mat.rows(), K = mat.cols();
  
  // allocate memory for the output
  VectorXd rvec(N);
  
  for(int i = 0; i < N; i++) {
    rvec[i] = mat.row(i) * vec;
  }

  return(rvec);
}
