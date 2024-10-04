/*
Copyright 2020, Yves Gallot

weight is free source code, under the MIT license (see LICENSE). You can redistribute, use and/or modify it.
Please give feedback to the authors if improvement is realized. It is distributed in the hope that it will be useful.

weight computes the 'weight' of the sequence k * 2^n + 1 for a range of k's.
See http://yves.gallot.pagesperso-orange.fr/papers/weight.pdf.
*/

#include <cstdint>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <primesieve.hpp>

class Weight
{
private:
	struct PrimeFactor
	{
		uint64_t prime;
		uint64_t order;
		uint64_t * sieve;
	};

	const uint64_t sieveMax;
	const double C0;
	std::vector<PrimeFactor> factorVector;

public:
	Weight(const uint64_t primeMax, const uint64_t nMax) : sieveMax(nMax), C0(exp(0.577215664) * log(primeMax) / double(nMax))
	{

		primesieve::iterator it(3, primeMax);
		uint64_t prime = it.next_prime();
		for (; prime < primeMax; prime = it.next_prime())
		{
			// std::cout << prime << std::endl;

			PrimeFactor factor; factor.prime = prime; factor.sieve = new uint64_t[prime];
			for (size_t kModP = 0; kModP < prime; ++kModP) factor.sieve[kModP] = uint64_t(-1);

			uint64_t kModP = prime - 1;		// (p - 1) * 2^0 + 1 = 0 (mod p)
			for (uint64_t n = 0; n < prime; ++n)
			{
				factor.sieve[kModP] = n;
				// std::cout << "  " << kModP << ", " << n << std::endl;

				// if k * 2^n + 1 = 0 (mod p) then k/2 * 2^{n + 1} + 1 = 0 (mod p)
				if (kModP % 2 == 0) kModP /= 2; else kModP = (kModP + prime) / 2;
				if (kModP == prime - 1)
				{
					// n + 1 is the order of 2 modulo p
					factor.order = n + 1;
					// std::cout << "  order = " << n + 1 << std::endl;
					break;
				}
			}
			factorVector.push_back(factor);
		}
	}

	double val(const uint64_t k) const
	{
		std::vector<bool> sieve(sieveMax, true);
		for (const auto & factor : factorVector)
		{
			const uint64_t prime = factor.prime, order = factor.order;
			const uint64_t initialN = factor.sieve[k % prime];
			if (initialN == uint64_t(-1)) continue;
			for (size_t n = initialN; n < sieveMax; n += order) sieve[n] = false;
		}

		size_t nsRemaining = 0;
		for (const bool nNotFactored : sieve) nsRemaining += nNotFactored ? 1 : 0;

		return nsRemaining * C0;
	}
};

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: weight k_min k_max [prime_max] [s_max]" << std::endl;
		std::cout << "         Compute the weights for odd k_min <= k <= k_max." << std::endl;
		std::cout << "         Estimates are computed with p <= prime_max (default 20000)" << std::endl;
		std::cout << "         and with n <= n_max is the max value (default 50000)." << std::endl;
		return 1;
	}

	uint64_t kMin = (argc > 1) ? uint64_t(std::atoll(argv[1])) : 3;
	uint64_t kMax = (argc > 2) ? uint64_t(std::atoll(argv[2])) : 1000000;
	if (kMin < 3) kMin = 3;
	if (kMin % 2 == 0) kMin += 1;
	if (kMax % 2 == 0) kMax -= 1;
	const uint64_t primeMax = (argc > 3) ? uint64_t(std::atoll(argv[3])) : 20000;
	const uint64_t nMax = (argc > 4) ? uint64_t(std::atoll(argv[4])) : 50000;

	std::cout << "Initializing prime list..." << std::endl;
	Weight weight(primeMax, nMax);

	std::ofstream pFile("weight.txt");
	if (!pFile.is_open()) return 2;

	std::cout << "Computing weights..." << std::endl;
	std::cout << std::setprecision(4);
	for (uint64_t k = kMin; k <= kMax; k += 2)
	{
		const double w = weight.val(k);
		pFile << k << " " << w << std::endl;
		if (k % 65536 == 1) std::cout << k * 100.0 / kMax << "%     \r";
	}
	std::cout << std::endl;

	pFile.close();
	std::cout << "Wrote file 'weight.txt'." << std::endl;

	return 0;
}
