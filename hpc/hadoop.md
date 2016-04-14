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

## Install R-Haddoop

### Install dependencies

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

### Install thrift

* (for debian)http://thrift-tutorial.readthedocs.org/en/latest/installation.html

### Install rhdfs, rhbase & rmr2 R packages

* https://github.com/RevolutionAnalytics/RHadoop/wiki/Installing-RHadoop-on-RHEL

Download packages:

```
wget https://raw.github.com/RevolutionAnalytics/rhdfs/blob/master/build/rhdfs_1.0.8.tar.gz?raw=true
wget https://github.com/RevolutionAnalytics/rhbase/blob/master/build/rhbase_1.2.1.tar.gz?raw=true
wget https://github.com/RevolutionAnalytics/rmr2/releases/download/3.3.1/rmr2_3.3.1.tar.gz
```

Configure thrift (http://stackoverflow.com/a/36427232/551589):

```
cd /usr/local/lib/pkgconfig
sudo perl -pi -e 's{(^includedir=.*/include$)}{$1/thrift}' thrift.pc
sudo perl -pi -e 's{(^Cflags:.*)}{$1 -std=c++11}' thrift.pc
```

See the changes:

```
hduser@debian:/usr/local/lib/pkgconfig$ diff thrift.pc.bck thrift.pc
23c23
< includedir=${prefix}/include
---
> includedir=${prefix}/include/thrift
29c29
< Cflags: -I${includedir}
---
> Cflags: -I${includedir} -std=c++11
```

Install R packages from source:

```
Sys.setenv("HADOOP_CMD" = "/home/hduser/hadoop/bin/hadoop")
Sys.setenv("HADOOP_STREAMING" = "/home/hduser/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.7.2.jar")

install.packages("rhbase_1.2.1.tar.gz?raw=true", repos = NULL)
```

## Throubleshooting (R)-Hadoop

R code for testing (https://github.com/RevolutionAnalytics/rmr2/blob/master/docs/tutorial.md):

```
library(rmr2)
small.ints = to.dfs(1:1000)
out <- from.dfs(mapreduce(input = small.ints, map = function(k, v) cbind(v, v^2)))
```

### Error: Permission denied

Error message: "org.apache.hadoop.security.AccessControlException: Permission denied: user= access=WRITE, inode="/tmp/"

Solution: http://stackoverflow.com/a/29981409/551589

Step 1 : stop hadoop and clean temp files from hduser

```
sudo rm -r /app/hadoop/tmp
sudo mkdir -p /app/hadoop/tmp
sudo chown hduser:hadoop /app/hadoop/tmp
sudo chmod 750 /app/hadoop/tmp
```

Step 2: format namenode

```
hdfs namenode -format
```

If the error still persists, try a temporal solution http://stackoverflow.com/a/11603165/551589

Edit the hdfs-site.xml file:

```
<property>
  <name>dfs.permissions</name>
  <value>false</value>
</property>
```
