#include "ttex_pass_update.h"

void ompt_readFileData(){

  std::cout<<"Loading 0"<<std::endl;

  // l_data.clear();
  // p_data.clear();

  printf("Loading\n");

  std::vector< std::vector<loop_details_pass> > l_data_temp;
  std::vector< std::vector<para_details> > p_data_temp;

  std::ifstream inFile("/home/swastik/dev/ttex_benchmark/data_log.txt", std::ios::binary);

  std::cout<<"file data read"<<std::endl;

    if (inFile) {
        // Read the data from the file
        std::cout<<"file data read"<<std::endl;
        size_t vectorSizeRow;
        inFile.read(reinterpret_cast<char*>(&vectorSizeRow), sizeof(vectorSizeRow));
        l_data_temp.resize(vectorSizeRow);
        for (auto& row : l_data_temp) {
          size_t vectorSizeColumn;
          inFile.read(reinterpret_cast<char*>(&vectorSizeColumn), sizeof(vectorSizeColumn));
          row.resize(vectorSizeColumn);
          for (auto& cell : row) {
              inFile.read(reinterpret_cast<char*>(&cell), sizeof(loop_details_pass));
          }
        }

        std::cout<<"read the loop details"<<std::endl;

        size_t vectorSize2Row;
        inFile.read(reinterpret_cast<char*>(&vectorSize2Row), sizeof(vectorSize2Row));
        p_data_temp.resize(vectorSize2Row);
        for (auto& row : p_data_temp) {
          size_t vectorSize2Column;
          inFile.read(reinterpret_cast<char*>(&vectorSize2Column), sizeof(vectorSize2Column));
          row.resize(vectorSize2Column);
          for (auto& cell : row) {
              inFile.read(reinterpret_cast<char*>(&cell), sizeof(para_details));
          }
        }

        inFile.read(reinterpret_cast<char*>(&loop_seq_profiler), sizeof(loop_seq_details));
        inFile.read(reinterpret_cast<char*>(&para_seq_profiler), sizeof(para_seq_details));

        std::cout<<"read the para details"<<std::endl;

        inFile.close();
        

        parallel_size = l_data_temp.size();
        l_data_size = (int*) malloc(l_data_temp.size() * sizeof(int));
        p_data_size = (int*) malloc(p_data_temp.size() * sizeof(int));

        std::cout<<"allocated sizes: "<<p_data_temp.size()<<std::endl;

        // for (const auto& item : l_data_temp) {
        //   for(const loop_details_pass& item: item) {
        //     std::cout<<"loop id: " << item.loop_id <<"\n";
        //     std::cout<<"Parallel id: " << item.parallel_id <<"\n";
        //     std::cout<<"split factor: " << item.split_factor << "\n";
        //     std::cout<<"seq id: " << item.seq_split <<"\n";
        //     std::cout<<"wcet_ns: " << item.wcet_ns <<"\n";
        //     std::cout<<"total instructions: " <<item.total_inst<<"\n";
        //   }
        // }

        // for (const auto& item : p_data_temp) {
        //   for(const para_details& item_2: item) {
        //     std::cout<<"sub id: " << item_2.id <<"\n";
        //     std::cout<<"Parallel id: " << item_2.parallel_id <<"\n";
        //     std::cout<<"split factor: " << item_2.seq_split << "\n";
        //     std::cout<<"total instructions: " <<item_2.total_inst<<"\n";
        //     std::cout<<"wcet_ns: " << item_2.wcet_ns <<"\n";
        //     std::cout<<"region id is: " <<item_2.ref<<"\n";
        //   }
        // }

        std::cout<<"=================================================================================================="<<std::endl;
        std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        std::cout<<"=================================================================================================="<<std::endl;

        std::cout<<"parallel region details printed: \n"<<std::endl;

        l_data = (loop_details_pass**) malloc(l_data_temp.size() * sizeof(loop_details_pass*));
        for(int i = 0 ; i < l_data_temp.size() ; i++) {
        //  std::cout<<"size of l_data_temp is:"<<l_data_temp[i].size()<<std::endl;
          l_data[i] = (loop_details_pass*) malloc(l_data_temp[i].size() * sizeof(loop_details_pass));
          l_data_size[i] = l_data_temp[i].size();
          //std::cout<<"-------------------"<<l_data_temp[i].size()<<"-------------"<<std::endl;
          for(int j = 0 ; j < l_data_temp[i].size() ; j++){
            l_data[i][j] = l_data_temp[i][j];
            // std::cout<<"print"<<std::endl;
            //std::cout<<"loop id:" << l_data_temp[i][j].loop_id <<"\n";
            // std::cout<<"Parallel id:" << l_data_temp[i][j].parallel_id <<"\n";
            // std::cout<<"split factor:" << l_data_temp[i][j].split_factor << "\n";
            // std::cout<<"seq id:" << l_data_temp[i][j].seq_split <<"\n";
            //std::cout<<"total instructions:" << l_data_temp[i][j].total_inst<<"\n";
          }
        }

        std::cout<<"assigned values to l_data"<<std::endl;

        p_data = (para_details**) malloc(p_data_temp.size() * sizeof(para_details*));
        for(int i = 0 ; i < p_data_temp.size() ; i++) {
          p_data[i] = (para_details*) malloc(p_data_temp[i].size() * sizeof(para_details));
          p_data_size[i] = p_data_temp[i].size();
          for(int j = 0 ; j < p_data_temp[i].size() ; j++){
            //std::cout<<"-------------------"<<p_data_temp[i].size()<<"-------------"<<std::endl;
            p_data[i][j] = p_data_temp[i][j];
            //std::cout<<"print"<<std::endl;
            // std::cout<<"sub id:" << p_data_temp[i][j].id <<"\n";
            // std::cout<<"Parallel id:" << p_data_temp[i][j].parallel_id <<"\n";
            // std::cout<<"seq id:" << p_data_temp[i][j].seq_split <<"\n";
            // std::cout<<"total instructions:" << p_data_temp[i][j].total_inst<<"\n";
          }
        }

        std::cout<<"assigned values to p_data"<<std::endl;

        printf("size of all the things: %d, %d, %d, %d, %d, %d, %d\n", sizeof(l_data), sizeof(*l_data), sizeof(l_data[0]), sizeof(*(l_data[0])), sizeof(loop_details_pass**), sizeof(loop_details_pass*), sizeof(loop_details_pass));
        printf("size of loop details after reading file: parallel size: %d  first parallel size: %d\n", sizeof(*l_data)/sizeof(loop_details_pass**),sizeof(*(l_data[0]))/sizeof(loop_details_pass));
        std::cout<<std::endl;
        //inFile.read(reinterpret_cast<char*>(l_data.data()), vectorSize * sizeof(loop_details_pass));

        // // // Print the read data
        // for (const auto& item : l_data_temp) {
        //   for(const loop_details_pass& item_2: item) {
        //     std::cout<<"loop id:" << item_2.loop_id << std::endl;
        //     std::cout<<"Parallel id:" << item_2.parallel_id << std::endl;
        //     std::cout<<"split factor:" << item_2.split_factor << std::endl;
        //     std::cout<<"seq id:" << item_2.seq_split << std::endl;
        //   }

        //   std::cout<<std::endl;
        // }

        // for (const auto& item : p_data_temp) {
        //   for(const para_details& item_2: item) {
        //     std::cout<<"ref:" << item_2.ref << std::endl;
        //     std::cout<<"Parallel id:" << item_2.parallel_id << std::endl;
        //     std::cout<<"id:" << item_2.id << std::endl;
        //     std::cout<<"seq id:" << item_2.seq_split << std::endl;
        //     std::cout<<"region id:" << item_2.ref << std::endl;
        //   }

        //   std::cout<<std::endl;
        // }

    } else {
        std::cerr << "Error opening the file for reading." << std::endl;
    }
}

