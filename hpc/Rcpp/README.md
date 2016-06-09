## Rcpp

* Quick reference https://cran.r-project.org/web/packages/Rcpp/vignettes/Rcpp-quickref.pdf

## Convert data

### From R to C++

* http://yuluer.com/page/dgghjeff-is-it-a-good-ideal-to-use-loops-for-rcpps-numericmatrix-in-c.shtml: `std::vector<double> v = Rcpp::as<std::vector<double>>(x);`

### From C++ to R

* http://stackoverflow.com/a/22381355/551589

## RcppArmadillo

* The official documentation: http://arma.sourceforge.net/docs.html
* Cheatsheet https://github.com/petewerner/misc/wiki/RcppArmadillo-cheatsheet

## RcppParallel

Main page: http://rcppcore.github.io/RcppParallel/

RcppEigen examples

* Parallel linear model bootstrap: https://gist.github.com/JWiley/d9cba55603471f75d438
    * http://lists.r-forge.r-project.org/pipermail/rcpp-devel/2015-April/008591.html

RcppArmadillo examples

* http://stackoverflow.com/questions/26234055/cohabitation-of-rcpparmadillo-and-rcppparallel
      * `arma::vec` is to be a reference member of the class

## crossprod

* [Faster R: Things to not forget](http://pj.freefaculty.org/blog/?p=122)
      * [Rnews_2004-1.pdf](https://cran.r-project.org/doc/Rnews/Rnews_2004-1.pdf)
