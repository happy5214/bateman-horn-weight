# weight
Compute the weight of Proth numbers

## About

Let *k* be an odd positive integer. We search for an estimate of the number of primes in the sequence of numbers of the form *P*<sub>*k*</sub> = *k*·2<sup>*n*</sup> + 1 for 1 &le; *n* &le; *N*. It is expected to be *C*<sub>*k*</sub> Sum<sub>1 &le; *n* &le; *N*</sub> 1/log(*P*<sub>*k*</sub>).  

**weight** computes an approximation of *C*<sub>*k*</sub> and is optimized for a large range of *k*.  
See http://yves.gallot.pagesperso-orange.fr/papers/weight.pdf for more details and the definition of p_max (*P*<sub>0</sub>) and n_max (*N*<sub>0</sub>).

## Build

This version was compiled with gcc.
