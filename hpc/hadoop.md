## R & hadoop

Tutorials

* https://rdatamining.wordpress.com/2013/08/20/step-by-step-to-build-my-first-r-hadoop-system/

## Install hadoop

* Thread http://askubuntu.com/questions/144433/how-to-install-hadoop
    * Protocol 1: http://askubuntu.com/a/279623
    * Protocol 2 (preferred, based on Protocol 1): http://askubuntu.com/a/546196

Tutorials

* http://www.bogotobogo.com/Linux/hadoop2_running_on_ubuntu_single_node_cluster.php
* http://www.michael-noll.com/tutorials/running-hadoop-on-ubuntu-linux-single-node-cluster/

Issues

* Warning `WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable`
      * http://www.ercoppa.org/Linux-Compile-Hadoop-220-fix-Unable-to-load-native-hadoop-library.htm

## Install R packages for haddoop

Configure Java path for R:

```
export JAVA_HOME=/usr/lib/jvm/java-7-oracle  
export PATH=$PATH:$JAVA_HOME/bin
R CMD javareconf
```

Install R packages, dependencies of hadoop-R:

```
install.packages(c("rJava", "Rcpp", "RJSONIO", "bitops", "digest", "functional", "stringr", "plyr", "reshape2"))
```
