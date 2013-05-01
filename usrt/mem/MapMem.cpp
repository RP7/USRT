#include <map>
#include <string>
#include <CPBuffer.h>
#include "mem.h"


namespace std {
  map< string, USRTmem* > gUSRTmems;
  
extern "C" {
  void initMem( const char *n )
  {
    string memN = string( n );
    gUSRTmems[memN] = new USRTmem();
    gUSRTmems[memN]->newUSRTmem( n );
    gUSRTmems[memN]->init();
  }

  int attach( const char *n )
  {
    string memN = string( n );
    gUSRTmems[memN] = new USRTmem();
    gUSRTmems[memN]->attach( n );
    char *name = gUSRTmems[memN]->getName();
    if( strcmp(name,n)==0 )
      return 0;
    else
      return -1;
  }  
  void start( const char *n )
  {
    string memN = string( n );
    gUSRTmems[memN]->start();
  }
  void pushTask( const char *n, task_t* t )
  {
    string memN = string( n );
    gUSRTmems[memN]->pushTask(t);
  }
  void *allocMem( const char *n, long long len )
  {
    string memN = string( n );
    return gUSRTmems[memN]->allocMem(len);
  }
  task_t *allocTask(const char *n,int64 ID)
  {
    task_t *task = (task_t*)allocMem( n, sizeof(task_t) );
    task->ID = ID;
    task->lock = 0;
    return task;  
  }
  int len( const char *n )
  {
    string memN = string( n );
    return gUSRTmems[memN]->len();
  }
  int getTask( const char *n, task_t* &ret )
  {
    string memN = string( n );
    return gUSRTmems[memN]->getTask(ret);
  }
  void release(const char *n)
  {
    string memN = string( n );
    delete gUSRTmems[memN];
    gUSRTmems.erase(memN);
  }
  void printStack(const char *n)
  {
    string memN = string( n );
    gUSRTmems[memN]->printStack();
  }
  void dumpHead(const char *n)
  {
    string memN = string( n );
    gUSRTmems[memN]->dumpHead();
  }
  
};// extern "C"

};//namespace std

