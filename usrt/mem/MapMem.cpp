#include <map>
#include <string>
#include <CPBuffer.h>
#include <USRTTask.h>


namespace std {
  map< string, USRTTask* > gUSRTmems;
  map< int64, USRTTask* > gUSRTmemsByKey;
  
extern "C" {
  void initMem( const char *n )
  {
    string memN = string( n );
    gUSRTmems[memN] = new USRTTask();
    gUSRTmems[memN]->newUSRTTask( n );
    gUSRTmems[memN]->init();
    gUSRTmemsByKey[gUSRTmems[memN]->getKey()]=gUSRTmems[memN];
  }

  int attach( const char *n )
  {
    string memN = string( n );
    gUSRTmems[memN] = new USRTTask();
    gUSRTmems[memN]->attach( n );
    gUSRTmemsByKey[gUSRTmems[memN]->getKey()]=gUSRTmems[memN];
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
    string memN = string( n );
    task_t *task = (task_t*)allocMem( n, sizeof(task_t) );
    memset( task, 0, sizeof(task_t) );
    task->ID = ID;
    task->memKey=gUSRTmems[memN]->getKey();
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
    if( gUSRTmemsByKey.find(gUSRTmems[memN]->getKey())!=gUSRTmemsByKey.end() )
      gUSRTmemsByKey.erase(gUSRTmems[memN]->getKey());
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

