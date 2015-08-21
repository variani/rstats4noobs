## Install R

Start reading with instructions: 

* http://cran.r-project.org/bin/linux/ for linux users
* http://cran.r-project.org/bin/linux/ubuntu/README.html for ubuntu users.

(Step 1) Add repository to your /etc/apt/sources.list file.

For Ubuntu lucid:

```
deb http://<my.favorite.cran.mirror>/bin/linux/ubuntu lucid/
```

```
deb http://http://cran.r-project.org/bin/linux/ubuntu lucid/
```

(Step 2)

The Ubuntu archives on CRAN are signed with the key of "Vincent Goulet <vincent.goulet@act.ulaval.ca>" with key ID E2A11821. You can set up it like:

```
gpg --keyserver subkeys.pgp.net --recv-key E2A11821
gpg -a --export E2A11821 | sudo apt-key add -
```

Install the R base core from the command line:

```
sudo apt-get update
sudo apt-get install r-base
sudo apt-get install r-base-dev
```

When is it necessary?
 * Some packages, e.g. 'helpr', requires the last version of R base core.

## Install packages

### Error: `Warning: unable to access index for repository https://cran.r-project.org/src/contrib`

via http://stackoverflow.com/questions/21595822/unable-to-install-package-shiny-on-r-3-0-2

```
options(repos = c(CRAN = "http://cran.rstudio.com"))
install.packages('shiny')
```

### Package `helpr`

Requires the latest version of R

### Package `RCurl`

```
sudo apt-get install libcurl4-openssl-dev
sudo R >
install.packages("RCurl")
```

Second, install the package:

```
sudo R >
install.packages("helpr", dependencies = T)
```

### Other packages

```
sudo apt-get install libmysqlclient-dev
sudo apt-get install libgtk2.0-dev
sudo apt-get install libxml2-dev
```
