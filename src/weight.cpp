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

inline uint32_t mulmod(const uint32_t x, const uint32_t y, const uint32_t m)
{
	return uint32_t((x * uint64_t(y)) % m);
}

inline uint32_t powmod(const uint32_t x, const uint32_t n, const uint32_t m)
{
	if (n == 0) return 1;

	uint32_t r = 1, y = x;
	for (uint32_t i = n; i != 1; i /= 2)
	{
		if (i % 2 != 0) r = mulmod(r, y, m);
		y = mulmod(y, y, m);
	}
	return mulmod(r, y, m);
}

class Number
{
private:
	std::vector<uint32_t> _prime;

public:
	Number()
	{
		_prime.push_back(2);
		for (uint32_t p = 3; p < 65536; p += 2) if (isprime(p)) _prime.push_back(p);
	}

public:
	void factor(const uint32_t n, std::vector<std::pair<uint32_t, uint32_t> > & fac) const
	{
		uint32_t m = n;
		for (const uint32_t & p : _prime)
		{
			if (m % p == 0)
			{
				uint32_t e = 0;
				do
				{
					m /= p;
					++e;	
				}
				while (m % p == 0);

				fac.push_back(std::make_pair(p, e));
				if (m == 1) return;
			}
		}
		if (m != 1) fac.push_back(std::make_pair(m, 1));
	}

public:
	uint32_t order2(const uint32_t p) const
	{
		uint32_t e = p - 1;

		std::vector<std::pair<uint32_t, uint32_t>> fac;
		factor(e, fac);

		for (const auto & f : fac)
		{
			const uint32_t p_i = f.first, v_i = f.second;
			for (uint32_t i = 0; i < v_i; ++i) e /= p_i;

			uint32_t g1 = powmod(2, e, p);
			while (g1 != 1)
			{
				g1 = powmod(g1, p_i, p);
				e *= p_i;
			}
		}

		return e;
	}
};

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
		Number number;
		for (uint32_t p = 3; p <= p_max; p += 2)
		{
			if (!isprime(p)) continue;
			const uint32_t o = number.order2(p);
			Pos pos; pos.p = p; pos.o = o; pos.s = new uint32_t[p];
			pos.s[0] = uint32_t(-1);
			for (uint32_t k = 1; k < p; ++k)
			{
				uint32_t k2pnmodp = k;
				pos.s[k] = uint32_t(-1);
				for (uint32_t n = 0; n < o; ++n)
				{
					if (k2pnmodp == p - 1) { if (pos.s[k] == uint32_t(-1)) pos.s[k] = n; else std::cout << "ERROR" << std::endl; }
					k2pnmodp *= 2;
					k2pnmodp -= (k2pnmodp >= p) ? p : 0;
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
			const uint32_t kmodp = k % p;
			const uint32_t n = po.s[kmodp];
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
		std::cout << "         Estimates are computed with p <= p_max (default 10000)" << std::endl;
		std::cout << "         and with n <= n_max is the max value (default 100000)." << std::endl;
		return EXIT_FAILURE;
	}

	uint32_t k_min = uint32_t(std::atoll(argv[1])), k_max = uint32_t(std::atoll(argv[2]));
	if (k_min < 3) k_min = 3;
	if (k_min % 2 == 0) k_min += 1;
	if (k_max % 2 == 0) k_max -= 1;
	const uint32_t p_max = (argc > 3) ? uint32_t(std::atoll(argv[3])) : 10000;
	const uint32_t n_max = (argc > 4) ? uint32_t(std::atoll(argv[4])) : 100000;

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
