# bigmemory: parallel computing on objects shared in memory
Andrey Ziyatdinov  
`r Sys.Date()`  







## Include


```r
library(bigmemory)
```


```r
# Assignment will down cast from integer to char
# Hint: To remove this warning type:  options(bigmemory.typecast.warning=FALSE)
options(bigmemory.typecast.warning = FALSE)
```

## Parameters


```r
cores <- 2
parallel <- (cores > 1)
```

### Parallel 


```r
if(parallel) {
  registerDoParallel(cores = cores)
}
```

## Data


```r
n <- 10e3
m <- 1e3
#mat <- matrix(rbinom(m*n, 1, 0.5), nrow = n, ncol = m)
mat <- matrix(rnorm(m*n), nrow = n, ncol = m)
```

## Shared bigmemory object


```r
bigmat <- as.big.matrix(mat, type = "char")
bigdesc <- describe(bigmat)
```

## Computation

### Sequantial (base R)


```r
gc0 <- gc(reset = TRUE) 

t_seq_base <- system.time({
  out_seq_base <- laply(1:m, function(i) {
    mean(mat[, i])
  })
})

gc_seq_base <- gc()
```

### Sequantial (bigmemory)


```r
gc0 <- gc(reset = TRUE) 

t_seq <- system.time({
  out_seq <- laply(1:m, function(i) {
    bigmat <- attach.big.matrix(bigdesc)
    
    mean(bigmat[, i])
  })
})

gc_seq <- gc()
```

### Parallel (base R)


```r
gc0 <- gc(reset = TRUE) 

t_par_base <- system.time({
  out_par_base <- laply(1:m, function(i) {
    mean(mat[, i])
  }, .parallel = parallel)
})

gc_par_base <- gc()
```

### Sequantial (bigmemory)


```r
gc0 <- gc(reset = TRUE) 

t_par <- system.time({
  out_par <- laply(1:m, function(i) {
    bigmat <- attach.big.matrix(bigdesc)
    
    mean(bigmat[, i])
  }, .parallel = parallel)
})

gc_par <- gc()
```

### Summary on resources


-------------------------------------------
     copmutation        memory_Mb   time_s 
---------------------- ----------- --------
  Sequential (base)       195.1     0.281  

Sequential (bigmemory)    93.0      0.586  

   Parallel (base)        81.8      0.655  

 Parallel (bigmemory)     81.8      0.852  
-------------------------------------------
