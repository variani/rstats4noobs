# Make a try of partykit
Andrey Ziyatdinov  
`r Sys.Date()`  



# Include


```r
library(plyr)
library(ggplot2)
library(gridExtra)

library(pander)
```


```r
theme_set(theme_light())

panderOptions('table.style', 'rmarkdown')

panderOptions('table.split.table', Inf)
panderOptions('knitr.auto.asis', FALSE)
```

## Include `partykit`


# Data sets

## Titanic


```r
data(Titanic, package = "datasets")

ttnc <- as.data.frame(Titanic)
ttnc <- ttnc[rep(1:nrow(ttnc), ttnc$Freq), 1:4]
names(ttnc)[2] <- "Gender"
```


```r
library(partykit)
```

# Code from the vignette constparty

## findsplit


```r
findsplit <- function(response, data, weights = NULL, alpha = 0.01) 
{
  if (is.null(weights)) weights <- rep(1L, nrow(data))
  
  ## extract response values from data
  y <- factor(rep(data[[response]], weights))

  ## perform chi-squared test of y vs. x
  mychisqtest <- function(x) {
    x <- factor(x)
    if(length(levels(x)) < 2) return(NA)
    ct <- suppressWarnings(chisq.test(table(y, x), correct = FALSE))
    pchisq(ct$statistic, ct$parameter, log = TRUE, lower.tail = FALSE)
  }
  xselect <- which(names(data) != response)
  logp <- sapply(xselect, function(i) mychisqtest(rep(data[[i]], weights)))
  names(logp) <- names(data)[xselect]

  ## Bonferroni-adjusted p-value small enough?
  if(all(is.na(logp))) return(NULL)
  minp <- exp(min(logp, na.rm = TRUE))
  minp <- 1 - (1 - minp)^sum(!is.na(logp))
  if(minp > alpha) return(NULL)

  ## for selected variable, search for split minimizing p-value  
  xselect <- xselect[which.min(logp)]
  x <- rep(data[[xselect]], weights)

  ## set up all possible splits in two kid nodes
  lev <- levels(x[drop = TRUE])
  if(length(lev) == 2) {
    splitpoint <- lev[1]
  } else {
    comb <- do.call("c", lapply(1:(length(lev) - 2),
      function(x) combn(lev, x, simplify = FALSE)))
    xlogp <- sapply(comb, function(q) mychisqtest(x %in% q))
    splitpoint <- comb[[which.min(xlogp)]]
  }

  ## split into two groups (setting groups that do not occur to NA)
  splitindex <- !(levels(data[[xselect]]) %in% splitpoint)
  splitindex[!(levels(data[[xselect]]) %in% lev)] <- NA_integer_
  splitindex <- splitindex - min(splitindex, na.rm = TRUE) + 1L

  ## return split as partysplit object
  return(partysplit(varid = as.integer(xselect),
    index = splitindex,
    info = list(p.value = 1 - (1 - exp(logp))^sum(!is.na(logp)))))
}
```

## growtree


```r
growtree <- function(id = 1L, response, data, weights = NULL, minbucket = 30) 
{
  if (is.null(weights)) weights <- rep(1L, nrow(data))

  ## for less than 30 observations stop here
  if (sum(weights) < minbucket) return(partynode(id = id))

  ## find best split
  sp <- findsplit(response, data, weights)
  ## no split found, stop here
  if (is.null(sp)) return(partynode(id = id))

  ## actually split the data
  kidids <- kidids_split(sp, data = data)

  ## set up all daugther nodes
  kids <- vector(mode = "list", length = max(kidids, na.rm = TRUE))
  for (kidid in 1:length(kids)) {
  ## select observations for current node
  w <- weights
  w[kidids != kidid] <- 0
  ## get next node id
  if (kidid > 1) {
    myid <- max(nodeids(kids[[kidid - 1]]))
  } else {
    myid <- id
  }
  ## start recursion on this daugther node
  kids[[kidid]] <- growtree(id = as.integer(myid + 1), response, data, w)
  }

  ## return nodes
  return(partynode(id = as.integer(id), split = sp, kids = kids,
    info = list(p.value = min(info_split(sp)$p.value, na.rm = TRUE))))
}
```

## mytree