void resetTimer(timespec t, int fd){ 
  //timer_settime(temp->thread_timer_id,0,&temp->thread_timer, NULL);
  int32_t id = syscall(__NR_gettid);
  //printf("scheduling cpu is %d for id %d\n: ", sched_getcpu(),id);
  //printf("modification thread id: %d\n", temp_thread_data->id);
  updated_timer test;
  test.id = id;
  test.time_val = t.tv_nsec/1000000;
  ioctl(fd, MOD_TIMER_NEW, &test);
  //printf("modification call made\n");
}

receive_timer receiveTime(int fd, timespec t){
  receive_timer rtime;
  rtime.id = syscall(__NR_gettid);
  rtime.time_val_to_sub = t.tv_sec*1000*1000*1000 + t.tv_nsec;
  ioctl(fd, GET_TIMER, &rtime);
  return rtime;
}

extern "C" int my_core_id() // to assign unique id's to thread (openmp might assign an id of finished thread to another)
{
  static uint64_t ID=0;
  int ret = (int) __sync_fetch_and_add(&ID,1);
  //assert(ret<MAX_THREADS && "Maximum number of allowed threads is limited by MAX_THREADS");
  return ret;
}

extern "C" int my_next_id() // to assign unique id's to thread (openmp might assign an id of finished thread to another)
{
  static uint64_t ID=0;
  int ret = (int) __sync_fetch_and_add(&ID,1);
  //assert(ret<MAX_THREADS && "Maximum number of allowed threads is limited by MAX_THREADS");
  return ret;
}

timespec getRegionElapsedTime(timespec start, timespec stop)
{
  timespec elapsed_time;
  if ((stop.tv_nsec - start.tv_nsec) < 0)
  {
    elapsed_time.tv_sec = stop.tv_sec - start.tv_sec - 1;
    elapsed_time.tv_nsec = stop.tv_nsec - start.tv_nsec + 1000000000;
  }
  else
  {
    elapsed_time.tv_sec = stop.tv_sec - start.tv_sec;
    elapsed_time.tv_nsec = stop.tv_nsec - start.tv_nsec;
  }
  return elapsed_time;
}

timespec timespec_normalise(timespec ts)
{
  while(ts.tv_nsec >= NSEC_PER_SEC)
  {
    ++(ts.tv_sec);
    ts.tv_nsec -= NSEC_PER_SEC;
  }
  
  while(ts.tv_nsec <= -NSEC_PER_SEC)
  {
    --(ts.tv_sec);
    ts.tv_nsec += NSEC_PER_SEC;
  }
  
  if(ts.tv_nsec < 0 && ts.tv_sec > 0)
  {
    /* Negative nanoseconds while seconds is positive.
     * Decrement tv_sec and roll tv_nsec over.
    */
    
    --(ts.tv_sec);
    ts.tv_nsec = NSEC_PER_SEC - (-1 * ts.tv_nsec);
  }
  else if(ts.tv_nsec > 0 && ts.tv_sec < 0)
  {
    /* Positive nanoseconds while seconds is negative.
     * Increment tv_sec and roll tv_nsec over.
    */
    
    ++(ts.tv_sec);
    ts.tv_nsec = -NSEC_PER_SEC - (-1 * ts.tv_nsec);
  }
  
  return ts;
}

timespec timespec_higher(timespec ts1, timespec ts2) {
    ts1 = timespec_normalise(ts1);
    ts2 = timespec_normalise(ts2);

    if(ts1.tv_sec > ts2.tv_sec){
        return ts1;
    }

    else if(ts1.tv_sec < ts2.tv_sec){
        return ts2;
    }

    else if(ts1.tv_nsec > ts2.tv_nsec){
        return ts1;
    }

    else {
        return ts2;
    }
}

bool timespec_compare(timespec ts1, timespec ts2){
    ts1 = timespec_normalise(ts1);
    ts2 = timespec_normalise(ts2);

    if(ts1.tv_nsec == ts2.tv_nsec && ts1.tv_sec == ts2.tv_sec){
        return true;
    }

    else {
        return false;
    }
}

timespec timespec_add(timespec ts1, timespec ts2)
{
  /* Normalise inputs to prevent tv_nsec rollover if whole-second values
   * are packed in it.
  */
  ts1 = timespec_normalise(ts1);
  ts2 = timespec_normalise(ts2);
  
  ts1.tv_sec  += ts2.tv_sec;
  ts1.tv_nsec += ts2.tv_nsec;
  
  return timespec_normalise(ts1);
}

timespec timespec_sub(timespec ts1, timespec ts2)
{
  /* Normalise inputs to prevent tv_nsec rollover if whole-second values
   * are packed in it.
  */
  ts1 = timespec_normalise(ts1);
  ts2 = timespec_normalise(ts2);
  
  ts1.tv_sec  -= ts2.tv_sec;
  ts1.tv_nsec -= ts2.tv_nsec;
  
  return timespec_normalise(ts1);
}

timespec returnKernelSubTime(int fd, timespec test){
  timespec temp;
  //clock_gettime(CLOCK_MONOTONIC, &temp);
  receive_timer t1 = receiveTime(fd, test);
  temp.tv_sec = t1.time_val / NSEC_PER_SEC;
  temp.tv_nsec = t1.time_val % NSEC_PER_SEC;

  return temp;
  //timespec temp_2  = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  //temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = getRegionElapsedTime(temp_2,temp);
}

////////////////////////////////////////////////////////////////////////

// static void countCounter (thread_data *ptr)
// {
//   __sync_add_and_fetch(&(ptr->node->counter),1);
// }

////////////////////////////////////////////////////////////////////////

extern "C" void on_ompt_callback_ompt_test(int parallel_region_id, int sub_id, int loop_id, int val)
//void  ompt_test(int parallel_region_id, int sub_id, int loop_id)
{
  // if(parallel_region_id == -1 && sub_id == -1 && loop_id == -1){
  //   return 0;
  // }

  //printf("Testing OMPT Test\n");

  //printf("OMPT TEST CALLED %d %d %d\n\n",parallel_region_id,sub_id,loop_id);

  ompt_data_t *current_thread = ompt_get_thread_data();
  //std::cout<<"current thread"<<std::endl;
  thread_info* temp_thread_data = (thread_info*) current_thread->ptr;

  timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);

  // if(temp_thread_data->id == 0){
  //   printf("=============================== attack delay ================================\n");
  //   struct timespec deadline;
  //   clock_gettime(CLOCK_MONOTONIC, &deadline);
  //   deadline.tv_nsec += 10000;
  //   if(deadline.tv_nsec >= 1000000000) {
  //     deadline.tv_nsec -= 1000000000;
  //     deadline.tv_sec++;
  //   }
  //   clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&deadline,NULL);
  // }

  // random_device rd;
  // mt19937 gen(rd());
  // uniform_int_distribution<> dis(0, 1);

  // int r = dis(gen);

  // if(r==0){
    // auto desiredDuration = std::chrono::nanoseconds(60000);  // Adjust the duration as needed

    // // Get the current time point
    // auto start = std::chrono::high_resolution_clock::now();

    // // Spin until the desired duration is reached
    // while (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start) < desiredDuration) {
    //     // Your spinning logic goes here
    //     // This loop will keep running until the desired duration is reached
    // }
  //}

  
  //std::cout <<"----- OMPT TEST PARALLEL ID FOUND IS: "<<temp_thread_data->data->value<<std::endl;
  if(parallel_region_id != temp_thread_data->pid && temp_thread_data->pid != -1){
    if(sub_id == -1){ 
      int loop_unique_id = l_data[parallel_region_id][loop_id].unique_loop_id;
      for(int i = 0 ; i < l_data_size[parallel_region_id]; i++){
        if(l_data[temp_thread_data->pid][i].unique_loop_id == loop_unique_id){
          loop_id = i;
          break;
        }
      } /*This is for the scenario where same loop with another parallel region is called - however no way to know new loop id*/
    }

    else if (loop_id == -1) {
      if(temp_thread_data->sid != -1){
        sub_id = temp_thread_data->sid; // loop was needed for loops as loop id is passed by compiler not evaluated at runtime like region id
                                        // so need to evalaute which loop can it be
      }

      // I think something is needed if sid = -1 - it would mean it does not belong to any parallel region and function is called sequentially so can maintain another logic for it
      // for now can we exit this function?
    }

    parallel_region_id = temp_thread_data->pid;
  }


  //std::cout <<"----- OMPT TEST PARALLEL ID NOT FOUND USING PARAMETER: "<< std::endl;
  if(temp_thread_data->pid == -1){
    /**TO DO*/
    // push back time for sequential region outside parallel region - // maybe we won't need sid now
    //timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
    //temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
    if(loop_id == -1){
      temp_thread_data->thread_current_timeout.push_back(para_seq);
    }

    else {
      // loop_seq.loop_id = loop_id;
      temp_thread_data->thread_current_timeout.push_back(loop_seq);
    }
    //temp_thread_data->thread_current_timeout.push_back(loop_execution[parallel_region_id][loop_id].expected_execution);
    // change the above to handle the sequential time
  }

  else {
    // timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
    // temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);

    if(loop_id != -1){
      temp_thread_data->thread_current_timeout.push_back(loop_execution[parallel_region_id][loop_id].expected_execution);
    }

    else {
      parallel_region[parallel_region_id][sub_id].expected_execution[0].loop_id = val;
      temp_thread_data->thread_current_timeout.push_back(parallel_region[parallel_region_id][sub_id].expected_execution[0]);
    }
  }


  //printf("OMPT TEST CALLED %d %d %d %dn\n",parallel_region_id,sub_id,loop_id,val);

  // do not need to check previous timer set or not in this case

  int temp_para_id = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].parallel_region_id;
  // if(temp_para_id > 10){
  //   std::cout<<"=====================------------------ parallel region id pushed in is ------------============"<<temp_para_id<<" "<<parallel_region_id<<" "<<loop_id<<std::endl;
  // }
  
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
  //temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].sub_region_id = temp_thread_data->counter-1;
  //clock_gettime(CLOCK_MONOTONIC, &temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et);
  
  timespec test;
  test.tv_sec = 0;
  test.tv_nsec = 0;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd,test);
  resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
  //get parallel id here

  // std::cout<<"================================================================="<<std::endl;
  // std::cout<<std::endl;
  // std::cout<<std::endl;
  // for(int i = 0 ; i < temp_thread_data->thread_current_timeout.size() ; i++){
  //   std::cout<<temp_thread_data->thread_current_timeout[i].parallel_region_id<<" ";
  // }
  // std::cout<<std::endl;
  // std::cout<<std::endl;
  // std::cout<<"================================================================="<<std::endl;

}


