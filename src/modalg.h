/*
Copyright 2024, Alexander Jones

weight is free source code, under the MIT license (see LICENSE). You can redistribute, use and/or modify it.
Please give feedback to the authors if improvement is realized. It is distributed in the hope that it will be useful.

weight computes the 'weight' of the sequence k * 2^n + 1 for a range of k's.
See http://yves.gallot.pagesperso-orange.fr/papers/weight.pdf.
*/

#ifndef MOD_ALG_H
#define MOD_ALG_H 1

#include <cstdint>

uint64_t modinv(uint64_t a, uint64_t p);
uint64_t divmod(uint64_t a, uint64_t b, uint64_t p);

#endif // MOD_ALG_H
