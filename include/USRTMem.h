#ifndef __USRT_Mem_H
#define __USRT_Mem_H
#include <usrttype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <CPBuffer.h>

class USRTMem : public CPBuffer {
  public:
    struct structMemHead {
      struct structCPBMeta meta;
      raw_spinlock_t lockM;
      long long _brk;
    };
  private:
    struct structMemHead *head;
  public:
    
    USRTMem();
    void attach( const char *n );
    void newUSRTMem( const char *n, long long dataL, long long cpL, long long resL );
    void init();
    void dumpHead();
    const char *getName();
    ~USRTMem();
    void start();
    void *allocMem( long long len );
    void *allocMemAlign( long long len, int align );
    int64 getKey();
};
#endif // __USRT_Mem_H
