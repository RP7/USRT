#ifndef USRT_WORKERS_H
#define USRT_WORKERS_H
#include <map>
#include <string>
#include <capability.h>
#include <CPBuffer.h>
#include <usrttype.h>
#include <USRTFifo.h>

namespace std {
  class workers : public USRTFifo {
    #define HEAPSIZE 256
    typedef int(*FuncCompare)(task_t*,task_t*);
    public:
      struct structHeap {
        int size;
        raw_spinlock_t lock;
        task_t* heap[HEAPSIZE];
        FuncCompare func;
      };
      struct structHeap wait;
      struct structHeap ready;
      struct structWorkersHead {
        struct structFifoHead fifo;
        int readySize;
        generalized_memory_t gmReady[HEAPSIZE];
        int waitSize;
        generalized_memory_t gmWait[HEAPSIZE];
      } *head;
    private:
      task_t card;
      std::map <int64,CCapability *> mCapabilities;
      void lpTask2G(generalized_memory_t* g, task_t* pt);
      void init();
    public:
      workers( const char* name );
      void attach( const char* name );
      workers();
      void start();
      ~workers(){};
      task_t* pop( struct structHeap& h );
      int insert( struct structHeap& h, task_t* a );
      int insert( struct structHeap& h, generalized_memory_t* a );
      void down(struct structHeap& h, int index );
      void up(struct structHeap& h, int index );
      int heapCheck(struct structHeap& h, int debug );
      void dumpTaskTime( task_t * a );
      void dumpHeap(struct structHeap& h);
      void storeHeap();
      void restoreHeap();
  
#if 0
      int init( int argc, char *argv[] );
      runAsThread();
      bindCpu();
#endif
  };
}
#endif

