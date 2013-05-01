#ifndef __USRT_mem_H
#define __USRT_mem_H
#include <usrttype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <CPBuffer.h>

class USRTmem {
  private:
    
    struct structTaskMemHead {
      struct structCPBMeta meta;
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
    USRTmem();
    void newUSRTmem( const char *n );
    void attach( const char *n );
    void newUSRTmem( const char *n, long long dataL, long long cpL, long long resL );
    void init();
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
