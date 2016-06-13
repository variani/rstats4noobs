## Data storage in Eigen

Discussions

* [Response](http://stackoverflow.com/a/16286562/551589) to Most efficient way to loop through an Eigen matrix: 
  Eigen allocates matrices in column-major (Fortran) order by default. 
  See there also comments about L1 cache, etc

Blog posts (examples)

* [An RcppEigen example](http://dmbates.blogspot.com.es/2012/03/rcppeigen-example.html) by D. Bates
    * There are corresponding classes Eigen::VectorXd and Eigen::MatrixXd that provide linear algebra operations.
    *  A Eigen::Map of another structure has the corresponding structure but takes a pointer to the storage 
       instead of allocating its own storage.  
