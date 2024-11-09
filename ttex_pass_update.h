#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <omp-tools.h>
#include <execinfo.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <omp.h>
#include <chrono>

using namespace std;

#define MAX_SPLIT 100000000
#define PERIOD_IN_NANOS (100UL * 1000000UL)
#define NSEC_PER_SEC 1000000000

#define max_threads 10
#define attack_delay 80000

#define omp_for_ref -1
#define omp_single_ref -2
#define omp_sections_ref 0

#define parallel_begin_id -2
#define thread_begin_id -6
#define parallel_end_id -3 
#define work_begin_id -4 
#define work_end_id -5  //  work end id can remain -5 as it does not provide an id in itself
#define sync_id -200
#define task_end_id -300
#define default_id -100

uint64_t global_id = 0;

// int *testing_integer;
// int **testing_integer_2;

int thread_priority = 10;
unsigned long int time_val_sec = 200000;
unsigned long int time_val_nsec = 1000*1000*1000;

typedef struct modified_timer{
        int32_t id;
        unsigned long int time_val;
} updated_timer;

typedef struct receive_timer {
  int32_t id;
  unsigned long int time_val_to_sub;
  unsigned long int time_val;
} receive_timer;

#define START_TIMER _IOW('S',2,int32_t*) // start the timer
#define MOD_TIMER _IOW('U',3,int32_t*) // modify the timer
#define DEL_TIMER _IOW('D',3,int32_t*) // delete the timer
#define MOD_TIMER_NEW _IOW('UT',3,updated_timer*)
#define GET_TIMER _IOWR('R',3,receive_timer*) // retreive the current time from the kernel

static ompt_get_thread_data_t ompt_get_thread_data;
static ompt_get_parallel_info_t ompt_get_parallel_info;
static ompt_get_unique_id_t ompt_get_unique_id;
static ompt_enumerate_states_t ompt_enumerate_states;

typedef struct timespec timespec;
timespec start_time, end_time;

timespec max_timeout; // on callback work end thread can sleep or work but unknown so no time noted
timespec vuln_threshold;

typedef struct timeout_node {
  int parallel_region_id;
  int sub_region_id;
  int sections_id;
  int loop_id;
  bool timer_value_set;
  timespec wcet;
  timespec et; // actual time taken
  bool timer_set_flag;
} timeout_node;

typedef struct details{ 
  int ref; // -2 means omp for and >0 means omp section and -1 means single
  // int splits_in_iter;
  // int sub_region_id; // there can be multiple regions of same type
  //vector<timeout_node> expected_execution; // vector for sections and second vector for sub_timer_id
  timeout_node* expected_execution;
} details;

details **parallel_region;
//std::vector< std::vector<details> > parallel_region;

typedef struct loop_details {
  // int splits; // number of calls (max count / split factor in the pass) // no need as every split call would have the same wcet (same priority thread executing same code)
  // int loop_id;
  // int splits_in_iter;
  // int sub_loop_id;
  timeout_node expected_execution; // vector based on sub timer id 
} loop_details;

//std::vector< std::vector<loop_details> > loop_execution;
loop_details **loop_execution;

typedef struct thread_info {
  int id;
  int pid;
  int fd;
  int sid; // stores the work id for additiional callbacks ... if this is 0 then the additional callback is in parallel region outside any directive
  ompt_data_t* data;
  vector<timeout_node> thread_current_timeout;
} thread_info;

typedef struct loop_details_pass {
  int parallel_id;
  int loop_id;
  int split_factor;
  int unique_loop_id;
  int seq_split;
  long int total_inst;
  unsigned long int wcet_ns;
  int total_threads;
  int fns;
  int unique_function_ids[500];
} loop_details_pass;

typedef struct para_details {
  int parallel_id;
  int id;
  int ref;
  int seq_split;
  long int total_inst;
  unsigned long int wcet_ns;
  int total_threads;
  int fns;
  int unique_function_ids[500];
} para_details;

typedef struct loop_seq_details {
  int loop_split;
  int seq_code_split;
  int total_inst;
  unsigned long int wcet_ns;
} loop_seq_details;

typedef struct para_seq_details {
  int seq_code_split;
  int total_inst;
  unsigned long int wcet_ns;
} para_seq_details;

loop_seq_details loop_seq_profiler;
para_seq_details para_seq_profiler;

timeout_node loop_seq;
timeout_node para_seq;

int parallel_size;

loop_details_pass** l_data;
int *l_data_size;
// std::vector< std::vector<loop_details_pass> > l_data;
// std::vector< std::vector<para_details> > p_data;
para_details** p_data;
int *p_data_size;
// predefine timeout nodes for parallel_begin, end, thread_begin, end --- these region should execute in similar time irrespective of anything

timeout_node parallel_begin;
timeout_node parallel_end;
timeout_node thread_begin; // thread end would be thread end no timeout needed
timeout_node work_begin; // only for threads which do not get any task assigned (only in case of sections though)
timeout_node work_end;
timeout_node sync_region;
timeout_node task_end;

unordered_map<int, vector<timeout_node> > log_data; // assuming max 100 threads

bool logdata = true; // first execution requires logging and subsequent execution do not -- this will basically be false for measuring execution time

void readFileData();
void resetTimer(timespec, int);
receive_timer receiveTime(int, timespec);
extern "C" int my_core_id();
extern "C" int my_next_id();
timespec getRegionElapsedTime(timespec, timespec);
timespec timespec_normalise(timespec);
timespec timespec_higher(timespec, timespec);
bool timespec_compare(timespec, timespec);