/*
Copyright 2020, Yves Gallot
Copyright 2024, Alexander Jones

weight is free source code, under the MIT license (see LICENSE). You can redistribute, use and/or modify it.
Please give feedback to the authors if improvement is realized. It is distributed in the hope that it will be useful.

weight computes the 'weight' of the sequence k * 2^n + 1 for a range of k's.
See http://yves.gallot.pagesperso-orange.fr/papers/weight.pdf.
*/

#include <algorithm>
#include <cstddef>
#include <cstdint>
#define _USE_MATH_DEFINES
#include <cmath>
#include <numbers>
// #include <iostream>
#include <vector>

#include <primesieve.hpp>

#include "WeightSieve.h"

inline static uint64_t divmod(uint64_t a, uint64_t b, uint64_t p) {
	if (b > p) {
		b %= p;
	}
	uint64_t aModB = a % b;
	while (aModB) {
		a += p;
		aModB = a % b;
	}
	return a / b;
}

WeightSieve::WeightSieve(const uint64_t primeMax, const uint64_t nMax, const uint32_t base, const int8_t c) : sieveMax(nMax), C0(exp(std::numbers::egamma) * log(primeMax) / double(nMax)), base(base), c(c) {
	primesieve::iterator it(2, primeMax);
	uint64_t prime = it.next_prime();

	for (; prime < primeMax; prime = it.next_prime()) {
		if (base % prime == 0) continue;
		// std::cout << prime << std::endl;

		PrimeFactor factor(prime);

		const uint64_t startingKModP = c > 0 ? prime - c : -c;
		uint64_t kModP = startingKModP;		// (p - 1) * 2^0 + 1 = 0 (mod p)
		for (uint64_t n = 0; n < prime; ++n) {
			factor.sieve[kModP] = n;
			// std::cout << "  " << kModP << ", " << n << std::endl;

			// if k * 2^n + 1 = 0 (mod p) then k/2 * 2^{n + 1} + 1 = 0 (mod p)
			kModP = divmod(kModP, base, prime);
			if (kModP == startingKModP) {
				// n + 1 is the order of 2 modulo p
				factor.order = n + 1;
				// std::cout << "  order = " << n + 1 << std::endl;
				break;
			}
		}
		factorVector.push_back(factor);
	}
}

double WeightSieve::value(const uint64_t k) const {
	std::vector<bool> sieve(sieveMax, true);

	for (const auto & factor : factorVector) {
		const uint64_t prime = factor.prime, order = factor.order;
		const uint64_t initialN = factor.sieve[k % prime];
		if (initialN == uint64_t(-1)) continue;
		for (size_t n = initialN; n < sieveMax; n += order) sieve[n] = false;
	}

	const std::ptrdiff_t nsRemaining = std::count(sieve.cbegin(), sieve.cend(), true);

	return nsRemaining * C0;
}
