## Multiple threads

- https://stackoverflow.com/questions/40135812/is-there-a-way-to-limit-the-number-of-r-processes-running
- https://stat.ethz.ch/pipermail/r-sig-hpc/2012-July/001432.html

From https://cran.r-project.org/web/packages/mvnfast/vignettes/mvnfast.html:

```
# We might also need to turn off BLAS parallelism 
library(RhpcBLASctl)
blas_set_num_threads(1)
```
