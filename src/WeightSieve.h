/*
Copyright 2020, Yves Gallot
Copyright 2024, Alexander Jones

weight is free source code, under the MIT license (see LICENSE). You can redistribute, use and/or modify it.
Please give feedback to the authors if improvement is realized. It is distributed in the hope that it will be useful.

weight computes the 'weight' of the sequence k * 2^n + 1 for a range of k's.
See http://yves.gallot.pagesperso-orange.fr/papers/weight.pdf.
*/

#ifndef WEIGHT_SIEVE_H
#define WEIGHT_SIEVE_H 1

#include <cstdint>
#include <vector>

class WeightSieve {
private:
	struct PrimeFactor {
		uint64_t prime;
		uint64_t order;
		uint64_t * sieve;

		PrimeFactor(const uint64_t prime) : prime(prime), sieve(new uint64_t[prime]) {
			for (size_t i = 0; i < prime; ++i) sieve[i] = uint64_t(-1);
		};
	};

	const uint64_t sieveMax;
	const double C0;
	std::vector<PrimeFactor> factorVector;

public:
	WeightSieve(const uint64_t primeMax, const uint64_t nMax);

	double value(const uint64_t k) const;
};

#endif // WEIGHT_SIEVE_H