```r
mytree <- function(formula, data, weights = NULL) 
{
  ## name of the response variable
  response <- all.vars(formula)[1]
  ## data without missing values, response comes last
  data <- data[complete.cases(data), c(all.vars(formula)[-1], response)]
  ## data is factors only
  stopifnot(all(sapply(data, is.factor)))

  if (is.null(weights)) weights <- rep(1L, nrow(data))
  ## weights are case weights, i.e., integers
  stopifnot(length(weights) == nrow(data) &
    max(abs(weights - floor(weights))) < .Machine$double.eps)

  ## grow tree
  nodes <- growtree(id = 1L, response, data, weights)

  ## compute terminal node number for each observation
  fitted <- fitted_node(nodes, data = data)
  ## return rich constparty object
  ret <- party(nodes, data = data,
    fitted = data.frame("(fitted)" = fitted,
                        "(response)" = data[[response]],
                        "(weights)" = weights,
                        check.names = FALSE),
    terms = terms(formula))
  as.constparty(ret)
}
```

## Fit mytree to Titanic data set


```r
(myttnc <- mytree(Survived ~ Class + Age + Gender, data = ttnc))
```

```

Model formula:
Survived ~ Class + Age + Gender

Fitted party:
[1] root
|   [2] Gender in Male
|   |   [3] Class in 1st
|   |   |   [4] Age in Child: Yes (n = 5, err = 0.0%)
|   |   |   [5] Age in Adult: No (n = 175, err = 32.6%)
|   |   [6] Class in 2nd, 3rd, Crew
|   |   |   [7] Age in Child
|   |   |   |   [8] Class in 2nd: Yes (n = 11, err = 0.0%)
|   |   |   |   [9] Class in 3rd: No (n = 48, err = 27.1%)
|   |   |   [10] Age in Adult
|   |   |   |   [11] Class in Crew: No (n = 862, err = 22.3%)
|   |   |   |   [12] Class in 2nd, 3rd: No (n = 630, err = 14.1%)
|   [13] Gender in Female
|   |   [14] Class in 3rd: No (n = 196, err = 45.9%)
|   |   [15] Class in 1st, 2nd, Crew: Yes (n = 274, err = 7.3%)

Number of inner nodes:    7
Number of terminal nodes: 8
```


```r
plot(myttnc)
```

![](figures/plot_myttnc-1.png)<!-- -->

# Grow a tree up to certain depth


```r
growtree_depth <- function(id = 1L, depth = 0, response, data, weights = NULL, 
  minbucket = 30, maxdepth = 0) 
{
  if (is.null(weights)) weights <- rep(1L, nrow(data))

  ## for less than 30 observations stop here
  if(sum(weights) < minbucket | depth >= maxdepth) {
    return(partynode(id = id, info = list(depth = depth)))
  }

  ## find best split
  sp <- findsplit(response, data, weights)
  ## no split found, stop here
  if (is.null(sp)) return(partynode(id = id, info = list(depth = depth)))

  ## actually split the data
  kidids <- kidids_split(sp, data = data)

  ## set up all daugther nodes
  kids <- vector(mode = "list", length = max(kidids, na.rm = TRUE))
  for (kidid in 1:length(kids)) {
  ## select observations for current node
  w <- weights
  w[kidids != kidid] <- 0
  ## get next node id
  if (kidid > 1) {
    myid <- max(nodeids(kids[[kidid - 1]]))
  } else {
    myid <- id
  }
  ## start recursion on this daugther node
  kids[[kidid]] <- growtree_depth(id = as.integer(myid + 1), depth = depth + 1, 
    response, data, w, 
    minbucket = minbucket, maxdepth = maxdepth)
  }

  ## return nodes
  return(partynode(id = as.integer(id), split = sp, kids = kids,
    info = list(depth = depth)))
}
```


```r
mytree_depth <- function(formula, data, weights = NULL, maxdepth = 0) 
{
  ## name of the response variable
  response <- all.vars(formula)[1]
  ## data without missing values, response comes last
  data <- data[complete.cases(data), c(all.vars(formula)[-1], response)]
  ## data is factors only
  stopifnot(all(sapply(data, is.factor)))

  if (is.null(weights)) weights <- rep(1L, nrow(data))
  ## weights are case weights, i.e., integers
  stopifnot(length(weights) == nrow(data) &
    max(abs(weights - floor(weights))) < .Machine$double.eps)

  ## grow tree
  nodes <- growtree_depth(id = 1L, depth = 0, response, data, weights, maxdepth = maxdepth)

  ## compute terminal node number for each observation
  fitted <- fitted_node(nodes, data = data)
  ## return rich constparty object
  ret <- party(nodes, data = data,
    fitted = data.frame("(fitted)" = fitted,
                        "(response)" = data[[response]],
                        "(weights)" = weights,
                        check.names = FALSE),
    terms = terms(formula))
  as.constparty(ret)
}
```

