#include <map>
#include <string>
#include <USRTMem.h>
#include <USRTTask.h>
#include <task.h>

void USRTTask::_printStack( int bp )
{
  if( bp>=STACKSIZE )
    printf("Error too big bp\n");
  printf("digraph G {\n");
  int i;
  if( bp<head->sp ) 
    for( i=bp;i<head->sp;i++ )
      dump( (task_t*)getBuf(head->stack[i],0LL) );
  else {
    for( i=head->sp;i<STACKSIZE;i++ )
      dump( (task_t*)getBuf(head->stack[i],0LL) );
    for( i=0;i<bp;i++ )
      dump( (task_t*)getBuf(head->stack[i],0LL) );
  }
  printf("}\n");
}

void USRTTask::dump( task_t *task ) {
  if( task != NULL ) {
    printf("\"%s(%d)\"->\"%s(%d)\" [label=\"%s(%d)\"];\n"
      ,nameOfKey(&(task->from))
      ,snOfKey(&(task->from))
      ,nameOfKey(&(task->to))
      ,snOfKey(&(task->to))
      ,nameOfKey(&(task->ID))
      ,snOfKey(&(task->ID))
      );
  }
  else
    printf("NULL task\n");
}
char *USRTTask::nameOfKey( int64 *key )
{
	ukey_t *pk=(ukey_t*)key;
	return (char *)(void *)&(pk->name); 
}
int USRTTask::snOfKey( int64 *key )
{
	ukey_t *pk=(ukey_t*)key;
	return (int)(pk->sn); 
}
int64 USRTTask::key2int( ukey_t *key )
{
	return *(int64 *)(key);
}
void USRTTask::printStack( int len )
{
  int bp = (head->sp-len+STACKSIZE)%STACKSIZE;
  _printStack(bp);
}

void USRTTask::printStack( )
{
  _printStack(head->rp);
}
void USRTTask::dumpHead()
{
  USRTMem::dumpHead();
  printf("write point: %d\n",head->sp);
  printf("read point: %d\n",head->rp);    
  if( head->lockS.slock==1 )
    printf("stack lock is unlock\n");
  else
    printf("stack lock is lock\n");
  int bp=head->rp;
  int cnt=0;
  while( bp != head->sp ) {
    printf("%4.4llx ",head->stack[bp]&0xffff);
    bp++;
    if( bp>=STACKSIZE )
      bp=0;
    cnt++;
    if( (cnt&0x7)==0 )
      printf("\n");
  }
  printf("\n");
}

USRTTask::USRTTask()
{
}

void USRTTask::newUSRTTask( const char *n )
{
  newUSRTMem( n
    , (long long)(sizeof(task_t)*STACKSIZE)
    , (long long)(sizeof(task_t))
    , (long long)sizeof(struct structTaskMemHead)
    );
  head = (struct structTaskMemHead*)CPBuffer::attach();
}

void USRTTask::attach( const char *n )
{
  USRTMem::attach( n );
  head = (struct structTaskMemHead*)CPBuffer::attach();
}

void USRTTask::newUSRTTask( const char *n, long long dataL, long long cpL, long long resL )
{
  newUSRTMem( n
    , dataL
    , cpL
    , resL
    );
  head = (struct structTaskMemHead*)CPBuffer::attach();
}
void USRTTask::init()
{
  USRTMem::init();
  memset( ((unsigned char*)head)+sizeof(struct structMemHead)
    , 0
    , sizeof(struct structTaskMemHead)-sizeof(struct structMemHead)
    );
}


USRTTask::~USRTTask()
{ 
}

int USRTTask::len()
{ 
  return (head->sp-head->rp+STACKSIZE)%STACKSIZE;
}

void USRTTask::start()
{ 
  __raw_spin_unlock(&(head->lockS));
  USRTMem::start();
}

void USRTTask::pushTask( task_t* task )
{
  __raw_spin_lock(&(head->lockS));
  head->stack[head->sp]=getOff((void *)task);
  head->sp++;
  if( head->sp>=STACKSIZE )
    head->sp = 0;
  __raw_spin_unlock(&(head->lockS));
}

int USRTTask::getTask( task_t* &ret )
{
  ret = NULL;
  int r=-1;
  __raw_spin_lock(&(head->lockS));
  if( head->rp!=head->sp ) {
    ret = (task_t*)getBuf(head->stack[head->rp],(long long)sizeof(task_t));
    head->rp++;
    if( head->rp>=STACKSIZE )
      head->rp = 0;
    r=0;
  }
  __raw_spin_unlock(&(head->lockS));
  return r;
}
