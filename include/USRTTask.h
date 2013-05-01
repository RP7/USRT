#ifndef __USRT_Task_H
#define __USRT_Task_H
#include <usrttype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <CPBuffer.h>

class USRTTask : CPBuffer {
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
    void _printStack( int bp );

  public:
    USRTTask();
    void newUSRTTask( const char *n );
    void attach( const char *n );
    void newUSRTTask( const char *n, long long dataL, long long cpL, long long resL );
    void init();
    char *getName();
    ~USRTTask();
    int len();
    void start();
    void *allocMem( long long len );
    void pushTask( task_t* task );
    int getTask( task_t* &ret );
    void printStack();
    void printStack( int len );
    void dumpHead();
    int64 getKey();
};
#endif // __USRT_Task_H
