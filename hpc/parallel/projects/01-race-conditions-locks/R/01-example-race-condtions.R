# @ https://stackoverflow.com/questions/31575585/shared-memory-in-parallel-foreach-in-r/37964291#37964291
#
# usage: repeat this script several times

### inc
library(parallel)
library(bigmemory)

### par
num_jobs <- 10

### shared object
shared <- big.matrix(nrow = 1, ncol = 1, type = 'double')

shared[1] <- 0

for(i in seq(1, num_jobs)) job <- mcparallel(shared[1] <- shared[1] + 1)
out <- mccollect()

cat(" - numer of expected increments (number of jobs):", num_jobs, "\n")
cat(" - numebre of realized increments:", shared[1], "\n")
