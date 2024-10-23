.DEFAULT_GOAL := all
.PHONY: all clean checkdata

#-include Makefile.deps

# CXXFLAGS=-O3
# CXXFLAGS+= -std=c++11

# CC_HOME is installation root of the C compiler
export CC_HOME="/home/swastik/dev/ttex/llvm/ttex_implementation/build"
# BINUTIL_HOME is installation root of the GNU binutils
export BINUTIL_HOME="/home/swastik/dev/ttex/llvm/ttex_implementation/build"
# GNUTOOL_HOME is installation root of the GNU tools
export GNUTOOL_HOME="/home/swastik/dev/ttex/llvm/ttex_implementation/build"

# Compilers and preprocessors
export CC=${CC_HOME}/bin/clang -O3 -flto -D_LIBCPP_ABI_UNSTABLE -fno-exceptions
export CXX=${CC_HOME}/bin/clang++ -O3 -flto -D_LIBCPP_ABI_UNSTABLE -fno-exceptions
export CPP=${CC_HOME}/bin/clang-cpp -O3 -flto -D_LIBCPP_ABI_UNSTABLE -fno-exceptions
export CXXCPP=""

PORTABILITY_FLAGS=-I${CC_HOME}/projects/openmp/runtime/src -L${CC_HOME}/lib -static-libgcc -fopenmp -Wl,--hash-style=both,--as-needed

# Arguments to use
#-funroll-loops -fno-omit-frame-pointer -fprefetch-loop-arrays -fpermissive
export CFLAGS=${PORTABILITY_FLAGS}
export CXXFLAGS=${PORTABILITY_FLAGS}
# export CPPFLAGS=""
# export CXXCPPFLAGS=""
export LDFLAGS=-fuse-ld=gold -L${CC_HOME}/lib64 -L${CC_HOME}/lib -Wl,--plugin-opt=--ttex_update_3,--plugin-opt=--nthreads=6,--plugin-opt=--threshold=60000,--plugin-opt=--splitval=10000,--plugin-opt=--shared_threshold=25,--plugin-opt=--check_file#,--plugin-opt=--set_val#,--plugin-opt=--tsys"
# export LDFLAGS="-L${CC_HOME}/lib64 -L${CC_HOME}/lib"

all:	kernel checkdata

kernel:	ttex_pass_update.o	main.o	kernel.o
	$(CXX) -v $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -fuse-ld=gold $^ -o $@

main.o:	main.cpp
	$(CXX) -fopenmp -c $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

kernel.o:	kernel.cpp
	$(CXX) -fopenmp -c $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

ttex_pass_update.o:	ttex_pass_update.cpp ttex_pass_update.h
	$(CXX) $(CXXFLAGS) -c $(CFLAGS) $(CPPFLAGS) ttex_pass_update.cpp

checkdata:
ifeq ($(wildcard data/ec_input.dat),)
	$(warning ec_input.dat not found. Did you forget to extract the test data?)
endif
ifeq ($(wildcard data/ec_output.dat),)
	$(warning ec_output.dat not found. Did you forget to extract the test data?)
endif

clean:
	rm -f kernel kernel.o main.o Makefile.deps

#Makefile.deps:
#	$(CXX) $(CFLAGS) -O3 $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -I../include -MM main.cpp *.cpp > Makefile.deps