extern "C"  void
on_ompt_callback_thread_begin(
  ompt_thread_t thread_type,
  ompt_data_t *thread_data)
{
  std::cout<<"----------------------- thread begin ---------------------"<<std::endl;
  std::cout<<"thread begin data is : "<<thread_begin.parallel_region_id<<std::endl;
  // thread_data->value = my_next_id();
  thread_info* temp_thread_data = (thread_info*) calloc(1,sizeof(thread_info));
  temp_thread_data->id = my_next_id();
  temp_thread_data->sid = -1;
  temp_thread_data->data = NULL;
  if(temp_thread_data->id == 0){
    temp_thread_data->pid = -1; // this thread begin has executed before parallel begin
  }

  std::cout<<"----------------------- thread begin 1_2---------------------"<<std::endl;
  temp_thread_data->thread_current_timeout = std::vector<timeout_node>();
  std::cout<<"thread data time out log size is : "<<temp_thread_data->thread_current_timeout.size()<<std::endl;
  std::cout<<"----------------------- thread begin 2---------------------"<<std::endl;
  if(temp_thread_data->thread_current_timeout.size() == 0){
    std::cout<<"----------------------- size empty ---------------------"<<std::endl;
  }
  temp_thread_data->thread_current_timeout.push_back(thread_begin);
  //createTimer(temp_thread_data);
  
  int core_id;
  std::cout<<"----------------------- thread begin ---------------------"<<std::endl;
  core_id = my_core_id()%6;

  std::cout<<"core used is :"<<core_id<<std::endl;

  pthread_t thread;
  pthread_attr_t attr;
  struct sched_param param;
  int policy;

  thread = pthread_self();

  // Initialize thread attributes
  pthread_attr_init(&attr);

  int result = pthread_getschedparam(thread, &policy, &param);
  if (result != 0) {
    std::cout<<"scheduling policy not retreived"<<std::endl;
  }

  else {
    std::cout<<"scheduling policy priority is :"<<param.sched_priority<<std::endl; 
  }

  // Set thread attributes to make it a real-time thread
  // pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  // 

  // Set the desired priority for the thread
  param.sched_priority = thread_priority;
  policy = SCHED_FIFO;
  //pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  pthread_setschedparam(thread, policy, &param); //pthread_attr_setschedparam is used before pthread_create

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

  //thread_info* temp_thread_data = (thread_info*) malloc(sizeof(thread_info));

  // printf("----------------------- thread begin ---------------------\n");
  int fd = open("/dev/etx_device", O_RDWR);
  temp_thread_data->fd = fd;
  //temp_thread_data->id = syscall(__NR_gettid);
  if(temp_thread_data->fd < 0) {
    std::cout<<"Cannot open device file..."<<std::endl;
    return;
  }

  thread_data->ptr = temp_thread_data;  // storing the thread data such that accessible to all events

  int32_t id = syscall(__NR_gettid);
  std::cout<<"Thread id and core is"<<id<<" "<<core_id<<std::endl;

  // thread_data->value = my_next_id();
  ioctl(temp_thread_data->fd, START_TIMER, &id);

  std::cout<<"thread data value "<<temp_thread_data->id<<std::endl;

  thread_data->ptr = temp_thread_data;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
  //clock_gettime(CLOCK_MONOTONIC, &temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et);

  timespec temp;
  temp.tv_sec = 0;
  temp.tv_nsec = 0;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
  // if(temp_thread_data->id == 0)
  //sleep(8);
  // printf("-------------------------- thread begin ends ------------------------\n");
}

extern "C" void
on_ompt_callback_parallel_begin(
  ompt_data_t *encountering_task_data,
  const ompt_frame_t *encountering_task_frame,
  ompt_data_t *parallel_data,
  unsigned int requested_parallelism,
  int flags,
  const void *codeptr_ra,
  unsigned int id)
{
  std::cout<<"parallel id sent is:" <<id<<std::endl;
  // uint64_t tid = ompt_get_thread_data()->value;
  ompt_data_t *current_thread = ompt_get_thread_data();
  parallel_data->value = id-1;//ompt_get_parallel_info();
  std::cout<<"Error checking 1"<<std::endl;
  thread_info* temp_thread_data = (thread_info*) current_thread->ptr;
  temp_thread_data->pid = (int) parallel_data->value;
  temp_thread_data->sid = 0; // first region of the code
  std::cout<<"Parallel region id is parallel region: "<<parallel_data->value<<std::endl;
  //para_id_map[parallel_data->value] = id-1; // id I send from clang starts from 1
  //printf("thread data value: %d\n", ((thread_info*) current_thread)->current->parallel_region_id);

  /*Un comment the two lines below if */

  timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  
    /**TODO*/
  // can change to below once considering parallel begin as sub region 0  
  //temp_thread_data->thread_current_timeout.push_back(parallel_region[id-1][0].expected_execution[0]);
  //temp_thread_data->thread_current_timeout.push_back(parallel_begin);
  std::cout<<"ref value is:"<<parallel_region[0][0].ref<<std::endl;
  temp_thread_data->thread_current_timeout.push_back(parallel_region[parallel_data->value][0].expected_execution[0]);
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].parallel_region_id = parallel_data->value;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].sub_region_id = 0;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].loop_id = 991;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
  //clock_gettime(CLOCK_MONOTONIC, &temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et);

  temp.tv_sec = 0;
  temp.tv_nsec = 0;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
  std::cout<<"-------- end of parallel begin -----------"<<std::endl;
}

