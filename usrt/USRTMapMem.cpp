#include <map>
#include <string>
#include <CPBuffer.h>
#include <USRTmem.h>


namespace std {
  map< string, USRTmem* > Tasks;
  
extern "C" {
  void initMem( const char *n )
  {
    string memN = string( n );
    Tasks[memN] = new USRTmem( n );
    Tasks[memN]->init();
    Tasks[memN]->setName( n );
  }
  int attach( const char *n )
  {
    string memN = string( n );
    Tasks[memN] = new USRTmem( n );
    char *name = Tasks[memN]->getName();
    if( strcmp(name,n)==0 )
      return 0;
    else
      return -1;
  }  
  void start( const char *n )
  {
    string memN = string( n );
    Tasks[memN]->start();
  }
  void pushTask( const char *n, task_t* t )
  {
    string memN = string( n );
    Tasks[memN]->pushTask(t);
  }
  void *allocMem( const char *n, long long len )
  {
    string memN = string( n );
    return Tasks[memN]->allocMem(len);
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
    return Tasks[memN]->len();
  }
  int getTask( const char *n, task_t* &ret )
  {
    string memN = string( n );
    return Tasks[memN]->getTask(ret);
  }
  void release(const char *n)
  {
    string memN = string( n );
    delete Tasks[memN];
    Tasks.erase(memN);
  }
  void printStack(const char *n)
  {
    string memN = string( n );
    Tasks[memN]->printStack();
  }
  void dumpHead(const char *n)
  {
    string memN = string( n );
    Tasks[memN]->dumpHead();
  }
  
};// extern "C"

};//namespace std

