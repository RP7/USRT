#include <usrttype.h>
#include "Q7Mem.h"

namespace std {
extern "C" {
  void initQ7Mem( const char *fileName )
  {
    Q7Mem *handle = new Q7Mem();
    handle->newQ7Mem( fileName
      , (long long)(75*4096)
      , (long long)(4096)
      , (long long)sizeof(struct Q7Mem::structQ7MemHead)
      );
  }

  Q7Mem *attachQ7Mem( const char *fileName )
  {
    Q7Mem *h = new Q7Mem();
    h->attach( fileName );
    h->start();
    return h;
  }  
  void dumpQ7Mem( Q7Mem *h )
  {
    h->dumpHead();
  }  
};// extern "C"

};//namespace std