extern "C"  void
on_ompt_callback_work(
  ompt_work_t wstype,
    ompt_scope_endpoint_t endpoint,
    ompt_data_t *parallel_data,
    ompt_data_t *task_data,
    uint64_t count,
    const void *codeptr_ra,
    unsigned int sub_parallel_id)
{

  ompt_data_t *current_thread = ompt_get_thread_data();
  thread_info* temp_thread_data = (thread_info*) current_thread->ptr;

  timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);

  temp_thread_data->pid = parallel_data->value;

  // temp_thread_data->data = parallel_data;

  if(endpoint == 1){ // start of work

    temp_thread_data->sid = sub_parallel_id;

    //printf("start of work, parallel id, sub_parallel_id%d %d\n", parallel_data->value, sub_parallel_id-1);
    std::cout<<"Thread id is **************** "<<temp_thread_data->id<<std::endl;
    std::cout<<"Sub region is **************" <<sub_parallel_id<<std::endl;
    std::cout<<"Display section count to confirm if changed: "<<count<<std::endl;
    std::cout<<"=============== ref is: "<<parallel_region[parallel_data->value][sub_parallel_id].ref<<std::endl;

    // timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
    // temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  
    vector<timeout_node> temp_node;

    bool max_timer = false;

    if(parallel_region[parallel_data->value][sub_parallel_id].ref > omp_sections_ref && count != -1){ // -1 count means thread enters but not takes any section will go to work end
      std::cout<<"count value is:"<<count<<std::endl;
      temp_thread_data->thread_current_timeout.push_back(parallel_region[parallel_data->value][sub_parallel_id].expected_execution[count-1]);
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].loop_id = 992;
    }

    else if (parallel_region[parallel_data->value][sub_parallel_id].ref == omp_for_ref){
      std::cout<<"value is----------"<<std::endl;
      temp_thread_data->thread_current_timeout.push_back(parallel_region[parallel_data->value][sub_parallel_id].expected_execution[0]); // only one vector in other case
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].loop_id = 992;
      //resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
    }

    else if (parallel_region[parallel_data->value][sub_parallel_id].ref == omp_single_ref){
      std::cout<<"--------------ompt single is detected---------"<<parallel_data->value<<" "<<sub_parallel_id<<std::endl;
      temp_thread_data->thread_current_timeout.push_back(parallel_region[parallel_data->value][sub_parallel_id].expected_execution[0]); // only one vector in other case
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].loop_id = 992;
      //resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
    }

    else {
      //work_begin.loop_id = 993;
      temp_thread_data->thread_current_timeout.push_back(work_begin);
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
      temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].loop_id = 993;
      max_timer = true;
      //resetTimer(max_timeout, temp_thread_data->fd); // thread will directly enter callback work end or sleep
      
      /* have to add implcit barrier callback waitime here and in implcit barrier reset to maxtimeout*/
    }

    temp.tv_sec = 0;
    temp.tv_nsec = 0;
    temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
    if (max_timer){
      resetTimer(max_timeout, temp_thread_data->fd);
    }

    else {
      resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
    }

    //sleep(5);

    std::cout<<"checking for error"<<std::endl;

    // for(int l = 0 ; l < temp_node.size() ; l++){
    //     temp_thread_data->thread_current_timeout.push_back(temp_node[l]);
    // }

   // maxvuln will pick the last id -- for nested parallelism
  }

  else {
    std::cout<<"--------------callback end---------"<<parallel_data->value<<" "<<sub_parallel_id<<std::endl;
    // work end - set the et of the current execution
    //clock_gettime(CLOCK_MONOTONIC, &temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et);

    // if(temp_thread_data->id == 4){
    //   sleep(3);
    // }
    std::cout<<"Thread id is **************** "<<temp_thread_data->id<<std::endl;
    std::cout<<"timer is been set for callback work"<<std::endl;
    temp_thread_data->thread_current_timeout.push_back(work_end);
    temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].parallel_region_id = parallel_data->value;
    //printf("----------------------------------------- sub region id detected is -----------%d\n",temp_thread_data->sid);
    temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].sub_region_id = temp_thread_data->sid;
    temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].loop_id = 995;
    //timespec temp;
    temp.tv_sec = 0;
    temp.tv_nsec = 0;
    temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
    resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
  }
}

extern "C" void
on_ompt_callback_sync_region(
  ompt_sync_region_t kind,
  ompt_scope_endpoint_t endpoint,
  ompt_data_t *parallel_data,
  ompt_data_t *task_data,
  const void *codeptr_ra)
{
  ompt_data_t *current_thread = ompt_get_thread_data();
  thread_info* temp_thread_data = (thread_info*) current_thread->ptr;

  // Can use kind to identify the type of wait .... but do not need it

  // if(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag){
  //   temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = false;
  //   timespec temp;
  //   receive_timer t = receiveTime(temp_thread_data->fd);
  //   temp.tv_sec = 0;
  //   temp.tv_nsec = t.time_val;
  //   timespec temp_2  = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  //   temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = getRegionElapsedTime(temp_2,temp);
  // }
  timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  temp_thread_data->thread_current_timeout.push_back(sync_region);
  temp.tv_sec = 0;
  temp.tv_nsec = 0;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  resetTimer(max_timeout, temp_thread_data->fd);
  // else {
  //   temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true; // sync region called again
  // }

  // resetTimer(sync_region.wcet,temp_thread_data->fd);
  // std::cout<<"Sync region called --- Thread id is ****************"<<temp_thread_data->id<<std::endl;

}


extern "C"  void
on_ompt_callback_parallel_end(
  ompt_data_t *parallel_data,
  ompt_data_t *encountering_task_data,
  int flags,
  const void *codeptr_ra)
{
  ompt_data_t *current_thread = ompt_get_thread_data();
  thread_info* temp_thread_data = (thread_info*) current_thread->ptr;
  
  timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  std::cout<<"--------------- end of parallel region -----------------------"<<parallel_data->value<<std::endl;

  // parallel_end.loop_id = 996;
  //temp_thread_data->thread_current_timeout.push_back(parallel_end);
  temp_thread_data->thread_current_timeout.push_back(para_seq);
  //temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].parallel_region_id = parallel_data->value;
  //temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].sub_region_id = temp_thread_data->sid;
  // clock_gettime(CLOCK_MONOTONIC, &temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et);
  //pthread_mutex_unlock(&lock_t);

  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].timer_set_flag = true;
  //clock_gettime(CLOCK_MONOTONIC, &temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et);
  temp_thread_data->sid = -1; // -1 as ompt_test uses this counter to identify regions
  temp_thread_data->pid = -1;
  temp.tv_sec = 0;
  temp.tv_nsec = 0;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  resetTimer(temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].wcet,temp_thread_data->fd);
  // if(temp_current_thread->id == 0)
   //sleep(10);
}

extern "C" void
on_ompt_callback_implicit_task(
  ompt_scope_endpoint_t endpoint,
  ompt_data_t *parallel_data,
  ompt_data_t *task_data,
  unsigned int actual_parallelism,
  unsigned int index,
  int flags
){
  ompt_data_t *current_thread = ompt_get_thread_data();
  thread_info* temp_thread_data = (thread_info*) current_thread->ptr;
  
  if(endpoint == 2) {
    printf("=============================== task end ================================%d\n", endpoint);
    printf("call back task end %d\n", temp_thread_data->id);

    timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
    temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
    temp_thread_data->thread_current_timeout.push_back(task_end);
    temp.tv_sec = 0;
    temp.tv_nsec = 0;
    temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
    resetTimer(max_timeout, temp_thread_data->fd);
  }
}

