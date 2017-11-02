// Title:: Implementation of Bounded Buffer
// Date :: 10/24/2017
#include "bounded_buffer.H"

// Includes
#include <iostream>
// Redoing this in C because fuck C++ 


using namespace std;

BoundedBuffer::BoundedBuffer(int d_size): sem(Semaphore(1)),
                                                           empty(Semaphore(1)),
                                                           full(Semaphore(1)){
  // Need to initialize our semaphores
  this->pos = -1;
  this->size = d_size;
  // Create our buffers
  this->data.resize(d_size);
  // Do any additional setup that might be required

}

// We might want to look into a way to sleep until the thing is no longer full
int BoundedBuffer::Pop_On(string data){
  this->sem.Wait();
  // Wait until we have control of a non full buffer
  // This seems poorly written. I would rather have something that can
  // work with a stack to implement priority to prevent starvation
  // TODO Starvation proof BoundedBuffer
  while(this->pos >= (this->size-2)){
   // Release this semaphore
    this->sem.Post();
    // Wait until the semaphore is not full
    this->full.Wait();
    // Get back the Semaphore
    this->sem.Wait(); 
  }
  this->data[this->pos+1] = data;
  // Check if we were empty and need to do something about that
  if (this->pos == -1){
    pos++;
    this->empty.Post();
  }else{
    // Otherwise we just increment the position pointer
    pos++;
  }
  this->sem.Post();
  return 1;
}
string BoundedBuffer::Pop_Off(void){
  this->sem.Wait();
  // Wait until the buffer is not empty
  // And we have control over the data
  while(this->pos < 0){
    // The buffer is empty
    // So we need to wait until it is not empty
    this->sem.Post();
    // Wait until we are not empty
    this->empty.Wait();
    // Gimme back that sweet sweet control
    this->sem.Wait();
  }
  string val = this->data[this->pos];
  
  //check if we are going from full --> not full    
  if (this->pos <= (this->size-1)){
    // decrement position and release the full semaphore
    this->pos--;
    this->full.Post();
  }else{
    // Otherwise we continue normally
    this->pos--;
  }
  // No value was returned so we do not decrement the value
  // Instead we just return everything
  this->sem.Post();
  return val;
}

// OH NO WE"RE GOING DOWN
BoundedBuffer::~BoundedBuffer(){
  // Add any additional freedom here
}





