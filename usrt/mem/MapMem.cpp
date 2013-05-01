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
    fprintf(stderr,"file %s\n",n);
        
    gUSRTmems[memN] = new USRTTask();
    gUSRTmems[memN]->attach( n );
    
    gUSRTmemsByKey[gUSRTmems[memN]->getKey()]=gUSRTmems[memN];
    const char *name = gUSRTmems[memN]->getName();
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
    task->mem.memKey=gUSRTmems[memN]->getKey();
    task->mem.offset=gUSRTmems[memN]->getOff((void *)task);
    task->mem.len=(long long)sizeof(task_t);
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
  static const char *findFileByKey( int64 key )
  {
    CPBuffer *t = new CPBuffer();
    const char *ret = t->findByKey( key );
    delete t;
    return ret;
  }
  void *G2L( generalized_memory_t *gp )
  {
    map<int64, USRTTask*>::iterator iter = gUSRTmemsByKey.find(gp->memKey);
    if( iter==gUSRTmemsByKey.end() ) {
      const char* fn = findFileByKey(gp->memKey);
      if( fn !=NULL )
      {
        char _fn[256];
        strcpy(_fn,fn);
        attach( _fn );
      }
      iter=gUSRTmemsByKey.find(gp->memKey);
    }
    if( iter!=gUSRTmemsByKey.end() ) 
      return iter->second->getBuf( gp->offset, gp->len );
    else
      return NULL;
  }
};// extern "C"

};//namespace std