## Fit mytree to Titanic data set


```r
# default
mytree_depth(Survived ~ Class + Age + Gender, data = ttnc, maxdepth = 0)
```

```

Model formula:
Survived ~ Class + Age + Gender

Fitted party:
[1] root: No (n = 2201, err = 32.3%) 

Number of inner nodes:    0
Number of terminal nodes: 1
```

```r
# maxedpth 2
(t2 <- mytree_depth(Survived ~ Class + Age + Gender, data = ttnc, maxdepth = 2))
```

```

Model formula:
Survived ~ Class + Age + Gender

Fitted party:
[1] root
|   [2] Gender in Male
|   |   [3] Class in 1st: No (n = 180, err = 34.4%)
|   |   [4] Class in 2nd, 3rd, Crew: No (n = 1551, err = 19.7%)
|   [5] Gender in Female
|   |   [6] Class in 3rd: No (n = 196, err = 45.9%)
|   |   [7] Class in 1st, 2nd, Crew: Yes (n = 274, err = 7.3%)

Number of inner nodes:    3
Number of terminal nodes: 4
```

```r
# maxedpth 20
mytree_depth(Survived ~ Class + Age + Gender, data = ttnc, maxdepth = 20)
```

```

Model formula:
Survived ~ Class + Age + Gender

Fitted party:
[1] root
|   [2] Gender in Male
|   |   [3] Class in 1st
|   |   |   [4] Age in Child: Yes (n = 5, err = 0.0%)
|   |   |   [5] Age in Adult: No (n = 175, err = 32.6%)
|   |   [6] Class in 2nd, 3rd, Crew
|   |   |   [7] Age in Child
|   |   |   |   [8] Class in 2nd: Yes (n = 11, err = 0.0%)
|   |   |   |   [9] Class in 3rd: No (n = 48, err = 27.1%)
|   |   |   [10] Age in Adult
|   |   |   |   [11] Class in Crew: No (n = 862, err = 22.3%)
|   |   |   |   [12] Class in 2nd, 3rd: No (n = 630, err = 14.1%)
|   [13] Gender in Female
|   |   [14] Class in 3rd: No (n = 196, err = 45.9%)
|   |   [15] Class in 1st, 2nd, Crew: Yes (n = 274, err = 7.3%)

Number of inner nodes:    7
Number of terminal nodes: 8
```

## Add an additional node


```r
t <- node_party(t2)

copytree <- function(node, id = 1L) 
{
  node_id <- node$id
  stopifnot(node_id == id)
  
  sp <- node$split
  if(is.null(sp)) {
    return(node)
  }

  ## actually split the data
  kids <- vector(mode = "list", length = max(kidids, na.rm = TRUE))

  for(kidid in 1:length(node$kids)) {
    kids[[kidid]] <- copytree(node$kids[[kidid]], id = node$kids[[kidid]]$id)
  }

  ## return nodes
  return(partynode(id = as.integer(id), split = sp, kids = kids,
    info = node$info))
}
```

## Manual interface


```r
tman <- partynode(1L, split = t$split, kids = list(
  partynode(2L, split = t$kids[[1]]$split, kids = list(
    partynode(3L),
    partynode(4L))),
  partynode(5L, split = t$kids[[2]]$split, kids = list(
    partynode(6L),
    partynode(7L)))))
```


```r
l <- list()

l <- c(l, list(
  list(id = 1, split = t$split, kids = c(2, 5), depth = 0),

  list(id = 2, split = t$kids[[1]]$split, kids = c(3, 4), depth = 1),
  list(id = 5, split = t$kids[[2]]$split, kids = c(6, 7), depth = 1),
  
  list(id = 3, depth = 2),
  list(id = 4, depth = 2),
  list(id = 6, depth = 2),
  list(id = 7, depth = 2))) 
```