extern "C"  void
on_ompt_callback_thread_end(
  ompt_data_t *thread_data)
{
  ompt_data_t *current_thread = ompt_get_thread_data();
  thread_info* temp_thread_data = (thread_info*) current_thread->ptr;
  // if(temp_current_thread->id == 2)
  //  sleep(5);

  if(temp_thread_data->id != 0){
    temp_thread_data->pid = -1; // at thread end all worker threads are out of parallel begin
  }

  timespec temp = temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et;
  temp_thread_data->thread_current_timeout[temp_thread_data->thread_current_timeout.size()-1].et = returnKernelSubTime(temp_thread_data->fd, temp);
  
  if(logdata){
    //std::cout<<"thread id:"<<temp_thread_data->id<<std::endl; // removing this line gives seg fault ... why? was that the main problem thread_end in async callback
    log_data[temp_thread_data->id] = std::vector<timeout_node>();
    log_data[temp_thread_data->id] = temp_thread_data->thread_current_timeout;
  }

  std::cout<<"+++++++++++++++++++++++ log data size: "<<log_data[temp_thread_data->id].size()<<" "<<temp_thread_data->id;

  // thread_info* temp = thread_timeout_map[thread_data->value];
  // thread_timeout_map.erase(thread_data->value);

  // if(temp_thread_data->id == 0){
  //   for(int i = 0 ; i < parallel_region.size(); i++){
  //     for(int j = 0 ; j < parallel_region[i].size() ; j++){
  //       printf("reference info: %d\n", parallel_region[i][j].ref);
  //     }
  //   }
  // }

  // for (const auto & [ key, value ] : log_data) {
  //   printf("------------------------------------------------------------\n");
  //   printf("Thread id %d\n\n", key);
  //   for(int j = 0 ; j < value.size() ; j++){
  //     printf("Parallel region: %d\n", value[j].parallel_region_id);
  //     printf("Sub region: %d\n", value[j].sub_region_id);
  //     printf("sections region: %d\n", value[j].sections_id);
  //     printf("Loop Id: %d\n", value[j].loop_id);
  //     //printf("Sub Timer id: %d\n", value[j].sub_timer_id);
  //     //timespec temp_time = getRegionElapsedTime(start_time,value[j].et);
  //     printf("%d seconds and %ld nanoseconds have elapsed!", value[j].et.tv_sec, value[j].et.tv_nsec);
  //     printf("\n");
  //   }
  //   printf("\n\n\n");
  // }
}

