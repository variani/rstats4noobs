## Install R on iOS

- https://rud.is/b/2015/10/22/installing-r-on-os-x-100-homebrew-edition/
  - https://rud.is/b/2015/10/20/installing-r-on-os-x/
- https://gist.github.com/sebkopf/9405675
- http://cran.us.r-project.org/
    - http://cran.us.r-project.org/bin/macosx/tools/
    - http://r.research.att.com/libs/
- fortran
    - http://stackoverflow.com/questions/23916219/os-x-package-installation-depends-on-gfortran-4-8

### brew
```
# base r
brew update
brew install r

brew install gls

# libs for R packages
brew install nlopt # for R package nloptr
brew install libssh2 # git2r
```

Directories

- `/usr/local/lib/R/3.3/` or `/usr/local/lib/R/3.4/`
    - for a particular package, e.g. Rcpp: `/usr/local/lib/R/3.4/site-library/Rcpp/include/`

### pck from cran

https://rud.is/b/2015/10/20/installing-r-on-os-x/

```
R 3.4.0
Version 3.4.0 (2017-04-21) -- "You Stupid Darkness"
for Mac OS X 10.11 (El Capitan) and higher

This multi-package contains following main components:
- R Framework 3.4.0            - R.app GUI 1.70
- Tcl/Tk 8.6.6 for X11 (optional, needed for the tcltk R package)
- Texinfo 5.2 (optional, needed to build documentation in R packages from sources)

Requirements:
- Mac OS X 10.11 (El Capitan) or higher

Note: By default the installer upgrades previous El Capitan build of R if present. If you want to keep the previous version, use
pkgutil --forget org.r-project.R.el-capitan.fw.pkg

The Cocoa GUI called R.app will be installed by default in your Applications folder,  R framework will be installed in /Library/Frameworks, Tcl/Tk and Texinfo will be installed in /usr/local
```

Fortran pkg:

```
GNU Fortran 4.2.3
for Mac OS X 10.4.4 or higher

This package contains gfortran 4.2.3 for Mac OS X 10.4.4 (Tiger) with Darwin (Apple) drivers. Supported targets are ppc, ppc64, i686 and x86_64. Multiple -arch flags can be specified to build fat binaries. All compilers are universal i.e. run natively on both Intel and PowerPC Macs. Both dynamic and static libraries are supplied.

Requirements:
- Mac OS X 10.4.4 or higher

All files will be installed in /usr/local

The compiler collection can be used by adding /usr/local/bin to your PATH, e.g.:
export PATH=$PATH:/usr/local/bin

To remove this package, run: sudo gfortran-uninstall

NOTE: Programs compiled with this Fortran compiler will dynamically link to libraries in /usr/local/lib to prevent incompatibilities with other installed compilers such as Xcode. Use static libraries or supply the libraries with your application if you want such binaries to run anywhere.

The preflight script of this package will fix /usr/loca/lib that was broken by Xcode installation (which creates an infinite symlink) if the problem is detected.
```

## Install R on Ubuntu

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

### unable to access index for repository

`Warning: unable to access index for repository https://cran.r-project.org/src/contrib`

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
