/*
Copyright 2020, Yves Gallot
Copyright 2024, Alexander Jones

weight is free source code, under the MIT license (see LICENSE). You can redistribute, use and/or modify it.
Please give feedback to the authors if improvement is realized. It is distributed in the hope that it will be useful.

weight computes the 'weight' of the sequence k * 2^n + 1 for a range of k's.
See http://yves.gallot.pagesperso-orange.fr/papers/weight.pdf.
*/

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "WeightSieve.h"

int main(int argc, char * argv[]) {
	if (argc < 3) {
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
	WeightSieve weight(primeMax, nMax);

	std::ofstream pFile("weight.txt");
	if (!pFile.is_open()) return 2;

	std::cout << "Computing weights..." << std::endl;
	std::cout << std::setprecision(4);
	for (uint64_t k = kMin; k <= kMax; k += 2) {
		const double w = weight.value(k);
		pFile << k << " " << w << std::endl;
		if (k % 65536 == 1) std::cout << k * 100.0 / kMax << "%     \r";
	}
	std::cout << std::endl;

	pFile.close();
	std::cout << "Wrote file 'weight.txt'." << std::endl;

	return 0;
}
