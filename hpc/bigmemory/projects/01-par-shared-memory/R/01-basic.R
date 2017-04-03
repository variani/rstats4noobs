# @ https://cran.r-project.org/web/packages/bigmemory/vignettes/Overview.pdf
# Footnote on p.4: We should note that doMC and multicore are particularly 
# well-suited for this.  When other parallel backends are used, one additional 
# command is required in the birthmonth() function: x <- attach.big.matrix(xdesc)
# where xdesc <- describe(x) would be required just prior to the foreach()
# loop, providing explicit shared-memory access across processes. In contrast,
# multicore automatically operates on shared memory, avoiding the need for this extra step.

### inc
library(bigmemory)

library(parallel)
library(plyr)
library(doParallel)

### par
cores <- 2
parallel <- (cores > 1)

### data
n <- 1e3
m <- 1e2
mat <- matrix(rbinom(m*n, 1, 0.5), nrow = n, ncol = m)

### big data
dat <- as.big.matrix(mat, shared = TRUE)
desc <- describe(dat)

rm(mat)

### parallel
if(parallel) {
  #cl <- makeCluster(cores, type = "FORK")
  #registerDoParallel(cl)
  registerDoParallel(cores = cores)
}

### run
#out <- parSapply(cl, 1:m, function(i) {
#out <- foreach(i = 1:m, .combine = c) %dopar% {
out <- laply(1:m, function(i) {
  #stopifnot(require(bigmemory))
  #dat2 <- attach.big.matrix(desc)
    
  mean(dat[, i])
#}) 
#}
}, .parallel = parallel) 
# .paropts = list(.packages = "bigmemory")

### cleaning
if(parallel) {
  #stopCluster(cl)
  stopImplicitCluster()
  ret <- gc()
}
