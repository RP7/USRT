#ifndef USRT_WORKERS_H
#define USRT_WORKERS_H
#include <map>
#include <string>
#include <capability.h>
#include <CPBuffer.h>
#include <usrttype.h>
#include <USRTMem.h>

namespace std {
  class workers : public USRTMem {
    #define HEAPSIZE 256
    typedef int(*FuncCompare)(task_t*,task_t*);
    struct structHeap {
      int size;
      raw_spinlock_t lock;
      task_t* heap[HEAPSIZE];
      FuncCompare func;
    };
    struct structWorkHead {
      struct structMemHead mem;
      struct structHeap wait;
      struct structHeap ready;
      task_t card;
    };
       
    private:
      std::map <int64,CCapability *> mCapabilities;
      void init();

    public:
      workers( const char* name );
      void attach( const char* name );
      workers();
      void start();
      ~workers();
      struct structWorkHead *head;
      task_t* pop( struct structHeap& h );
      int insert( struct structHeap& h, task_t* a );
      int insert( struct structHeap& h, generalized_memory_t* a );
      void down(struct structHeap& h, int index );
      void up(struct structHeap& h, int index );
      int heapCheck(struct structHeap& h, int debug );
      void dumpTaskTime( task_t * a );
      void dumpHeap(struct structHeap& h);  
#if 0
      int init( int argc, char *argv[] );
      runAsThread();
      bindCpu();
#endif
  };
}
#endif

