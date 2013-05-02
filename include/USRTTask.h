#ifndef __USRT_Task_H
#define __USRT_Task_H
#include <usrttype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <CPBuffer.h>
#include <USRTFifo.h>

class USRTTask : public USRTFifo {
  private:
    void _printStack( int bp );

  public:
    USRTTask();
    void newUSRTTask( const char *n );
    void newUSRTTask( const char *n, long long dataL, long long cpL, long long resL );
    ~USRTTask();
    void pushTask( task_t* task );
    int getTask( task_t* &ret );
    void printStack();
    void printStack( int len );
    void dump( task_t *task );
    char *nameOfKey( int64 *key );
    int snOfKey( int64 *key );
    int64 key2int( ukey_t *key );
};
#endif // __USRT_Task_H
