/* 
    File: simpleclient.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2013/01/31

    Simple client main program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "bounded_buffer.H"
#include "reqchannel.H"

using namespace std;
vector<BoundedBuffer> writer_buff;
vector<BoundedBuffer>  stat_buffer;
bool patient_channels_alive[3];
struct Worker_Args{
  int worker_id;
};

struct Request_Args{
  int total_req_num;
  std::string name;
  std::string o_file;
};

struct Statistics_Args{
  std::string o_file;
  int id_num;
  int total_req_num;
};
int start_time;

// Global Variables

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int lookup_id_for_name(string name){
  if (name == "John Smith"){
    return 0;
  }else if (name == "Jane Smith"){
    return 1;
  }else{
    return 2;
  }
}

// Local Function Declaration

float curr_time(){
  return (float((clock() - start_time)/CLOCKS_PER_SEC));
}


// Worker Thread Execution
void* worker_thread(void* arg){
  // Do worker thread work here
  struct Worker_Args* args = (struct Worker_Args*) arg;
  string channel_id = "worker" + int2string(args->worker_id);
  RequestChannel channel(channel_id, RequestChannel::Side::CLIENT_SIDE);
  // Arg will pass our request channel
  while (patient_channels_alive[0] || patient_channels_alive[1]
        || patient_channels_alive[2]) {  
    for(int i=0; i< 3; i++){
      if (patient_channels_alive[i]){
        // Loop through each patient buffer
        string request = writer_buff[i].Pop_Off();
        // write it to the server
        string ret_val = channel.send_request(request);
        if (ret_val == "bye"){
          // Send a notification out that this channel decided to die 
          patient_channels_alive[i] = false;
        }else{
          stat_buffer[i].Pop_On(ret_val); 
        }
      }
    }
  }
  // All request channels have shutdown and we can now destroy ourselves
  pthread_exit(NULL); 

  //Do Destruction stuff
}

void* statistic_thread(void* args){
  // Do Statistic's things
  struct Statistics_Args* arg = (struct Statistics_Args*)args;
  int recv_reqs = 0;
  // Initialize our output file
  ofstream o_file(arg->o_file);
  o_file << "Data,TimeSinceStart" << endl; 
  // Run through each request 
  while (recv_reqs < arg->total_req_num){
    // get the request  
    string req_dat = stat_buffer[arg->id_num].Pop_Off();
    float currTime = curr_time();
    o_file << req_dat + "," << currTime << endl;
  }
  // Finished all of the requests so we destroy ourselves
  pthread_exit(NULL);
}

/*
 *  Name passed as an argument
 *  Outputs to a file with an associated number
 *  Runs 1 request at a time and then sleeps
 *
 *
 */
void* request_thread(void* args){
  // Do Request Thread work here
    
  struct Request_Args* arg = (struct Request_Args*) args;
  int sent_requests = 0;
  int id = lookup_id_for_name(arg->name);
  ofstream o_file(arg->o_file);
  o_file << "TimeSinceStart" << endl;
  string request_string = "data " + arg->name;
  while (sent_requests < arg->total_req_num){
    o_file << curr_time() << endl; 
    writer_buff[id].Pop_On(request_string);
  }
  // Now we send the quit command
  writer_buff[id].Pop_On("quit");
  cout << "So Long and thanks for all the fish" << endl;
  pthread_exit(NULL);
  // Destroy ourselves

}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
  // n = number of data requests per person
  // b = size of bounded buffer standard size
  // w = numbder of worker threads
  patient_channels_alive[0] = true;
  patient_channels_alive[1] = true;
  patient_channels_alive[2] = true;

  int total_requests = 0;
  int bounded_buffer_size = 0;
  int num_worker_threads = 0;
  char c_opt;
  while ((c_opt = getopt(argc,argv,"n:b:w:")) != -1){
    // Switch-Case to determine the option
    switch(c_opt){
      case 'n':
        total_requests = (int)atoi(optarg);
        break;
      case 'b':
        bounded_buffer_size = (int)atoi(optarg);
        break;
      case 'w':
        num_worker_threads = (int)atoi(optarg);
        break;
      case '?':
        printf("Unrecognized argument: %c ",c_opt);
        break;
      default:
        printf("Unrecognized argument: %c ",c_opt);
        break;
    }
  }
  pthread_t threads[num_worker_threads + 6];
  //Make the buffers
  for (int i=0 ; i<3; i++){
    writer_buff.push_back(BoundedBuffer(bounded_buffer_size));
    stat_buffer.push_back(BoundedBuffer(bounded_buffer_size));
  }
  // Now we generate 3 patients
  string patient1 = "Joe Smith"; 
  string o_file_stat1 = "JoeSmithStat.csv";
  string o_file_req1  = "JoeSmithReq.csv";
   
  string patient2 = "Jane Smith"; 
  string o_file_stat2 = "JaneSmithStat.csv";
  string o_file_req2  = "JaneSmithReq.csv"; 
  
  string patient3 = "John Doe"; 
  string o_file_stat3 = "JohnDoeStat.csv";
  string o_file_req3  = "JohnDoeReq.csv";
  
  // Make the request channels and Statistic Channels
  struct Statistics_Args pat1_stat = {o_file_stat1,0,total_requests};
  struct Statistics_Args pat2_stat = {o_file_stat2,1,total_requests};
  struct Statistics_Args pat3_stat = {o_file_stat3,2,total_requests};
  
  struct Request_Args pat1_req = {total_requests,patient1,o_file_req1};
  struct Request_Args pat2_req = {total_requests,patient2,o_file_req2};
  struct Request_Args pat3_req = {total_requests,patient3,o_file_req3};
  start_time = clock(); 
  // Generate the statistic channels
  pthread_create(&threads[0], NULL, statistic_thread,(void*)&pat1_stat);
  pthread_create(&threads[1], NULL, statistic_thread,(void*)&pat2_stat);
  pthread_create(&threads[2], NULL, statistic_thread,(void*)&pat3_stat);
  // Generate the request threads 
  pthread_create(&threads[3], NULL, request_thread,(void*)&pat1_req);
  pthread_create(&threads[4], NULL, request_thread,(void*)&pat2_req);
  pthread_create(&threads[5], NULL, request_thread,(void*)&pat3_req);
  struct Worker_Args worker_arguments[num_worker_threads];
  for (int i=6; i < num_worker_threads; i++){
    worker_arguments[i] = {i-6};
    pthread_create(&threads[i], NULL, worker_thread,(void*)&worker_arguments[i-6]); 
  }
  // Now lets build some worker threads
  // Start Clock right before the time
  while (patient_channels_alive[0] || patient_channels_alive[1] || 
          patient_channels_alive[2]){
    // Just loop till we are done
  }
  usleep(1000000);
}