#define register_callback_t(name, type)                       \
do {                                                           \
  type f_##name = &on_##name;                                 \
  if (ompt_set_callback(name, (ompt_callback_t)f_##name) ==   \
      ompt_set_never)                                         \
    printf("0: Could not register callback '" #name "'\n");   \
} while(0)

#define register_callback(name) register_callback_t(name, name##_t)

extern "C" void ompt_initializeTimeoutData(){

    // testing_integer = (int*) malloc(sizeof(int));
    // testing_integer_2 = (int**) malloc(3*sizeof(int*));
    // testing_integer_2[0] = (int*) malloc(2*sizeof(int));

    // testing_integer[0] = 2;
    // testing_integer_2[0][1] = 3;

  ompt_readFileData();

  std::cout<<"completed reading file data"<<std::endl;

  timespec exec_time;
  exec_time.tv_sec = 20;
  exec_time.tv_nsec = 0; // 1 sec wcet to everything for now

  // thread_begin is to initialize the time retreiving time value from the kernel

  max_timeout.tv_sec = time_val_sec;
  max_timeout.tv_nsec = time_val_nsec;

  vuln_threshold.tv_sec = 0;
  vuln_threshold.tv_nsec = 60000;

  parallel_begin.wcet = max_timeout;
  thread_begin.wcet = max_timeout;
  parallel_end.wcet = max_timeout;
  work_begin.wcet = max_timeout;
  work_end.wcet = max_timeout;
  sync_region.wcet = max_timeout;
  task_end.wcet = max_timeout;

  parallel_begin.timer_value_set = true;
  thread_begin.timer_value_set = true;
  parallel_end.timer_value_set = true;
  work_begin.timer_value_set = true;
  work_end.timer_value_set = true;
  sync_region.timer_value_set = true;
  task_end.timer_value_set = true;

  parallel_begin.parallel_region_id = parallel_begin_id;
  thread_begin.parallel_region_id = default_id;
  parallel_end.parallel_region_id = parallel_end_id;
  work_begin.parallel_region_id = default_id;
  work_end.parallel_region_id = default_id;
  sync_region.parallel_region_id = default_id;
  task_end.parallel_region_id = default_id;

  parallel_begin.sub_region_id = default_id;
  thread_begin.sub_region_id = thread_begin_id;
  parallel_end.sub_region_id = default_id;
  work_begin.sub_region_id = work_begin_id;
  work_end.sub_region_id = work_end_id;
  sync_region.sub_region_id = sync_id;
  task_end.sub_region_id = task_end_id;
  sync_region.loop_id = 997;
  task_end.loop_id = 998;

  parallel_begin.sections_id = default_id;
  thread_begin.sections_id = default_id;
  parallel_end.sections_id = default_id;
  work_begin.sections_id = default_id;
  work_end.sections_id = default_id;

  parallel_begin.loop_id = default_id;
  thread_begin.loop_id = default_id;
  parallel_end.loop_id = default_id;
  work_begin.loop_id = default_id;
  work_end.loop_id = default_id;

  parallel_begin.timer_set_flag = false;
  thread_begin.timer_set_flag = false;
  parallel_end.timer_set_flag = false;
  work_begin.timer_set_flag = false;
  work_end.timer_set_flag = false;
  sync_region.timer_set_flag = false;

  std::cout<<"Printing details: no. of parallel regions: "<<parallel_size<<std::endl;

  loop_execution = (loop_details**) malloc(parallel_size*sizeof(loop_details*));
  parallel_region = (details**) malloc(parallel_size*sizeof(details*)); 

  loop_seq.timer_set_flag = false;
  loop_seq.loop_id = -1;
  loop_seq.parallel_region_id = -1;
  loop_seq.sub_region_id = -1;
  loop_seq.timer_value_set = true;
  if(loop_seq_profiler.wcet_ns == 0){
    loop_seq.wcet = max_timeout;
  }

  else {
    timespec testing_time;
    testing_time.tv_sec = loop_seq_profiler.wcet_ns / 1000000000;
    testing_time.tv_nsec = loop_seq_profiler.wcet_ns % 1000000000;
    loop_seq.wcet = timespec_higher(testing_time,vuln_threshold);
  }

  para_seq.timer_set_flag = true;
  para_seq.timer_value_set = true;
  para_seq.loop_id = -1;
  para_seq.parallel_region_id = -1;
  para_seq.sub_region_id = -1;
  if(para_seq_profiler.wcet_ns == 0){
    para_seq.wcet = max_timeout;
  }

  else {
    timespec testing_time;
    testing_time.tv_sec = para_seq_profiler.wcet_ns / 1000000000;
    testing_time.tv_nsec = para_seq_profiler.wcet_ns % 1000000000;
    para_seq.wcet = timespec_higher(testing_time,vuln_threshold);
  }

  //loop_execution = std::vector < std::vector<loop_details> > (l_data.size(), std::vector<loop_details>());

  for(int i = 0 ; i < parallel_size; i++){

    //loop_execution[i] = std::vector<loop_details> (l_data[i].size());

    std::cout<<"Loops in parallel regions are "<<l_data_size[i]<<std::endl;

    loop_execution[i] = (loop_details*) malloc(l_data_size[i]*sizeof(loop_details));

    for (int j = 0 ; j < l_data_size[i]  ; j++) {
      
        timeout_node temp;
        if(l_data[i][j].wcet_ns == 0) { 
          temp.wcet = max_timeout;
        }

        else {
          timespec testing_time;
          testing_time.tv_sec = l_data[i][j].wcet_ns / 1000000000;
          testing_time.tv_nsec = l_data[i][j].wcet_ns % 1000000000;
          temp.wcet = timespec_higher(testing_time,vuln_threshold);
        }
        
        temp.timer_value_set = true;
        temp.sub_region_id = -1; // this will be updated by previous timeout sub_region_id , counter for now
        temp.parallel_region_id = i;
        temp.loop_id = j;
        temp.sections_id = default_id;
        temp.timer_set_flag = false;
        loop_execution[i][j].expected_execution = temp;
    }

  /* 

    added callback is an extensible idea ... if openmp supports different priority for sub regions this can be extended to have 
    different callback time for different subregions like work_callback

    Also thats the reason why we stick to openmp sub region specific timed analysis than basic block

    We support OpenMP extension of have real-time task in implicit task models

    Talk about all this in paper

  */

  //parallel_region = std::vector < std::vector<details> > (p_data.size(), std::vector<details>());

    parallel_region[i] = (details*) malloc(p_data_size[i]*sizeof(details)); 
    //parallel_region[i] = std::vector<details> (p_data[i].size());

    for(int j = 0 ; j < p_data_size[i] ; j++){ // j = 0 is parallel begin

      timeout_node temp;

      std::cout<<"ref region id: "<<p_data[i][j].ref<<std::endl;
      parallel_region[i][j].ref = p_data[i][j].ref;

      if(p_data[i][j].ref == -1000){ // parallel begin
        std::cout<<"Printing details for parallel begin"<<std::endl;
        if(p_data[i][j].wcet_ns == 0) { 
          temp.wcet = max_timeout;
        }

        else {
          timespec testing_time;
          testing_time.tv_sec = p_data[i][j].wcet_ns / 1000000000;
          testing_time.tv_nsec = p_data[i][j].wcet_ns % 1000000000;
          temp.wcet = timespec_higher(testing_time,vuln_threshold);
        }
       

        temp.timer_value_set = true;
        temp.sub_region_id = j;
        temp.parallel_region_id = i;
        temp.sections_id = default_id;
        temp.loop_id = -1;
        temp.timer_set_flag = false;
        printf("Printing details for parallel begin\n");
        parallel_region[i][j].expected_execution = (timeout_node*) malloc(sizeof(timeout_node));
        parallel_region[i][j].expected_execution[0] = temp;
        printf("Testing reference number %d\n", parallel_region[i][j].ref);
        printf("Testing code verification %ld\n", parallel_region[i][j].expected_execution[0].wcet.tv_sec);
        // continue;
      }
  
      else if(p_data[i][j].ref > omp_sections_ref){ //sections

          printf("Sections detected\n");
          parallel_region[i][j].expected_execution = (timeout_node*) malloc(p_data[i][j].ref*sizeof(timeout_node));
          for(int k = 0 ; k < p_data[i][j].ref ; k++){
              // here will be another loop here for max vul ... then temp_v will have more noes per section
              if(p_data[i][j].wcet_ns == 0) { 
                temp.wcet = max_timeout;
              }

              else {
                timespec testing_time;
                testing_time.tv_sec = p_data[i][j].wcet_ns / 1000000000;
                testing_time.tv_nsec = p_data[i][j].wcet_ns % 1000000000;
                temp.wcet = timespec_higher(testing_time,vuln_threshold);
              }

              temp.timer_value_set = true;
              temp.sub_region_id = j;
              temp.parallel_region_id = i;
              temp.sections_id = k;
              temp.loop_id = -1;
              temp.timer_set_flag = false;
              //parallel_region[i][j].expected_execution.push_back(temp);
              parallel_region[i][j].expected_execution[k] = temp;
          }
          // printf("checking parallel region impl ............... %d\n", parallel_region[i][j].expected_execution[1]->sections_id);
      }
    
      else if(p_data[i][j].ref == omp_single_ref) { // single
          if(p_data[i][j].wcet_ns == 0) { 
            temp.wcet = max_timeout;
          }

          else {
            timespec testing_time;
            // testing_time.tv_sec = p_data[i][j].wcet_ns / 1000000000;
            // testing_time.tv_nsec = p_data[i][j].wcet_ns % 1000000000;
            temp.wcet = vuln_threshold;
          }
          
          temp.timer_value_set = true;
          temp.sub_region_id = j;
          temp.parallel_region_id = i;
          temp.sections_id = default_id;
          temp.loop_id = -1;
          temp.timer_set_flag = false;
          //parallel_region[i][j].expected_execution.push_back(temp);
          parallel_region[i][j].expected_execution = (timeout_node*) malloc(sizeof(timeout_node));
          parallel_region[i][j].expected_execution[0] = temp;
      }

      else if(p_data[i][j].ref == omp_for_ref) { // omp for // *loop_split factor in the other case
      
          if(p_data[i][j].wcet_ns == 0) { 
            temp.wcet = max_timeout;
          }

          else {
            timespec testing_time;
            testing_time.tv_sec = p_data[i][j].wcet_ns / 1000000000;
            testing_time.tv_nsec = p_data[i][j].wcet_ns % 1000000000;
            temp.wcet = timespec_higher(testing_time,vuln_threshold);
          }
          temp.timer_value_set = true;
          temp.sub_region_id = j;
          temp.parallel_region_id = i;
          temp.sections_id = default_id;
          temp.loop_id = -1;
          temp.timer_set_flag = false;
          //parallel_region[i][j].expected_execution.push_back(temp);
          parallel_region[i][j].expected_execution = (timeout_node*) malloc(sizeof(timeout_node));
          parallel_region[i][j].expected_execution[0] = temp;
      }
    }
  }

  printf("printing details of parallel regions\n");
  std::cout<<std::endl;
  // for(int i = 0 ; i < parallel_region.size(); i++){
  //   for(int j = 0 ; j < parallel_region[i].size() ; j++){
  //     printf("reference info: %d\n", parallel_region[i][j].ref);
  //   }
  // }

}

extern "C" int ompt_initialize(
  ompt_function_lookup_t lookup,
  int initial_device_num,
  ompt_data_t *tool_data)
{
  // printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@ %d\n", parallel_region[2][0].parallel_id);

  ompt_set_callback_t ompt_set_callback = (ompt_set_callback_t) lookup("ompt_set_callback");
  ompt_get_thread_data = (ompt_get_thread_data_t) lookup("ompt_get_thread_data");
  ompt_get_parallel_info = (ompt_get_parallel_info_t) lookup("ompt_get_parallel_info");
  ompt_get_unique_id = (ompt_get_unique_id_t) lookup("ompt_get_unique_id");
  ompt_enumerate_states = (ompt_enumerate_states_t) lookup("ompt_enumerate_states");

  register_callback(ompt_callback_parallel_begin);
  register_callback(ompt_callback_parallel_end);
  register_callback(ompt_callback_thread_begin);
  register_callback(ompt_callback_thread_end);
  register_callback(ompt_callback_work);
  register_callback(ompt_callback_sync_region);
  register_callback(ompt_callback_ompt_test);
  register_callback(ompt_callback_implicit_task);
  //register_callback(ompt_callback_implicit_barrier);

  //bool flag = true;
  //ompt_test(-1,-1,-1);

  // pthread_mutex_init(&lock_t,NULL);
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  printf("Checking initial\n");
  std::cout<<std::endl;
  return 1; //success
}

void ompt_logDataToFile(){

  printf("Working logDataToFile\n");
  std::cout<<std::endl;

  //readFileData(); // not sure why we have to reevaluate again

  std::vector< std::vector<loop_details_pass> > l_data_temp;
  std::vector< std::vector<para_details> > p_data_temp;

  if(timespec_compare(loop_seq.wcet,max_timeout)){
    loop_seq_profiler.wcet_ns = 0;
  }

  else {
    loop_seq_profiler.wcet_ns = loop_seq.wcet.tv_sec*1000000000+loop_seq.wcet.tv_nsec;
  }

  if(timespec_compare(para_seq.wcet,max_timeout)){
    para_seq_profiler.wcet_ns = 0;
  }

  else {
    para_seq_profiler.wcet_ns = loop_seq.wcet.tv_sec*1000000000+loop_seq.wcet.tv_nsec;
  }

  for(int i = 0 ; i < parallel_size ; i++){
    std::vector<loop_details_pass> temp;
    for (int j = 0 ; j < l_data_size[i] ; j++) { 
      if(timespec_compare(loop_execution[i][j].expected_execution.wcet, max_timeout)){
        l_data[i][j].wcet_ns = 0; // this means that this region was not called at all in the code execution hence not needed to set any wcet values
      }

      else {
        l_data[i][j].wcet_ns = (loop_execution[i][j].expected_execution.wcet.tv_sec*1000000000)+(loop_execution[i][j].expected_execution.wcet.tv_nsec);
      }
      temp.push_back(l_data[i][j]);
      // std::cout<<"+++++++++++++ stored wcet in nsec is ++++++++++ "<<l_data[i][j].wcet_ns<<std::endl;
    }
    l_data_temp.push_back(temp);
  
    std::vector<para_details> temp_2;
    for(int j = 0 ; j < p_data_size[i] ; j++){
      std::cout<<p_data[i][j].ref<<std::endl;
      if(p_data[i][j].ref > 0){
       // std::cout<<"parallel region id for ref is: "<<p_data[i][j].parallel_id<<std::endl;
        for(int k = 0 ; k < p_data[i][j].ref ; k++){
          unsigned long int wcet_temp = 0;
          if(!timespec_compare(parallel_region[i][j].expected_execution[k].wcet, max_timeout)){
            wcet_temp = (parallel_region[i][j].expected_execution[k].wcet.tv_sec*1000000000)+(parallel_region[i][j].expected_execution[k].wcet.tv_nsec);
          }

          if(p_data[i][j].wcet_ns < wcet_temp){
            p_data[i][j].wcet_ns = wcet_temp;
          }
        }
      }

      else {
        //std::cout<<"parallel region id and sub id is: "<<p_data[i][j].parallel_id<<" "<<p_data[i][j].id<<std::endl;
        timeout_node temp = parallel_region[i][j].expected_execution[0];
       // std::cout<<"checking for existance of timeout ndoe"<<std::endl;
        if(timespec_compare(parallel_region[i][j].expected_execution[0].wcet, max_timeout)){
          p_data[i][j].wcet_ns = 0;
        }

        else {
          p_data[i][j].wcet_ns = (parallel_region[i][j].expected_execution[0].wcet.tv_sec*1000000000)+(parallel_region[i][j].expected_execution[0].wcet.tv_nsec);
        }
        std::cout<<"time in sec is: "<<parallel_region[i][j].expected_execution[0].wcet.tv_sec<<std::endl;
      }

      temp_2.push_back(p_data[i][j]);
    }

    p_data_temp.push_back(temp_2);
  }

  // std::ofstream outputFile2("/home/swastik/dev/ttex/benchmarks/parsec-3.0/output_2.txt");

  //   // Check if the file is opened successfully
  // if (!outputFile2.is_open()) {
  //     std::cerr << "Error opening the file." << std::endl;
  //     return; // Exit with an error code
  // }

  // std::cout<<"create file"<<std::endl;	

  // outputFile2 << "Time in ns\t\tParallel id\tSub id\tLoop id" << std::endl;

  // std::map< std::vector<int>, bool > map_wcet;

  // for (const auto & [ key, value ] : log_data) {
  //   //printf("------------------------------------------------------------\n");
  //   //printf("Thread id %d\n\n", key);
  //   for(int j = 0 ; j < value.size() ; j++){
  //      if(value[j].sub_region_id == thread_begin_id || value[j].sub_region_id == work_end_id || value[j].sub_region_id == sync_id || value[j].sub_region_id == task_end_id)
  //                   continue; // not logging the 1st iteration sub_id == -1
  //     // printf("Parallel region: %d\n", value[j].parallel_region_id);
  //     // printf("Sub region: %d\n", value[j].sub_region_id);
  //     // printf("sections region: %d\n", value[j].sections_id);
  //     // printf("Loop Id: %d\n", value[j].loop_id);
  //     // //printf("Sub Timer id: %d\n", value[j].sub_timer_id);
  //     // //timespec temp_time = getRegionElapsedTime(start_time,value[j].et);
  //     // printf("%d seconds and %ld nanoseconds have elapsed!", value[j].et.tv_sec, value[j].et.tv_nsec);
  //     // printf("\n");
  //     int p_id = value[j].parallel_region_id;
  //     int s_id = value[j].sub_region_id;
  //     int l_id = value[j].loop_id;
      
  //     std::vector<int> v{p_id, s_id, l_id};
  //     if(map_wcet.find(v) == map_wcet.end()){
  //       if(p_id != -1){
  //         if(s_id != -1){
  //           outputFile2 << p_data[p_id][s_id].wcet_ns << "\t\t" << p_id << "\t" << s_id << "\t" << l_id << "\t"<<key<<std::endl;
  //         }

  //         else {
  //           outputFile2 << l_data[p_id][l_id].wcet_ns << "\t\t" << p_id << "\t" << s_id << "\t" << l_id << "\t"<<key<<std::endl;
  //         }
  //       }

  //       map_wcet[v] = true;
  //     }
  //     //std::cout<< "### evaluated time in sec: " << value[j].et.tv_sec << std::endl;
      
  //     //std::cout<<"##################################### evaluated time value in ns is: "<< t<<"Parallel and sub id and loop id is: "<<value[j].parallel_region_id<<" "<<value[j].sub_region_id<<" "<<value[j].loop_id<<std::endl; //std::fixed << std::setprecision(2) <<t<<std::endl;
      
  //   }
  //   printf("\n\n\n");
  // }

  // outputFile2.close();

  std::cout<<"------------------- works till here --------------------"<<std::endl;

   std::ofstream outFile("/home/swastik/dev/ttex_benchmark/data_log_to_pass.txt",std::ios::binary);

    if (outFile) {
        std::cout<<"---------------- open output ---- file "<<std::endl;
        size_t num2Rows = l_data_temp.size();
        outFile.write(reinterpret_cast<const char*>(&num2Rows), sizeof(num2Rows));

        // Write each row's size and data
        for (const auto& row : l_data_temp) {
            size_t rowSize = row.size();
            outFile.write(reinterpret_cast<const char*>(&rowSize), sizeof(rowSize));
            for (const loop_details_pass& cell : row) {
                outFile.write(reinterpret_cast<const char*>(&cell), sizeof(loop_details_pass));
            }
        }

        // for (const auto& item : l_data_temp) {
        //   for(const loop_details_pass& item_2: item) {
        //     std::cout<<"loop id:" << item_2.loop_id <<"\n";
        //     std::cout<<"Parallel id:" << item_2.parallel_id <<"\n";
        //     std::cout<<"split factor:" << item_2.split_factor << "\n";
        //     std::cout<<"seq id:" << item_2.seq_split <<"\n";
        //     std::cout<<"wcet_ns: "<<item_2.wcet_ns <<"\n";
        //     std::cout<<"total instructions:" <<item_2.total_inst<<"\n";
        //   }

        //   std::cout<<std::endl;
        // }
        std::cout<<"----------------output loop written----------------"<<std::endl;
        // Write the number of rows
        size_t numRows = p_data_temp.size();
        outFile.write(reinterpret_cast<const char*>(&numRows), sizeof(numRows));

        // Write each row's size and data
        for (const auto& row : p_data_temp) {
            size_t rowSize = row.size();
            outFile.write(reinterpret_cast<const char*>(&rowSize), sizeof(rowSize));
            for (const para_details& cell : row) {
                outFile.write(reinterpret_cast<const char*>(&cell), sizeof(para_details));
            }
        }

        std::cout<<"----------------output parallel written----------------"<<std::endl;

        outFile.write(reinterpret_cast<const char*>(&loop_seq_profiler), sizeof(loop_seq_details));
        outFile.write(reinterpret_cast<const char*>(&para_seq_profiler), sizeof(para_seq_details));
        
        std::cout<<"----------------------- output written to the file back --------------------"<<std::endl;
        outFile.close();
    } else { 
        std::cout<< "Error opening the file for writing \n";
    }
}

void ompt_processLogData(){
    // retreive all loop ids != default number for each thread
    // retreive parallel ids of all those loops ids and
    // generate wcet based on highest time value and set is as WCET time value in loop data structure for parallel region


    // retreive all parallel ids and sub region ids for which thread begin etc is NULL
    // places similarly to the data structure above

    // generate wcet for thread begin and parallel begin in general and store everything in a file

    printf("checking before log loop\n");
    std::cout<<std::endl;

    for (const auto & [ key, value ] : log_data) {
      for(int j = 0 ; j < value.size() ; j++){
        if(value[j].parallel_region_id == -1){
          if(value[j].sub_region_id != -1){
            unsigned long int test = (value[j].et.tv_sec*1000*1000*1000) + (value[j].et.tv_nsec);
            if(loop_seq.timer_value_set){
              loop_seq.wcet = value[j].et;
              loop_seq.timer_value_set = false;
            }

            loop_seq.wcet = timespec_higher(loop_seq.wcet, value[j].et);
          }

          else {
            if(para_seq.timer_value_set){
              para_seq.wcet = value[j].et;
              para_seq.timer_value_set = false;
            }

            para_seq.wcet = timespec_higher(para_seq.wcet, value[j].et);
          }
        }
      }
    }

    for (const auto & [ key, value ] : log_data) {
      //std::cout<<"is there log_data"<<std::endl;
      for(int j = 0 ; j < value.size() ; j++){
        //std::cout<<"is there value to process "<<value.size()<<std::endl;
        if(value[j].sub_region_id == -1  && value[j].parallel_region_id != -1){

          unsigned long int test = (value[j].et.tv_sec*1000*1000*1000) + (value[j].et.tv_nsec);
          //std::cout<<"+++++++++++++ stored wcet in nsec is ++++++++++"<<test<<std::endl;
          //std::cout<<"loop region id:" << value[j].loop_id<<std::endl;
          //std::cout<<"loop id not a default id: "<<j<<std::endl;
          //std::cout<<"parallel region id: "<< value[j].parallel_region_id<<std::endl;
          if(loop_execution[value[j].parallel_region_id][value[j].loop_id].expected_execution.timer_value_set){
            //std::cout<<"this condition only executes once for each region"<<std::endl;
            loop_execution[value[j].parallel_region_id][value[j].loop_id].expected_execution.wcet = value[j].et;
            loop_execution[value[j].parallel_region_id][value[j].loop_id].expected_execution.timer_value_set = false;
          }

          //std::cout<<"error checking"<<std::endl;
          // if(timespec_compare(loop_execution[value[j].parallel_region_id][value[j].loop_id].expected_execution.wcet, max_timeout)){
          //   loop_execution[value[j].parallel_region_id][value[j].loop_id].expected_execution.wcet = value[j].et;
          // }

          loop_execution[value[j].parallel_region_id][value[j].loop_id].expected_execution.wcet = timespec_higher(loop_execution[value[j].parallel_region_id][value[j].loop_id].expected_execution.wcet, value[j].et);
          // process wcet if found higher one then set that as wcet
        }
      }
    }

    printf("checking log loop prints\n");
    std::cout<<std::endl;

    for (const auto & [ key, value ] : log_data) {
        for(int j = 0 ; j < value.size() ; j++){
            // consider single for and sections here
            if(value[j].sub_region_id != -1 && value[j].parallel_region_id != default_id && value[j].parallel_region_id != -1){
                int temp_id = value[j].sub_region_id;
                if(temp_id == parallel_end_id || temp_id == thread_begin_id || temp_id == work_end_id)
                    continue;

                
                printf("Value of sub-region id: %d and parallel id: %d and ref is %d\n", value[j].sub_region_id, value[j].parallel_region_id, parallel_region[value[j].parallel_region_id][value[j].sub_region_id].ref);
                std::cout<<std::endl;
                //printf("ref: %d\n", parallel_region[value[j].parallel_region_id][value[j].sub_region_id].ref);

                //printf("parallel id is: %d and sub region id is: %d\n",value[j].parallel_region_id,value[j].sub_region_id);   

                if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].ref > omp_sections_ref){
                  if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set){
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[value[j].sections_id].wcet = value[j].et;
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set = false;
                  }
                  parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[value[j].sections_id].wcet = timespec_higher(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[value[j].sections_id].wcet, value[j].et);
                }

                else if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].ref == omp_for_ref){
                  if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set){
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet = value[j].et;
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set = false;
                  }
                  parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet = timespec_higher(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet, value[j].et);
                  std::cout<<"value set for parallel and sub id: "<<value[j].sub_region_id<<" "<<value[j].parallel_region_id<<std::endl;
                }

                else if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].ref == omp_single_ref){
                  if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set){
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet = value[j].et;
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set = false;
                  }
                  parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet = timespec_higher(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet, value[j].et);
                }

                else if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].ref == -1000){
                  if(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set){
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet = value[j].et;
                    parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution->timer_value_set = false;
                  }
                  parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet = timespec_higher(parallel_region[value[j].parallel_region_id][value[j].sub_region_id].expected_execution[0].wcet, value[j].et);  
                }
            }
        }
    }

    printf("checking log parallel region prints\n");
    std::cout<<std::endl;
    ompt_logDataToFile();
}