```r
names(l) <- laply(l, function(x) x$id) 
```


```r
list2tree <- function(id = 1, nlist) 
{
  stopifnot(!is.null(names(nlist)))
  
  idstr <- as.character(id)
  stopifnot(idstr %in% names(nlist))
  
  sp <- nlist[[idstr]]$split
  
  if(is.null(sp)) {
    return(partynode(id = as.integer(id)))
  }

  lkids <- nlist[[idstr]]$kids
  stopifnot(!is.null(lkids))
  
  ## actually split the data
  kids <- vector(mode = "list", length = length(lkids))

  for(kidid in 1:length(lkids)) {
    kids[[kidid]] <- list2tree(id = lkids[kidid], nlist)
  }

  ## return nodes
  return(partynode(id = as.integer(id), split = sp, kids = kids))
}
```

# Grow a tree vertically


```r
# id = 1; response = "Survived"; data = ttnc; weights = rep(1, nrow(ttnc))
# growtree_vert(1, "Survived", ttnc, rep(1, nrow(ttnc)))
growtree_vert <- function(id = 1L, response, data, weights = NULL, minbucket = 30) 
{
  if (is.null(weights)) weights <- rep(1L, nrow(data))

  ### init
  depth <- 0

  nlist <- list() 
  nlist <- c(nlist, list(
    list(id = 1, weights = weights, depth = depth)))
  
  maxid <- 1    
  
  ### loop by `depth`
  while(TRUE) {
    depth <- depth + 1
    
    dvals <- laply(nlist, function(x) x$depth)
    nind <- which(dvals == (depth - 1))
    
    cnt <- 0
    for(i in seq(1, length(nind))) {
      k <- nind[i]
      node <- nlist[[k]]
      
      # break on `minbucket`
      if(sum(node$weights) < minbucket) {
        next
      } 
      
      # find best split
      sp <- findsplit(response, data, node$weights)
      
      #  break on no split found
      if(is.null(sp)) {
        next
      }
      
      # split the data
      kidids <- kidids_split(sp, data = data)
      lenkids <- max(kidids, na.rm = TRUE)

      # udapte the node
      nlist[[k]]$split <- sp
      nlist[[k]]$kids <- seq(maxid + 1, by = 1, length = lenkids)

      # set up all daugther nodes
      for(kidid in seq(1, lenkids)) {
        # select observations for current node
        w <- node$weights
        w[kidids != kidid] <- 0
        
        # add a new node
        nlist <- c(nlist, list(
          list(id = maxid + 1, weights = w, depth = depth)))

        maxid <- maxid + 1
      }
      
      cnt <- cnt + 1
    }
    
    if(cnt == 0) {
      break
    }
  }
  names(nlist) <- laply(nlist, function(x) x$id) 
  
  return(nlist)
}
```


```r
# formula = Survived ~ Class + Age + Gender; data = ttnc; weights = NULL
# mytree_vert(Survived ~ Class + Age + Gender, ttnc)
mytree_vert <- function(formula, data, weights = NULL) 
{
  ## name of the response variable
  response <- all.vars(formula)[1]
  ## data without missing values, response comes last
  data <- data[complete.cases(data), c(all.vars(formula)[-1], response)]
  ## data is factors only
  stopifnot(all(sapply(data, is.factor)))

  if (is.null(weights)) weights <- rep(1L, nrow(data))
  ## weights are case weights, i.e., integers
  stopifnot(length(weights) == nrow(data) &
    max(abs(weights - floor(weights))) < .Machine$double.eps)

  ## grow tree
  nlist <- growtree_vert(id = 1L, response, data, weights)
  nodes <- list2tree(id = 1, nlist) 

  ## compute terminal node number for each observation
  fitted <- fitted_node(nodes, data = data)
  ## return rich constparty object
  ret <- party(nodes, data = data,
    fitted = data.frame("(fitted)" = fitted,
                        "(response)" = data[[response]],
                        "(weights)" = weights,
                        check.names = FALSE),
    terms = terms(formula))
  as.constparty(ret)
}
```

## Applyt to the Titanic dataset


```r
vert <- mytree_vert(Survived ~ Class + Age + Gender, ttnc)
plot(vert)
```

![](figures/vert_tree-1.png)<!-- -->


