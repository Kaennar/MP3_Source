// Title:: Bounded Buffer
// Date :: 10/24/2017

// Includes
#include <string>
#include "semaphore.H"
// Data structure for popping data on and off of a stack with thread safety (some at least :) )


// This is because I'm forced to use this disgusting language.
using namespace std;


// Type for the data
template <typename T>
class BoundedBuffer {
  private:
    Semaphore sem;
    Semaphore empty;
    Semaphore full;
    int pos;
    int size;
    T* data;
  public:
    // Constructor
    BoundedBuffer(int d_size);
    
    // Returns 1 if successful, 0 if it is not
    int Pop_On(T data);
    // R
    T Pop_Off(void); 
    
    
    // Destructor
    ~BoundedBuffer();
  


};




