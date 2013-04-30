#ifndef __USRT_mem_H
#define __USRT_mem_H
#include <usrttype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

class USRTmem {
  private:
    struct structTaskMemHead {
      char name[256];
      raw_spinlock_t lockM;
      long long _brk;
      raw_spinlock_t lockS;
      int sp;
      int rp;
      long long stack[STACKSIZE];
    } *head;
    long long size;
    CPBuffer *buf;
    void _printStack( int bp );

  public:
    USRTmem( const char *n );
    void init();
    void setName( const char *n );
    char *getName();
    ~USRTmem();
    int len();
    void start();
    void *allocMem( long long len );
    void pushTask( task_t* task );
    int getTask( task_t* &ret );
    void printStack();
    void printStack( int len );
    void dumpHead();
};
#endif
