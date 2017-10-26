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
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "bounded_buffer.H"
#include "reqchannel.H"

#define REQUEST_RATE 100 // Hz
#define FIRST_NAMES 20
#define LAST_NAMES 24
// Italian Suffiz == di and il
using namespace std;



// Global Variables
float cycle_time = (float)1/100; 

// Bounded_Buffers for each of the names
// We need a name generator!!!!


// Local Function Declaration

// Worker Thread Execution
void* worker_thread(void* arg){
  // Do worker thread work here
}

void* statistic_thread(void* args){
  // Do Satistic Thread work here
  //
  // Need to log incoming data and reception time
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
  

  

  // Sleep for a random amount of microseconds
  int u_time = rand()%100000;
  usleep(u_time);
}

string generate_name(){
  int suffix = rand() % 2;
  int firstname = rand() % FIRST_NAMES + 1;
  int lastname = rand() % LAST_NAMES + 1;
  // read through each and get the associated name 
  ifstream f_name_file;
  ifstream l_name_file;
  ifstream suff_file;
  
  f_name_file.open("./first_name.txt");
  l_name_file.open("./last_name.txt");
  suff_file.open("./suffixes.txt");
  int line_index = 0;
  std::string first = "";
  while(line_index < firstname){
    getline(f_name_file,first);
    line_index++;
  }
  line_index = 0;
  std::string last;
  while(line_index < lastname){
    getline(l_name_file,last);
    line_index++;
  }
  line_index = 0;
  std::string suff;
  while(line_index < suffix){
    getline(suff_file,suff);
    line_index++; 
  }
  f_name_file.close();
  suff_file.close();
  l_name_file.close();
  std::string retval = first + " " + suff + " " + last;
  return retval;
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
  // n = number of data requests per person
  // b = size of bounded buffer standard size
  // w = numbder of worker threads
  int data_requests_per_persons = 0;
  int bounded_buffer_size = 0;
  int num_worker_threads = 0;
  char c_opt;
  while ((c_opt = getopt(argc,argv,"n:b:w:")) != -1){
    // Switch-Case to determine the option
    switch(c_opt){
      case 'n':
        data_requests_per_persons = (int)atoi(optarg);
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
  // Now we generate 3 patients
  
  usleep(1000000);
}
