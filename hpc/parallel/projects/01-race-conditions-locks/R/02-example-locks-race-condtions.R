# @ https://stackoverflow.com/questions/31575585/shared-memory-in-parallel-foreach-in-r/37964291#37964291
#
# usage: repeat this script several times

### inc
library(parallel)
library(bigmemory)

library(synchronicity) #for locks

### par
num_jobs <- 20

### shared object
shared <- big.matrix(nrow = 1, ncol = 1, type = 'double')
shared_sync <- big.matrix(nrow = 1, ncol = 1, type = 'double')

shared[1] <- 0
shared_sync[1]  <- 0

### semaphor
m <- boost.mutex()

### increment functions
inc_race <- function() 
{
  shared[1] <- shared[1] + 1
}

inc_sync <- function() 
{
  lock(m)
  shared_sync[1] <- shared_sync[1] + 1
  unlock(m)
}


### race
for(i in seq(1, num_jobs)) job <- mcparallel(inc_race())
out <- mccollect()

for(i in seq(1, num_jobs)) job <- mcparallel(inc_sync())
out <- mccollect()

cat(" - number of expected increments (number of jobs):", num_jobs, "\n")
cat(" - number of realized unsync. increments:", shared[1], "\n")
cat(" - number of realized sync. increments:", shared_sync[1], "\n")
