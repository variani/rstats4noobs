## .Rprofile

Blog posts

* http://www.onthelambda.com/2014/09/17/fun-with-rprofile-and-customizing-r-startup/

### .Rprofile at andrey@salambo3

```
.First <- function() {
  options(
    repos = c(CRAN = "http://cran.rstudio.com/"),
    browserNLdisabled = TRUE,
    deparse.max.lines = 2)
}

if (interactive()) {
  suppressMessages(require(devtools))
  suppressMessages(require(knitr))
  suppressMessages(require(rmarkdown))
}
```
