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

inline bool isprime(const uint32_t n)
{
	if (n % 2 == 0) return (n == 2);
	if (n < 9) return true;
	const uint32_t s = uint32_t(sqrt(double(n))) + 1;
	for (uint32_t d = 3; d <= s; d += 2) if (n % d == 0) return false;
	return true;
}

class Weight
{
private:
	struct Pos
	{
		uint32_t p;
		uint32_t o;
		uint32_t * s;
	};

	const uint32_t s_max;
	const double C0;
	std::vector<Pos> pos_vect;

public:
	Weight(const uint32_t p_max, const uint32_t n_max) : s_max(n_max), C0(exp(0.577215664) * log(p_max) / double(n_max))
	{
		// Number number;
		for (uint32_t p = 3; p <= p_max; p += 2)
		{
			if (!isprime(p)) continue;
			// std::cout << p << std::endl;

			Pos pos; pos.p = p; pos.s = new uint32_t[p];
			for (size_t k = 0; k < p; ++k) pos.s[k] = uint32_t(-1);

			uint32_t k = p - 1;		// (p - 1) * 2^0 + 1 = 0 (mod p)
			for (uint32_t n = 0; n < p; ++n)
			{
				pos.s[k] = n;
				// std::cout << "  " << k << ", " << n << std::endl;

				// if k * 2^n + 1 = 0 (mod p) then k/2 * 2^{n + 1} + 1 = 0 (mod p)
				if (k % 2 == 0) k /= 2; else k = (k + p) / 2;
				if (k == p - 1)
				{
					// n + 1 is the order of 2 modulo p
					pos.o = n + 1;
					// std::cout << "  order = " << n + 1 << std::endl;
					break;
				}
			}
			pos_vect.push_back(pos);
		}
	}

	double val(const uint32_t k) const
	{
		std::vector<bool> sieve(s_max, true);
		for (const auto & po : pos_vect)
		{
			const uint32_t p = po.p, o = po.o;
			const uint32_t n = po.s[k % p];
			if (n == uint32_t(-1)) continue;
			for (size_t i = n; i < s_max; i += o) sieve[i] = false;
		}

		size_t n = 0;
		for (const bool b : sieve) n += b ? 1 : 0;

		return n * C0;
	}
};

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: weight k_min k_max [p_max] [s_max]" << std::endl;
		std::cout << "         Compute the weights for odd k_min <= k <= k_max." << std::endl;
		std::cout << "         Estimates are computed with p <= p_max (default 20000)" << std::endl;
		std::cout << "         and with n <= n_max is the max value (default 50000)." << std::endl;
		return EXIT_FAILURE;
	}

	uint32_t k_min = (argc > 1) ? uint32_t(std::atoll(argv[1])) : 3;
	uint32_t k_max = (argc > 2) ? uint32_t(std::atoll(argv[2])) : 1000000;
	if (k_min < 3) k_min = 3;
	if (k_min % 2 == 0) k_min += 1;
	if (k_max % 2 == 0) k_max -= 1;
	const uint32_t p_max = (argc > 3) ? uint32_t(std::atoll(argv[3])) : 20000;
	const uint32_t n_max = (argc > 4) ? uint32_t(std::atoll(argv[4])) : 50000;

	std::cout << "Initializing prime list..." << std::endl;
	Weight weight(p_max, n_max);

	std::ofstream pFile("weight.txt");
	if (!pFile.is_open()) return EXIT_FAILURE;

	std::cout << "Computing weights..." << std::endl;
	std::cout << std::setprecision(4);
	for (uint32_t k = k_min; k <= k_max; k += 2)
	{
		const double w = weight.val(k);
		pFile << k << " " << w << std::endl;
		if (k % 65536 == 1) std::cout << k * 100.0 / k_max << "%     \r";
	}
	std::cout << std::endl;

	pFile.close();
	std::cout << "Wrote file 'weight.txt'." << std::endl;

	return EXIT_SUCCESS;
}
