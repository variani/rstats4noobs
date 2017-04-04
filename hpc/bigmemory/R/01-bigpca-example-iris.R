### inc
library(bigpca)

### data
mat <- iris[, -5]

bmat <- as.big.matrix(mat, backingfile = "iris.mat", descriptorfile = "iris.dsc")

### model
mod <- big.PCA(bmat, return.loadings = TRUE)

### plot
plot(mod$loadings[, 1], mod$loadings[, 2], col = iris[, 5])

### clean
rm(bmat)
unlink(c("iris.mat","iris.dsc"))
