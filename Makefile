.DEFAULT_GOAL := all
.PHONY: all clean checkdata

-include Makefile.deps

CXXFLAGS=-O3
CXXFLAGS+= -std=c++11

all: kernel checkdata

kernel: main.o kernel.o 
	$(CXX) -fopenmp $^ -o $@

main.o: main.cpp
	$(CXX) -fopenmp -c $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)  $< -o $@

kernel.o: kernel.cpp
	$(CXX) -fopenmp -c $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

checkdata:
ifeq ($(wildcard data/ec_input.dat),)
	$(warning ec_input.dat not found. Did you forget to extract the test data?)
endif
ifeq ($(wildcard data/ec_output.dat),)
	$(warning ec_output.dat not found. Did you forget to extract the test data?)
endif

clean:
	rm -f kernel kernel.o main.o Makefile.deps

Makefile.deps:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)  -MM main.cpp *.cpp > Makefile.deps

