/*
Copyright 2024, Alexander Jones

weight is free source code, under the MIT license (see LICENSE). You can redistribute, use and/or modify it.
Please give feedback to the authors if improvement is realized. It is distributed in the hope that it will be useful.

weight computes the 'weight' of the sequence k * 2^n + 1 for a range of k's.
See http://yves.gallot.pagesperso-orange.fr/papers/weight.pdf.
*/

#include <cstdint>

#include "modalg.h"

uint64_t modinv(uint64_t a, uint64_t p) {

	/* thanks to the folks at www.mersenneforum.org */
	/* borrowed from yafu by Ben Buhrow, under public domain. */

	uint64_t ps1, ps2, parity, dividend, divisor, rem, q, t;


	q = 1;
	rem = a;
	dividend = p;
	divisor = a;
	ps1 = 1;
	ps2 = 0;
	parity = 0;

	while (divisor > 1) {
		rem = dividend - divisor;
		t = rem - divisor;
		if (rem >= divisor) {
			q += ps1; rem = t; t -= divisor;
			if (rem >= divisor) {
				q += ps1; rem = t; t -= divisor;
				if (rem >= divisor) {
					q += ps1; rem = t; t -= divisor;
					if (rem >= divisor) {
						q += ps1; rem = t; t -= divisor;
						if (rem >= divisor) {
							q += ps1; rem = t; t -= divisor;
							if (rem >= divisor) {
								q += ps1; rem = t; t -= divisor;
								if (rem >= divisor) {
									q += ps1; rem = t; t -= divisor;
									if (rem >= divisor) {
										q += ps1; rem = t;
										if (rem >= divisor) {
											q = dividend / divisor;
											rem = dividend % divisor;
											q *= ps1;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		q += ps2;
		parity = ~parity;
		dividend = divisor;
		divisor = rem;
		ps2 = ps1;
		ps1 = q;
	}

	if (parity == 0)
		return ps1;
	else
		return p - ps1;
}

uint64_t divmod(uint64_t a, uint64_t b, uint64_t p) {
	const uint64_t bInv = modinv(b, p);
	return (a * bInv) % p;
}
