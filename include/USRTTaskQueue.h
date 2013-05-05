#ifndef USRT_Task_Queue_H
#define USRT_Task_Queue_H
#include <map>
#include <string>
#include <capability.h>
#include <CPBuffer.h>
#include <usrttype.h>
#include <USRTFifo.h>

namespace std {
  class USRTTaskQueue : public USRTFifo {
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
      void lpTask2G(generalized_memory_t* g, task_t* pt);
      void init();
      int insert( struct structHeap& h, task_t* a );
      void down(struct structHeap& h, int index );
      void up(struct structHeap& h, int index );
      task_t* pop( struct structHeap& h );
      raw_spinlock_t criticalArea;
    public:
      USRTTaskQueue( const char* name );
      void attach( const char* name );
      USRTTaskQueue();
      void start();
      ~USRTTaskQueue(){};
      int heapCheck(struct structHeap& h, int debug );
      int insert( generalized_memory_t* a );
      task_t* pop( ){ return pop( ready ); };
      int update();
      void dumpTaskTime( task_t * a );
      void dumpHeap(struct structHeap& h);
      void storeHeap();
      void restoreHeap();
      utime_t getNow();
  };
}
#endif //USRT_Task_Queue_H