extern "C" void ompt_finalize(ompt_data_t* data)
{
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  timespec total_t = getRegionElapsedTime(start_time,end_time);
  double total = total_t.tv_sec*1000 + total_t.tv_nsec/1000000;
  printf("Logging =++++++++++++++++++++++++++++++ %f\n\n\n\n\n\n\n", total);
  printf("%d\n", log_data.size());

  std::ofstream outputFile("/home/swastik/dev/ttex/benchmarks/parsec-3.0/output_1.txt");

    // Check if the file is opened successfully
  if (!outputFile.is_open()) {
      std::cerr << "Error opening the file." << std::endl;
      return; // Exit with an error code
  }
 
  std::cout<<"create file"<<std::endl;	

  outputFile << "Time in ns\t\tParallel id\tSub id\tLoop id" << std::endl;

  for (const auto & [ key, value ] : log_data) {
    //printf("------------------------------------------------------------\n");
    //printf("Thread id %d\n\n", key);
    for(int j = 0 ; j < value.size() ; j++){
       if(value[j].parallel_region_id == -1 || value[j].sub_region_id == thread_begin_id || value[j].sub_region_id == work_end_id || value[j].sub_region_id == sync_id || value[j].sub_region_id == task_end_id)
                    continue; // not logging the 1st iteration sub_id == -1
      // printf("Parallel region: %d\n", value[j].parallel_region_id);
      // printf("Sub region: %d\n", value[j].sub_region_id);
      // printf("sections region: %d\n", value[j].sections_id);
      // printf("Loop Id: %d\n", value[j].loop_id);
      // //printf("Sub Timer id: %d\n", value[j].sub_timer_id);
      // //timespec temp_time = getRegionElapsedTime(start_time,value[j].et);
      // printf("%d seconds and %ld nanoseconds have elapsed!", value[j].et.tv_sec, value[j].et.tv_nsec);
      // printf("\n");
      
      unsigned long int t = (value[j].et.tv_sec*1000*1000*1000) + (value[j].et.tv_nsec);
      outputFile << t << "\t\t" << value[j].parallel_region_id << "\t" << value[j].sub_region_id << "\t" << value[j].loop_id << "\t"<<key<<std::endl;
      //std::cout<< "### evaluated time in sec: " << value[j].et.tv_sec << std::endl;
      
      std::cout<<"##################################### evaluated time value in ns is: "<< t<<"Parallel and sub id and loop id is: "<<value[j].parallel_region_id<<" "<<value[j].sub_region_id<<" "<<value[j].loop_id<<" "<<key<<std::endl; //std::fixed << std::setprecision(2) <<t<<std::endl;
      
    }
    printf("\n\n\n");
  }

  ompt_processLogData();  

  printf("Checking final\n");
  std::cout<<std::endl;
}

extern "C" ompt_start_tool_result_t* ompt_start_tool(
  unsigned int omp_version,
  const char *runtime_version)
{
  //ompt_test(-1,-1,-1);
  ompt_initializeTimeoutData();
  static ompt_start_tool_result_t ompt_start_tool_result = {&ompt_initialize,&ompt_finalize,{.ptr=NULL}};
  return &ompt_start_tool_result;
}