CXX = g++
FLAGS = -O2 -std=c++20
INC = -I./primesieve/include/
LIBS = -lm

PRIMESIEVE_OBJS = ./primesieve/src/api.o ./primesieve/src/CountPrintPrimes.o ./primesieve/src/CpuInfo.o ./primesieve/src/Erat.o \
	./primesieve/src/EratSmall.o ./primesieve/src/EratMedium.o ./primesieve/src/EratBig.o ./primesieve/src/iterator.o \
	./primesieve/src/IteratorHelper.o ./primesieve/src/LookupTables.o ./primesieve/src/MemoryPool.o ./primesieve/src/PrimeGenerator.o \
	./primesieve/src/nthPrime.o ./primesieve/src/ParallelSieve.o ./primesieve/src/popcount.o ./primesieve/src/PreSieve.o \
	./primesieve/src/PrimeSieve.o ./primesieve/src/RiemannR.o ./primesieve/src/SievingPrimes.o

.PHONY: all

all: weight

weight: weight.o WeightSieve.o $(PRIMESIEVE_OBJS)
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(FLAGS) $(INC) -c -o $@ $<

clean:
	rm -f weight *.o ./primesieve/src/*.o
