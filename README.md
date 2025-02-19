## DaphneBenchmark
This is an extended part of the [DaphneBenchmark Suite](https://github.com/esa-tu-darmstadt/daphne-benchmark). As mentioned within the paper, we are specifically interested in the eucledian clustering implementation. Download dataset of any size from the original repositiry or use the medium size dataset in this.

Run: ``https://github.com/mittalswastik/ttex_benchmark.git``

### T-Tex Profiler Updates

Similar to the T-Tex pass, update the paths in the following lines within the T-Tex profiler

1. [Data Path from Pass to Profiler](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/ttex_pass_update.cpp#L8)
2. [Data Path from Profiler to Pass](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/ttex_pass_update.cpp#L1402)

### Execute as root

T-Tex executes ioctl calls to interact with the kernel. Hence, for convenience we start with the root user \
``sudo -i``

### Compilation Phase1

* Phase1 of T-Tex does not provide any analysis from the profiler to the llvm-pass
  * Update [Makefile](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/Makefile#L30) - Remove the check-file & set-val plugin
* Update the paths in the Makefile for [clang](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/Makefile#L10)
* Either copy omp.h & omp_tools.h (generally in ~/llvm-project/build/projects/openmp/runtime/src/) generated after compiling llvm to the same directory as the benchmark or add path using -I flag
  * omp.h - OpenMP runtime library
  * omp-tools.h - OpenMP ompt profiler tool 
 
### Compilation Phase2-n

* Add the check-file plugin to allow the compiler to read profiler statistics

### Execution

* Compile and execute the Linux-kernel-Module [code](https://github.com/mittalswastik/ttex_kernel/tree/master) for timers and also kernel timer crediting.
* run ``export LD_LIBRARY_PATH=/path-to-llvm-lib:$LD_LIBRARY_PATH
  * Llvm lib path is generally ~/llvm-project/build/lib
* run ``./kernel > output_1.txt`` \
    ``cat output_1.txt | grep "##################################### evaluated time value in ns is" > output_time_1.txt`` \
    ``sed 's/[^0-9]*\([0-9]\+\)[^0-9]*/\1/g' output_time_1.txt  > ttex_1``
* For subsequent phases modify the compilation flag in the makefile as mentioned above
  * run ``chmod +x script``
  * ``./script``
  * Sometimes due to large data, there could be copy errors resulting in broken profiler statistics
    * Compile and run each phase individually like phase 1 to ensure successfull execution
  * [Boxplot](https://github.com/mittalswastik/ttex_benchmark/blob/master/boxplot_2) script can be used to generate graphs
    * Update according to the number of phases evaluated (if executed without the script)
    * run ``gnuplot boxplot_2`` or
    * run ``python3 boxplot_python``
   
### Testing various scenarios

After secruity evaluation is completed and code regions are generated.
* Uncomment (set-val)[https://github.ncsu.edu/smittal6/ttex_benchmark/blob/master/Makefile#L30] plugin to stop further evaluation
* Comment [ompt_processLogData](https://github.com/mittalswastik/ttex_benchmark/blob/75a9c89654c614d5d57fbd8a5f757d233b1ffc44/ttex_pass_update.cpp#L1626) for any further region generation

#### Testing attack scenarios 
 * Uncomment [spin_delays](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/ttex_pass_update.cpp#L364) in all the code regions to introduce spin attack of various intensity
   * Attack intensity in nanoseconds can be updated [here](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/ttex_pass_update.h#L35)
   * Execute application once after compiling and retrieve and save the timing values in a as mentioned above in [Execution](https://github.com/mittalswastik/ttex_benchmark/blob/master/README.md#execution)
     
#### Comparing to T-SYS
 * Add the t-sys plugin in the [Makefile](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/Makefile#L30), execute the multiphase approch to generate code regions and execution time analysis
 * Run the attack scenario as mentioned before
   
#### Comparing to with and without Kernel Timer Crediting
 * See [README](https://github.com/mittalswastik/ttex_kernel/blob/master/README.md)

#### Playing around with security parameters
[Makefile](https://github.com/mittalswastik/ttex_benchmark/blob/c8ba0f1a783428993f248cd4cd33815597282592/Makefile#L30) has various flags like splitval, threshold which can be updated for initial loop split and security threshold value respectively
