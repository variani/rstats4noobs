library(plyr)
library(ggplot2)
library(gridExtra)

theme_set(theme_light())

library(microbenchmark)

library(Rcpp)
library(RcppParallel)

### functions
sourceCpp("src/mult_RcppEigen.cpp")

sourceCpp("src/par_mult_apply_Rcpp_colwise.cpp") 

wrap_par_mult_apply_Rcpp_colwise <- function(..., cores = -1, chunkSize = 1)
{
  stopifnot(require(RcppParallel))
  
  if(!missing(cores)) { 
    RcppParallel::setThreadOptions(numThreads = cores)
  }
  
  par_mult_apply_Rcpp_colwise(..., chunkSize = chunkSize)
}

### bench
nseq <- seq(3000, 10000, length = 5)
df <- ldply(nseq, function(n) {
  n <- ceiling(n)
  k <- n

  set.seed(1)
  X <- matrix(runif(n * k), nrow = n, ncol = k)
  y <- runif(k)

  tX <- t(X)
  ty <- t(y)
  
  cat(" * n:", n, "\n")
  
  out <- microbenchmark(
    mult_RcppEigen(X, y),
    wrap_par_mult_apply_Rcpp_colwise(X, y, chunkSize = 100, cores = 2),
    wrap_par_mult_apply_Rcpp_colwise(X, y, chunkSize = 100, cores = 32),
    wrap_par_mult_apply_Rcpp_colwise(X, y, chunkSize = 100, cores = 64),
    wrap_par_mult_apply_Rcpp_colwise(X, y, chunkSize = 100),
    times = 10)
  
  df <- subset(as.data.frame(summary(out)), select = c("expr", "median"))
  df$n <- n
  
  return(df)
}) 

# plotting
p <- ggplot(df, aes(n, median, color = expr)) + geom_point() + geom_line()
p
