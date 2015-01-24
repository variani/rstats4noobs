## Package `rmarkdown`

`rmarkdown` make use of `pandoc` functionality, and may require a higher version of `pandoc` of that installed on Ubuntu.

Solution is provided by Rstudio [via](https://github.com/rstudio/rmarkdown/blob/master/PANDOC.md)

* Install Rstudio http://www.rstudio.com/products/rstudio/download/
* Make links to binaries coming with the Rstudio distribution

```
sudo ln -s /usr/lib/rstudio/bin/pandoc/pandoc /usr/local/bin
sudo ln -s /usr/lib/rstudio/bin/pandoc/pandoc-citeproc /usr/local/bin
```
