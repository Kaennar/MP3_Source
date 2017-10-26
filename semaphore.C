// Implementation of Semaphore.H


// Includes
#include "semaphore.H"
// pthread_mutex_t m
// pthread_cond_t c

Semaphore::Semaphore(int _val){
  // Initialize the mutex
  // Condition to Unlock 
  // and the value
  value = _val;
  pthread_cond_init(&this->c, NULL);
  pthread_mutex_init(&this->m, NULL);
}

Semaphore::~Semaphore(){
  pthread_cond_destroy(&this->c);
  pthread_mutex_destroy(&this->m);
}

// Post
int Semaphore::P(void){
  // Lock the semaphore
  pthread_mutex_lock(&this->m);
  this->value--;
  if (this->value <= 0){
    // Need to signal our mutex and be done with it
    pthread_cond_signal(&this->c);
  
  }
  pthread_mutex_unlock(&this->m);
  return this->value;
}

// Wati
int Semaphore::V(void){
  // Wait
  pthread_mutex_lock(&this->m);
  this->value++;
  
  if (this->value <= 0){
    // signal that this is ready
    pthread_cond_wait(&this->c, &this->m); 
  }
  pthread_mutex_unlock(&this->m);
  
  return this->value;
}


