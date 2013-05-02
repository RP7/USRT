#ifndef __USRT_Fifo_H
#define __USRT_Fifo_H
#include <usrttype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <CPBuffer.h>
#include <USRTMem.h>

class USRTFifo : public USRTMem {
  public:
    struct structFifoHead {
      struct structMemHead mem;
      raw_spinlock_t lockF;
      int sp;
      int rp;
      long long fifo[FIFOSIZE];
    };
  private:
    struct structFifoHead *head;
  public:
    USRTFifo();
    void newUSRTFifo( const char *n );
    void attach( const char *n );
    void newUSRTFifo( const char *n, long long dataL, long long cpL, long long resL );
    void init();
    ~USRTFifo();
    int len();
    void start();
    void dumpHead();
    void push( void *t );
    void* get();
    struct structFifoHead * getHead() {return head;};
};
#endif // __USRT_Fifo_H
