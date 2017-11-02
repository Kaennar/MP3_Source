
#include <vector>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include "bounded_buffer.H"
#include "reqchannel.H"

#include <cassert>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>


vector<BoundedBuffer> vec;
pthread_cond_t cond;
pthread_mutex_t mute;
string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
void* MakerThread1 (void* arg){
  // Setting up the request
  for (int i=0; i < 100 ; i++){
    vec.at(0).Pop_On(int2string(i));
  }
  pthread_exit(NULL);
}
void* MakerThread2 (void* arg){
  // Setting up the request
  for (int i=100; i < 200 ; i++){
    vec.at(0).Pop_On(int2string(i));
  }
  pthread_exit(NULL);
}
void* MakerThread3 (void* arg){
  // Setting up the request
  for (int i=0; i < 100 ; i++){
    vec.at(1).Pop_On(int2string(i));
  }
  pthread_exit(NULL);
}
void* MakerThread4 (void* arg){
  // Setting up the request
  for (int i=100; i < 200 ; i++){
    vec.at(1).Pop_On(int2string(i));
  }
  pthread_exit(NULL);
}

void* ConsumerThread1 (void* arg){
  // Setting up the request
  int ret = 200;
  while (ret > 2){
    std::string retval = vec.at(0).Pop_Off();
    std::cout << retval << std::endl << std::flush;
    ret--;
  }
  pthread_exit(NULL);
}

void* ConsumerThread2 (void* arg){
  // Setting up the request
  int ret = 200;
  while (ret > 2){
    std::string retval = vec.at(1).Pop_Off();
    std::cout << retval << std::endl << std::flush;
    ret--;
  }
  pthread_exit(NULL);
}

int main(){
  // Create two threads one that makes data another who reads it
  pthread_t maker1;
  pthread_t maker2;
  pthread_t maker3;
  pthread_t maker4;
  pthread_t reader;
  pthread_t reader1;
  pthread_cond_init(&cond, NULL);
  pthread_mutex_init(&mute,NULL);
  
  vec.push_back(BoundedBuffer(4));
  vec.push_back(BoundedBuffer(4));
  pthread_create(&maker1, NULL, MakerThread1,NULL);
  pthread_create(&maker2, NULL, MakerThread2,NULL);
  pthread_create(&maker3, NULL, MakerThread3,NULL);
  pthread_create(&maker4, NULL, MakerThread4,NULL);
  pthread_create(&reader, NULL, ConsumerThread1,NULL);
  pthread_create(&reader1, NULL, ConsumerThread2,NULL);
  usleep(1000000);
  std::cout << "Everything is Done!" << std::endl;
}

