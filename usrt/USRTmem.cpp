#include <map>
#include <string>
#include <CPBuffer.h>
#include <USRTmem.h>

void USRTmem::printStack()
{
  printf("digraph G {\n");
  int i;
  int bp = (head->sp-len+STACKSIZE)%STACKSIZE;
  if( bp>head->sp ) 
    for( i=head->sp;i<bp;i++ )
      dump( buf->getBuf(head->stack[i],0LL) );
  else {
    for( i=head->sp;i<STACKSIZE;i++ )
      dump( buf->getBuf(head->stack[i],0LL) );
    for( i=0;i<bp;i++ )
      dump( buf->getBuf(head->stack[i],0LL) );
  }
  printf("}\n");
}

USRTmem::USRTmem( const char *n )
{
  buf = new CPBuffer( (long long)(sizeof(task_t)*STACKSIZE)
    , (long long)(sizeof(task_t))
    , (long long)sizeof(struct structTaskMemHead)
    , n
    );
  head = (struct structTaskMemHead*)buf->attach();
}

void USRTmem::init()
{
  memset(head,0,sizeof(struct structTaskMemHead);
}

void USRTmem::setName( const char *n )
{
  strcmp(head->name,n);
}

char* USRTmem::getName()
{
  return head->name;
}

USRTmem::~USRTmem()
{ 
  if( buf )
    delete buf;
  buf=NULL;
}

int USRTmem::len()
{ 
  return (head->sp-head->rp+STACKSIZE)%STACKSIZE;
}

void USRTmem::start()
{ 
  __raw_spin_unlock(&(head->lockS));
  __raw_spin_unlock(&(head->lockM));
}

void *USRTmem::allocMem( long long len )
{
  __raw_spin_lock(&(head->lockM));
  long long last = brk;
  brk+=len;
  __raw_spin_unlock(&(head->lockM));
  if( buf )
    return buf->getBuf(last,len);
  else
    return NULL;
}

void USRTmem::pushTask( task_t* task )
{
  __raw_spin_lock(&(head->lockS));
  head->stack[head->sp]=buf->getOff((void *)task);
  head->sp++;
  if( head->sp>=STACKSIZE )
    head->sp = 0;
  __raw_spin_unlock(&(head->lockS));
}

int USRTmem::getTask( task_t* &ret )
{
  ret = NULL;
  int r=-1;
  __raw_spin_lock(&(head->lockS));
  if( head->rp!=head->sp ) {
    ret = (task_t*)buf->getBuf(head->stack[head->rp],(long long)sizeof(task_t));
    head->rp++;
    if( head->rp>=STACKSIZE )
      head->rp = 0;
    r=0;
  }
  __raw_spin_unlock(&(head->lockS));
  return r;
}

namespace std {
  map< string, USRTmem* > Tasks;
  
extern "C" {
  void init( const char *n )
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
  void pushTask( const char *n, task_t t )
  {
    string memN = string( n );
    Tasks[memN]->pushTask(t);
  }
  void *allocMem( const char *n, long long len )
  {
    string memN = string( n );
    return Tasks[memN]->allocMem(len);
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
};// extern "C"

};//namespace std

