#ifndef __USRT_Task_H
#define __USRT_Task_H
#include <usrttype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <CPBuffer.h>
#include <USRTMem.h>

class USRTTask : public USRTMem {
  private:
    struct structTaskMemHead {
      struct structMemHead mem;
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
    ~USRTTask();
    int len();
    void start();
    void pushTask( task_t* task );
    int getTask( task_t* &ret );
    void printStack();
    void printStack( int len );
    void dumpHead();
    void dump( task_t *task );
    char *nameOfKey( int64 *key );
    int snOfKey( int64 *key );
    int64 key2int( ukey_t *key );
};
#endif // __USRT_Task_H
