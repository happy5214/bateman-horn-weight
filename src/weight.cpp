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

#include "arg_parser.h"
#include "WeightSieve.h"

static std::string invocation_name = "weight";

static void print_help() {
	std::cout << "Usage: " << invocation_name << " k_min k_max [-p prime_max] [-n s_max] [-b base] [--riesel]" << std::endl;
	std::cout << "         Compute the weights for odd k_min <= k <= k_max." << std::endl;
	std::cout << "         Estimates are computed with p <= prime_max (default 20000)" << std::endl;
	std::cout << "         and with n <= n_max is the max value (default 50000)." << std::endl;
}

int main(int argc, char * argv[]) {
	if (argc > 0) {
		invocation_name = argv[0];
	}

	// Parse arguments
	const Arg_parser::Option options[] = {
		{ 'p', "prime-max", Arg_parser::yes },
		{ 'n', "sieve-max", Arg_parser::yes },
		{ 'b', "base",      Arg_parser::yes },
		{ 'r', "riesel",    Arg_parser::no  },
		{ 'h', "help",      Arg_parser::no  },
		{   0, 0,           Arg_parser::no  }
	};

	const Arg_parser parser( argc, argv, options );
	if (parser.error().size()) {
		std::cerr << "Argument error: " << parser.error() << std::endl;
		return 1;
	}

	uint32_t base = 2;
	uint64_t primeMax = 20000;
	uint64_t nMax = 50000;
	int8_t c = 1;

	int argind;

	for (argind = 0; argind < parser.arguments(); ++argind ) {
		const int code = parser.code(argind);
		if (!code) {
			break;
		}
		switch (code) {
			case 'p': primeMax = uint64_t(std::stoll(parser.argument(argind))); break;
			case 'n': nMax = uint64_t(std::stoll(parser.argument(argind))); break;
			case 'b': base = uint32_t(std::stoi(parser.argument(argind))); break;
			case 'r': c = -1; break;
			case 'h': print_help(); return 0;
			default :
				std::cerr << "Uncaught option: " << code << std::endl;
		}
	}

	std::string kMinStr = parser.argument( argind++ );
	if (kMinStr.empty()) {
		kMinStr = "2";
	}
	std::string kMaxStr = parser.argument( argind++ );
	if (kMaxStr.empty()) {
		kMaxStr = "1000000";
	}

	uint64_t kMin = uint64_t(std::stoll(kMinStr));
	uint64_t kMax = uint64_t(std::stoll(kMaxStr));
	if (kMin < 2) kMin = 2;
	if (kMin % base == 0) kMin += 1;
	if (kMax % base == 0) kMax -= 1;

	std::cout << "Initializing prime list..." << std::endl;
	WeightSieve weight(primeMax, nMax, base, c);

	std::ofstream pFile("weight.txt");
	if (!pFile.is_open()) return 2;

	std::cout << "Computing weights..." << std::endl;
	std::cout << std::setprecision(4);
	for (uint64_t k = kMin; k <= kMax; k++) {
		if (k % base == 0) continue;
		const double w = weight.value(k);
		pFile << k << " " << w << std::endl;
		if (k % 65536 == 1) std::cout << k * 100.0 / kMax << "%     \r";
	}
	std::cout << std::endl;

	pFile.close();
	std::cout << "Wrote file 'weight.txt'." << std::endl;

	return 0;
}
