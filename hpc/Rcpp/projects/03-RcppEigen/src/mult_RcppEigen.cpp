#include <RcppEigen.h>
// [[Rcpp::depends(RcppEigen)]]

using namespace Rcpp;

using Eigen::Map;           // 'maps' rather than copies 
using Eigen::MatrixXd;      // variable size matrix, double precision
using Eigen::VectorXd;      // variable size vector, double precision

// [[Rcpp::export()]]
VectorXd mult_RcppEigen(const Map<MatrixXd> & mat, const Map<VectorXd> & v)
{
  VectorXd rvec = mat * v;
  
  return(rvec);
}
